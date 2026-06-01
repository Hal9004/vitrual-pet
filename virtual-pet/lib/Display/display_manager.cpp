#include "display_manager.h"

DisplayManager::DisplayManager()
    : lastRenderedScreen(SCREEN_MAIN),
      lastFullRedrawTime(0),
      lastMenuActionIndex(-1),
      petWasDeadLastFrame(false) {
}

void DisplayManager::init() {
    clearScreen(TFT_BLACK);
}

void DisplayManager::clearScreen(uint32_t color) {
    M5.Lcd.clear(color);
}

void DisplayManager::fillRect(int x, int y, int width, int height, uint32_t color) {
    M5.Lcd.fillRect(x, y, width, height, color);
}

// renderDisplay() — the one call loop() makes every frame.
// It decides whether to show the death screen or one of the three live screens,
// then delegates to the appropriate private render method.
//
// Action-menu information arrives as three primitives (name, relevant stat,
// current index) rather than as an ActionMenu reference. This keeps
// DisplayManager unaware of ActionMenu — the caller does the extraction.
void DisplayManager::renderDisplay(int happiness, int hunger, int energy, int cleanliness,
                                   int sick, int moodIndex, const char* selectedActionName,
                                   RelevantStat relevantStat, int currentActionIndex,
                                   bool petIsDead, const char* petName,
                                   ScreenState screenState) {
    // --- Death state ---
    // Draw the death screen once when the pet first dies, then hold it.
    // The petWasDeadLastFrame flag prevents redrawing the death screen every frame.
    if (petIsDead) {
        if (!petWasDeadLastFrame) {
            petWasDeadLastFrame = true;
            showDeathScreen();
        }
        return;
    }

    // --- Revival: coming back from dead ---
    // The pet was just reset. Set lastFullRedrawTime to 0 so the normal screen
    // redraws immediately on the very next frame instead of waiting up to 5 seconds.
    if (petWasDeadLastFrame) {
        petWasDeadLastFrame = false;
        lastFullRedrawTime = 0;
    }

    // --- Normal alive render: dispatch to the correct screen ---
    switch (screenState) {
        case SCREEN_MAIN:
            renderMainScreen(moodIndex, petName);
            break;
        case SCREEN_STATS:
            renderStatsScreen(happiness, hunger, energy, cleanliness, sick, moodIndex, petName);
            break;
        case SCREEN_INTERACT:
            renderInteractScreen(happiness, hunger, energy, cleanliness, sick, moodIndex,
                                 selectedActionName, relevantStat, currentActionIndex, petName);
            break;
    }
}

// -----------------------------------------------------------------------
// Main screen render
// Shows the pet face filling the centre of the screen, with a two-tab nav
// bar at the bottom so the user can enter Stats or Interact.
// -----------------------------------------------------------------------
void DisplayManager::renderMainScreen(int moodIndex, const char* petName) {
    bool screenChanged   = (lastRenderedScreen != SCREEN_MAIN);
    bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

    if (screenChanged || intervalElapsed) {
        clearScreen();
        printCenteredText(petName, TITLE_ZONE.y, TFT_YELLOW, 2);
        drawPetSprite(moodIndex, MAIN_FACE_CENTER_Y, SPRITE_80X80_TEST_WIDTH, SPRITE_80X80_TEST_HEIGHT, sprite_80x80_test[0]);
        showPetMoodText(moodIndex, MAIN_MOOD_Y);
        drawMainNavBar();

        lastFullRedrawTime = millis();
        lastRenderedScreen = SCREEN_MAIN;
    }
}

// drawMainNavBar()
// Draws the two-tab bar at the bottom of the Main screen. The nav bar is the
// user's entry point into the rest of the app — without it the user would
// have no visible cue that B and C take them to the Interact and Stats
// screens. Both tabs are drawn the same way (no highlight state) because
// the Main screen has no concept of "selected tab"; a single press jumps
// straight to the chosen screen.
void DisplayManager::drawMainNavBar() {
    int tabWidth = MAIN_NAV_ZONE.width / 2;

    // Left tab — Stats
    M5.Lcd.drawRect(MAIN_NAV_ZONE.x, MAIN_NAV_ZONE.y, tabWidth, MAIN_NAV_ZONE.height, TFT_CYAN);
    printText("Stats", MAIN_NAV_ZONE.x + 6, MAIN_NAV_ZONE.y + 5, TFT_CYAN, 1);

    // Right tab — Interact
    int rightX = MAIN_NAV_ZONE.x + tabWidth;
    M5.Lcd.drawRect(rightX, MAIN_NAV_ZONE.y, tabWidth, MAIN_NAV_ZONE.height, TFT_CYAN);
    printText("Interact", rightX + 3, MAIN_NAV_ZONE.y + 5, TFT_CYAN, 1);
}

// -----------------------------------------------------------------------
// Stats screen render
// Preserves the original layout exactly: stat bars at the top, face below.
// A "B/C: Back" hint replaces the old action menu indicator at the bottom.
// -----------------------------------------------------------------------
void DisplayManager::renderStatsScreen(int happiness, int hunger, int energy, int cleanliness,
                                       int sick, int moodIndex, const char* petName) {
    bool screenChanged   = (lastRenderedScreen != SCREEN_STATS);
    bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

    if (screenChanged || intervalElapsed) {
        clearScreen();
        showPetStatus(happiness, hunger, energy, cleanliness, sick, petName);
        showPetMood(moodIndex);

        // Back hint at the bottom instead of the action menu indicator
        M5.Lcd.drawRect(MENU_ZONE.x, MENU_ZONE.y, MENU_ZONE.width, MENU_ZONE.height, TFT_CYAN);
        printText("B/C: Back", MENU_ZONE.x + 10, MENU_ZONE.y + 4, TFT_CYAN, 1);

        lastFullRedrawTime = millis();
        lastRenderedScreen = SCREEN_STATS;
    }
}

// -----------------------------------------------------------------------
// Interact screen render
// Shows the pet face in the top half, a single contextual stat bar in the
// middle (the stat the selected action affects), and the action menu
// indicator at the very bottom — the same indicator style as before.
// -----------------------------------------------------------------------
void DisplayManager::renderInteractScreen(int happiness, int hunger, int energy, int cleanliness,
                                          int sick, int moodIndex, const char* selectedActionName,
                                          RelevantStat relevantStat, int currentActionIndex,
                                          const char* petName) {
    bool screenChanged   = (lastRenderedScreen != SCREEN_INTERACT);
    bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

    if (screenChanged || intervalElapsed) {
        clearScreen();
        printCenteredText(petName, TITLE_ZONE.y, TFT_YELLOW, 2);
        drawPetSprite(moodIndex, INTERACT_FACE_CENTER_Y, SPRITE_80X80_TEST_WIDTH, SPRITE_80X80_TEST_HEIGHT, sprite_80x80_test[0]);
        showPetMoodText(moodIndex, INTERACT_MOOD_Y);
        drawContextualStatBar(happiness, hunger, energy, cleanliness, sick, relevantStat);
        drawMenuIndicator(selectedActionName, MENU_ZONE.x, MENU_ZONE.y);

        lastMenuActionIndex = currentActionIndex;
        lastFullRedrawTime  = millis();
        lastRenderedScreen  = SCREEN_INTERACT;
        return;
    }

    // Fast path: when the user presses B or C to cycle actions, only redraw
    // the contextual stat bar and the menu indicator — not the whole screen.
    // This prevents the face from flickering on every button press.
    if (currentActionIndex != lastMenuActionIndex) {
        drawContextualStatBar(happiness, hunger, energy, cleanliness, sick, relevantStat);
        drawMenuIndicator(selectedActionName, MENU_ZONE.x, MENU_ZONE.y);
        lastMenuActionIndex = currentActionIndex;
    }
}

// drawContextualStatBar()
// Draws a single stat bar in the INTERACT_STAT_ZONE area — only the one stat
// that the currently selected action affects. The label, current value, and
// colour all come from the relevantStat parameter, so the bar changes as the
// user scrolls through actions.
//
// Why a single stat instead of all five? The Interact screen exists for the
// user to choose an action and see its consequence. Showing only the stat
// that will change keeps the focus on the cause-and-effect link between the
// selected action and the pet's response.
void DisplayManager::drawContextualStatBar(int happiness, int hunger, int energy,
                                           int cleanliness, int sick, RelevantStat relevantStat) {
    // Clear the area first so the previous bar does not bleed through
    fillRect(INTERACT_STAT_ZONE.x, INTERACT_STAT_ZONE.y,
             INTERACT_STAT_ZONE.width, INTERACT_STAT_ZONE.height, TFT_BLACK);

    // Save and Back have no specific stat — leave the area blank
    if (relevantStat == STAT_NONE) {
        return;
    }

    const char* label;
    int         value;
    uint32_t    color;

    switch (relevantStat) {
        case STAT_HAPPINESS:  label = "Happy";  value = happiness;  color = TFT_GREEN;  break;
        case STAT_HUNGER:     label = "Hunger"; value = hunger;     color = TFT_RED;    break;
        case STAT_ENERGY:     label = "Energy"; value = energy;     color = TFT_BLUE;   break;
        case STAT_CLEANLINESS:label = "Clean";  value = cleanliness;color = TFT_CYAN;   break;
        case STAT_SICKNESS:   label = "Sick";   value = sick;       color = TFT_PURPLE; break;
        default:              return;
    }

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setCursor(INTERACT_STAT_ZONE.x, INTERACT_STAT_LABEL_Y);
    M5.Lcd.printf("%s: %d", label, value);
    drawStatusBar(value, 100, INTERACT_STAT_ZONE.x, INTERACT_STAT_BAR_Y, INTERACT_STAT_ZONE.width, color);
}

// -----------------------------------------------------------------------
// Shared helpers used by more than one render method
// -----------------------------------------------------------------------

// drawMenuIndicator() — the compact action name box at the very bottom.
// Used on the Interact screen. Clears its area before drawing.
// Takes the action name as a plain string — no ActionMenu reference needed —
// so this helper has no knowledge of the menu's internals.
void DisplayManager::drawMenuIndicator(const char* selectedActionName, int x, int y) {
    fillRect(x, y, MENU_ZONE.width, MENU_ZONE.height, TFT_BLACK);
    M5.Lcd.drawRect(x, y, MENU_ZONE.width, MENU_ZONE.height, TFT_CYAN);
    printText("Action: ", x + 2, y + 4, TFT_CYAN, 1);
    printText(selectedActionName, x + 50, y + 4, TFT_YELLOW, 1);
}

// showPetMoodText() — draws just the mood label at the given Y position.
// Separated from drawPetSprite() so the Main and Interact screens can place
// the text at different heights without calling the full showPetMood() path.
void DisplayManager::showPetMoodText(int moodIndex, int textY) {
    const char* moodText;
    uint32_t    moodColor;

    switch (moodIndex) {
        case 0: moodText = "Hungry";    moodColor = TFT_RED;    break;
        case 1: moodText = "Tired";     moodColor = TFT_BLUE;   break;
        case 2: moodText = "Happy";     moodColor = TFT_GREEN;  break;
        case 3: moodText = "Sick";      moodColor = TFT_PURPLE; break;
        case 4: moodText = "Sad";       moodColor = TFT_ORANGE; break;
        case 5: moodText = "Clean";     moodColor = TFT_CYAN;   break;
        case 6: moodText = "Energised"; moodColor = TFT_YELLOW; break;
        default:moodText = "Neutral";   moodColor = TFT_WHITE;  break;
    }

    printCenteredText(moodText, textY, moodColor, 2);
}

// showPetMood() — convenience wrapper that draws both the face and the mood
// text at the Stats screen positions. Used by renderStatsScreen().
void DisplayManager::showPetMood(int moodIndex) {
    drawPetSprite(moodIndex, PET_FACE_ZONE.y + PET_FACE_ZONE.height / 2, SPRITE_NEWPISKEL2_WIDTH, SPRITE_NEWPISKEL2_HEIGHT, sprite_newpiskel2[0]);
    showPetMoodText(moodIndex, MOOD_ZONE.y);
}

// showPetStatus() — draws all five labelled stat bars using the Stats screen zones.
// Does not clear the screen — the caller must do that first.
void DisplayManager::showPetStatus(int happiness, int hunger, int energy, int cleanliness,
                                   int sick, const char* petName) {
    printCenteredText(petName, TITLE_ZONE.y, TFT_YELLOW, 2);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE);

    M5.Lcd.setCursor(STATS_ZONE.x, HAPPY_BAR_ZONE.labelY);
    M5.Lcd.printf("Happy: %d", happiness);
    drawStatusBar(happiness, 100, STATS_ZONE.x, HAPPY_BAR_ZONE.barY, STATS_ZONE.width, TFT_GREEN);

    M5.Lcd.setCursor(STATS_ZONE.x, HUNGER_BAR_ZONE.labelY);
    M5.Lcd.printf("Hunger: %d", hunger);
    drawStatusBar(hunger, 100, STATS_ZONE.x, HUNGER_BAR_ZONE.barY, STATS_ZONE.width, TFT_RED);

    M5.Lcd.setCursor(STATS_ZONE.x, ENERGY_BAR_ZONE.labelY);
    M5.Lcd.printf("Energy: %d", energy);
    drawStatusBar(energy, 100, STATS_ZONE.x, ENERGY_BAR_ZONE.barY, STATS_ZONE.width, TFT_BLUE);

    M5.Lcd.setCursor(STATS_ZONE.x, CLEAN_BAR_ZONE.labelY);
    M5.Lcd.printf("Clean: %d", cleanliness);
    drawStatusBar(cleanliness, 100, STATS_ZONE.x, CLEAN_BAR_ZONE.barY, STATS_ZONE.width, TFT_CYAN);

    M5.Lcd.setCursor(STATS_ZONE.x, SICK_BAR_ZONE.labelY);
    M5.Lcd.printf("Sick: %d", sick);
    drawStatusBar(sick, 100, STATS_ZONE.x, SICK_BAR_ZONE.barY, STATS_ZONE.width, TFT_PURPLE);
}

// drawPetSprite() — draws the pet's face as a bitmap sprite.
// The sprite is centred horizontally on the screen, and its vertical centre
// is positioned at faceCenterY so each screen can choose where the face sits.
//
// Why pass the sprite data and dimensions in as parameters?
// Each screen wants a different sprite size: Stats uses a tiny 32x32, Interact
// uses 48x48, and Main uses 64x64. Passing the sprite data and its width/height
// in lets this single function draw any sprite without needing a switch on
// screen state. The caller decides which sprite to use.
//
// Why is moodIndex unused for now?
// Eventually each mood (happy, sad, hungry, etc.) will have its own sprite.
// Keeping moodIndex in the signature today means the call sites are already
// the right shape when we add that lookup later — no second round of edits.
//
// Why SPRITE_TRANSPARENT_COLOR?
// The sprite is a square block of pixels, but the pet's outline is not square.
// Pixels matching SPRITE_TRANSPARENT_COLOR (0x1FF8) are skipped by pushImage(),
// so the screen background shows through and the pet appears to "float"
// instead of sitting inside a coloured rectangle.
void DisplayManager::drawPetSprite(int moodIndex, int faceCenterY, int spriteWidth, int spriteHeight, const uint16_t* spriteData) {
    int spriteX = (SCREEN_WIDTH - spriteWidth) / 2;
    int spriteY = faceCenterY - (spriteHeight / 2);

    M5.Lcd.pushImage(spriteX, spriteY, spriteWidth, spriteHeight, spriteData, SPRITE_TRANSPARENT_COLOR);
}

// -----------------------------------------------------------------------
// Primitive drawing and text helpers
// -----------------------------------------------------------------------

void DisplayManager::printText(const char* text, int x, int y, uint32_t color, uint8_t size) {
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextColor(color);
    M5.Lcd.setTextSize(size);
    M5.Lcd.print(text);
}

void DisplayManager::printCenteredText(const char* text, int y, uint32_t color, uint8_t size) {
    M5.Lcd.setTextColor(color);
    M5.Lcd.setTextSize(size);

    int textWidth = strlen(text) * 6 * size;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    if (x < 0) { x = 0; }

    M5.Lcd.setCursor(x, y);
    M5.Lcd.print(text);
}

void DisplayManager::drawStatusBar(int value, int maxValue, int x, int y, int width, uint32_t color) {
    M5.Lcd.drawRect(x, y, width, STAT_BAR_HEIGHT, TFT_WHITE);

    int fillWidth = (value * width) / maxValue;
    if (fillWidth > 0) {
        M5.Lcd.fillRect(x, y, fillWidth, STAT_BAR_HEIGHT, color);
    }
}

void DisplayManager::showMessage(const char* message) {
    printCenteredText(message, 160, TFT_WHITE, 1);
}

void DisplayManager::showActionFeedback(const char* action) {
    printCenteredText(action, 180, TFT_GREEN, 1);
}

// showDeathScreen() — clears the screen and shows game-over text.
// Forces lastFullRedrawTime to 0 so the normal screen redraws immediately
// the moment the user restarts.
void DisplayManager::showDeathScreen() {
    clearScreen(TFT_BLACK);

    printCenteredText("Your pet", 60, TFT_RED, 2);
    printCenteredText("has died!", 85, TFT_RED, 2);
    printCenteredText("Press A to", 130, TFT_WHITE, 2);
    printCenteredText("restart", 155, TFT_WHITE, 2);

    lastFullRedrawTime = 0;
}
