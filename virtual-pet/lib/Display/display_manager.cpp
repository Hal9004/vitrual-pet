#include "display_manager.h"
#include "../Actions/action_menu.h"

DisplayManager::DisplayManager()
    : lastRenderedScreen(SCREEN_MAIN),
      lastFullRedrawTime(0),
      lastMenuActionIndex(-1),
      petWasDeadLastFrame(false) {
}

void DisplayManager::init() {
    clearScreen(TFT_BLACK);
}

void DisplayManager::clearScreen() {
    M5.Lcd.clear(TFT_BLACK);
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
void DisplayManager::renderDisplay(int happiness, int hunger, int energy, int cleanliness,
                                   int sick, int moodIndex, const ActionMenu& menu,
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
            renderInteractScreen(happiness, hunger, energy, cleanliness, sick, moodIndex, menu, petName);
            break;
    }
}

// -----------------------------------------------------------------------
// SCREEN_MAIN render
// Shows the pet face filling the centre of the screen, with a two-tab nav
// bar at the bottom so the player can enter Stats or Interact.
// -----------------------------------------------------------------------
void DisplayManager::renderMainScreen(int moodIndex, const char* petName) {
    bool screenChanged   = (lastRenderedScreen != SCREEN_MAIN);
    bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

    if (screenChanged || intervalElapsed) {
        clearScreen();
        printCenteredText(petName, TITLE_ZONE.y, TFT_YELLOW, 2);
        drawPetFace(moodIndex, MAIN_FACE_CENTER_Y, MAIN_FACE_RADIUS);
        showPetMoodText(moodIndex, MAIN_MOOD_Y);
        drawMainNavBar();

        lastFullRedrawTime = millis();
        lastRenderedScreen = SCREEN_MAIN;
    }
}

// Draws the two-tab bar at the bottom of the Main screen.
// Both tabs are always drawn the same way — no highlight state needed.
// B switches to Interact; C switches to Stats (handled in NavigationManager).
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
// SCREEN_STATS render
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
// SCREEN_INTERACT render
// Shows the pet face in the top half, a single contextual stat bar in the
// middle (the stat the selected action affects), and the action menu
// indicator at the very bottom — the same indicator style as before.
// -----------------------------------------------------------------------
void DisplayManager::renderInteractScreen(int happiness, int hunger, int energy, int cleanliness,
                                          int sick, int moodIndex, const ActionMenu& menu,
                                          const char* petName) {
    bool screenChanged   = (lastRenderedScreen != SCREEN_INTERACT);
    bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

    if (screenChanged || intervalElapsed) {
        clearScreen();
        printCenteredText(petName, TITLE_ZONE.y, TFT_YELLOW, 2);
        drawPetFace(moodIndex, INTERACT_FACE_CENTER_Y, INTERACT_FACE_RADIUS);
        showPetMoodText(moodIndex, INTERACT_MOOD_Y);
        drawContextualStatBar(happiness, hunger, energy, cleanliness, sick, menu.getRelevantStat());
        drawMenuIndicator(menu, MENU_ZONE.x, MENU_ZONE.y);

        lastMenuActionIndex = menu.getCurrentActionIndex();
        lastFullRedrawTime  = millis();
        lastRenderedScreen  = SCREEN_INTERACT;
        return;
    }

    // Fast path: when the player presses B or C to cycle actions, only redraw
    // the contextual stat bar and the menu indicator — not the whole screen.
    // This prevents the face from flickering on every button press.
    if (menu.getCurrentActionIndex() != lastMenuActionIndex) {
        drawContextualStatBar(happiness, hunger, energy, cleanliness, sick, menu.getRelevantStat());
        drawMenuIndicator(menu, MENU_ZONE.x, MENU_ZONE.y);
        lastMenuActionIndex = menu.getCurrentActionIndex();
    }
}

// Draws a single stat bar in the INTERACT_STAT_ZONE area.
// The label, current value, and colour all come from the relevantStat field
// of the currently selected action — so the bar changes as the player scrolls.
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
void DisplayManager::drawMenuIndicator(const ActionMenu& menu, int x, int y) {
    Action selectedAction = menu.getSelectedAction();

    fillRect(x, y, MENU_ZONE.width, MENU_ZONE.height, TFT_BLACK);
    M5.Lcd.drawRect(x, y, MENU_ZONE.width, MENU_ZONE.height, TFT_CYAN);
    printText("Action: ", x + 2, y + 4, TFT_CYAN, 1);
    printText(selectedAction.name, x + 50, y + 4, TFT_YELLOW, 1);
}

// showPetMoodText() — draws just the mood label at the given Y position.
// Separated from drawPetFace() so the Main and Interact screens can place
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
    drawPetFace(moodIndex, PET_FACE_ZONE.y + PET_FACE_ZONE.height / 2, 18);
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

// drawPetFace() — draws a face circle, two eyes, and a mood-based mouth at
// the given centre Y position and radius. Called with different values on
// each screen so the face size and position can vary per layout.
void DisplayManager::drawPetFace(int moodIndex, int faceCenterY, int faceRadius) {
    int centerX = SCREEN_WIDTH / 2;

    M5.Lcd.drawCircle(centerX, faceCenterY, faceRadius, TFT_WHITE);

    M5.Lcd.fillCircle(centerX - 7, faceCenterY - 5, 2, TFT_WHITE);
    M5.Lcd.fillCircle(centerX + 7, faceCenterY - 5, 2, TFT_WHITE);

    switch (moodIndex) {
        case 2: // Happy — smile curves upward
            M5.Lcd.drawCircle(centerX, faceCenterY + 3, 6, TFT_WHITE);
            M5.Lcd.fillCircle(centerX, faceCenterY + 3, 6, TFT_BLACK);
            M5.Lcd.drawCircle(centerX, faceCenterY + 1, 6, TFT_WHITE);
            break;
        case 4: // Sad — frown curves downward
            M5.Lcd.drawCircle(centerX, faceCenterY + 9, 6, TFT_WHITE);
            M5.Lcd.fillCircle(centerX, faceCenterY + 9, 6, TFT_BLACK);
            M5.Lcd.drawCircle(centerX, faceCenterY + 11, 6, TFT_WHITE);
            break;
        default: // Neutral — straight line
            M5.Lcd.drawLine(centerX - 6, faceCenterY + 6, centerX + 6, faceCenterY + 6, TFT_WHITE);
            break;
    }
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

void DisplayManager::printText(String text, int x, int y, uint32_t color, uint8_t size) {
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
// the moment the player restarts.
void DisplayManager::showDeathScreen() {
    clearScreen(TFT_BLACK);

    printCenteredText("Your pet", 60, TFT_RED, 2);
    printCenteredText("has died!", 85, TFT_RED, 2);
    printCenteredText("Press A to", 130, TFT_WHITE, 2);
    printCenteredText("restart", 155, TFT_WHITE, 2);

    lastFullRedrawTime = 0;
}
