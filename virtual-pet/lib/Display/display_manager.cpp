#include "display_manager.h"
#include "../Actions/action_menu.h"

// Constructor
DisplayManager::DisplayManager() 
    : currentState(DisplayState::STATUS_VIEW),
      lastStatusUpdate(0),
      lastHappiness(0),
      lastHunger(0),
      lastEnergy(0),
      lastMoodIndex(0) {
    // Constructor - initialization handled in init()
}

// Initialize the display
void DisplayManager::init() {
    // M5.begin() should be called in main setup()
    // This method can be used for any additional display setup
    clearScreen(TFT_BLACK);
}

// Clear the entire screen
void DisplayManager::clearScreen() {
    M5.Lcd.clear(TFT_BLACK);
}

void DisplayManager::clearScreen(uint32_t color) {
    M5.Lcd.clear(color);
}

// Fill a rectangular region with a color
void DisplayManager::fillRect(int x, int y, int width, int height, uint32_t color) {
    M5.Lcd.fillRect(x, y, width, height, color);
}

// Set the current display state
void DisplayManager::setDisplayState(DisplayState newState) {
    currentState = newState;
}

// Get the current display state
DisplayState DisplayManager::getDisplayState() const {
    return currentState;
}

// Check if enough time has passed to update the status display
bool DisplayManager::shouldUpdateStatusDisplay() {
    unsigned long currentTime = millis();
    if (currentTime - lastStatusUpdate >= STATUS_UPDATE_INTERVAL) {
        lastStatusUpdate = currentTime;
        return true;
    }
    return false;
}

// Render the entire display based on current state
// This is the main coordinated render function
void DisplayManager::renderFrame(int happiness, int hunger, int energy, int moodIndex) {
    // Only update if status has changed or if it's time for a refresh
    if (currentState == DisplayState::STATUS_VIEW && shouldUpdateStatusDisplay()) {
        // Clear once for full status update
        clearScreen();
        showPetStatus(happiness, hunger, energy);
        showPetMood(moodIndex);
        lastHappiness = happiness;
        lastHunger = hunger;
        lastEnergy = energy;
        lastMoodIndex = moodIndex;
    }
}

// Unified display render - renders pet status AND menu indicator together at same refresh rate.
// Also handles the death screen. Comparing currentState before drawing prevents redrawing
// the same screen every frame, which is what causes LCD flicker.
void DisplayManager::renderDisplay(int happiness, int hunger, int energy, int moodIndex, const ActionMenu& menu, bool petIsDead) {
    // --- Death state ---
    // If the pet just died, draw the death screen once and stop.
    // The currentState check means we only draw on the first frame of death, not every frame.
    if (petIsDead) {
        if (currentState != DisplayState::DEAD) {
            currentState = DisplayState::DEAD;
            showDeathScreen();
        }
        return;
    }

    // --- Revival: coming back from dead state ---
    // The pet was just reset. Switch back to STATUS_VIEW and clear lastStatusUpdate
    // so the normal screen redraws immediately instead of waiting up to 5 seconds.
    if (currentState == DisplayState::DEAD) {
        currentState = DisplayState::STATUS_VIEW;
        lastStatusUpdate = 0;
    }

    // --- Normal alive render ---
    // Only redraw every STATUS_UPDATE_INTERVAL milliseconds to avoid flicker.
    if (currentState == DisplayState::STATUS_VIEW && shouldUpdateStatusDisplay()) {
        clearScreen();
        showPetStatus(happiness, hunger, energy);
        showPetMood(moodIndex);
        drawMenuIndicator(menu, 5, 220);

        lastHappiness = happiness;
        lastHunger = hunger;
        lastEnergy = energy;
        lastMoodIndex = moodIndex;
    }
}

// Draw menu indicator - moved to DisplayManager for coordinated rendering
void DisplayManager::drawMenuIndicator(const ActionMenu& menu, int x, int y) {
    Action selectedAction = menu.getSelectedAction();
    
    // Clear only the indicator area before redrawing
    fillRect(x, y, 130, 20, TFT_BLACK);
    
    // Draw small box around the indication area
    M5.Lcd.drawRect(x, y, 130, 20, TFT_CYAN);
    
    // Show selected action name in small text
    printText("Action: ", x + 2, y + 4, TFT_CYAN, 1);
    printText(selectedAction.name, x + 50, y + 4, TFT_YELLOW, 1);
}


// Print text at specific coordinates
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

// Print centered text on a line
void DisplayManager::printCenteredText(const char* text, int y, uint32_t color, uint8_t size) {
    M5.Lcd.setTextColor(color);
    M5.Lcd.setTextSize(size);

    // Calculate center position
    int textWidth = strlen(text) * 6 * size;  // Approximate character width
    int x = (SCREEN_WIDTH - textWidth) / 2;
    if (x < 0) x = 0;  // Ensure text doesn't go off screen

    M5.Lcd.setCursor(x, y);
    M5.Lcd.print(text);
}

// Show pet status information (does not clear screen - must be called by renderFrame)
void DisplayManager::showPetStatus(int happiness, int hunger, int energy) {
    // Title
    printCenteredText("Virtual Pet", 10, TFT_YELLOW, 2);

    // Status bars
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE);

    // Happiness
    M5.Lcd.setCursor(5, 40);
    M5.Lcd.printf("Happy: %d", happiness);
    drawStatusBar(happiness, 100, 5, 50, 100, TFT_GREEN);

    // Hunger
    M5.Lcd.setCursor(5, 70);
    M5.Lcd.printf("Hunger: %d", hunger);
    drawStatusBar(hunger, 100, 5, 80, 100, TFT_RED);

    // Energy
    M5.Lcd.setCursor(5, 100);
    M5.Lcd.printf("Energy: %d", energy);
    drawStatusBar(energy, 100, 5, 110, 100, TFT_BLUE);
}

// Show current pet mood (does not clear screen - called by renderFrame after showPetStatus)
void DisplayManager::showPetMood(int moodIndex) {
    const char* moodText;
    uint32_t moodColor;

    switch(moodIndex) {
        case 0: moodText = "Hungry"; moodColor = TFT_RED; break;
        case 1: moodText = "Tired"; moodColor = TFT_BLUE; break;
        case 2: moodText = "Happy"; moodColor = TFT_GREEN; break;
        case 3: moodText = "Sick"; moodColor = TFT_PURPLE; break;
        case 4: moodText = "Sad"; moodColor = TFT_ORANGE; break;
        case 5: moodText = "Clean"; moodColor = TFT_CYAN; break;
        case 6: moodText = "Energised"; moodColor = TFT_YELLOW; break;
        default: moodText = "Neutral"; moodColor = TFT_WHITE; break;
    }

    // Show mood at bottom of screen
    printCenteredText(moodText, 200, moodColor, 2);

    // Draw simple pet face based on mood
    drawPetFace(moodIndex);
}

// Show temporary message
void DisplayManager::showMessage(const char* message) {
    printCenteredText(message, 160, TFT_WHITE, 1);
}

// Show action feedback
void DisplayManager::showActionFeedback(const char* action) {
    printCenteredText(action, 180, TFT_GREEN, 1);
}

// Draw a status bar
void DisplayManager::drawStatusBar(int value, int maxValue, int x, int y, int width, uint32_t color) {
    // Background bar
    M5.Lcd.drawRect(x, y, width, 10, TFT_WHITE);

    // Filled portion
    int fillWidth = (value * width) / maxValue;
    if (fillWidth > 0) {
        M5.Lcd.fillRect(x, y, fillWidth, 10, color);
    }
}

// showDeathScreen()
// Clears the screen and displays a game-over message with restart instructions.
// Resets lastStatusUpdate so the normal status screen redraws immediately after restart.
void DisplayManager::showDeathScreen() {
    clearScreen(TFT_BLACK);

    // Red title to signal the pet has died
    printCenteredText("Your pet", 60, TFT_RED, 2);
    printCenteredText("has died!", 85, TFT_RED, 2);

    // Instruction line below
    printCenteredText("Press A to", 130, TFT_WHITE, 2);
    printCenteredText("restart", 155, TFT_WHITE, 2);

    // Force a full status redraw on the next loop after the player restarts
    lastStatusUpdate = 0;
}

// Draw a simple pet face based on mood
void DisplayManager::drawPetFace(int moodIndex) {
    int centerX = SCREEN_WIDTH / 2;
    int faceY = 140;

    // Face circle
    M5.Lcd.drawCircle(centerX, faceY, 25, TFT_WHITE);

    // Eyes
    M5.Lcd.fillCircle(centerX - 10, faceY - 5, 3, TFT_WHITE);
    M5.Lcd.fillCircle(centerX + 10, faceY - 5, 3, TFT_WHITE);

    // Mouth based on mood
    switch(moodIndex) {
        case 2: // Happy
            M5.Lcd.drawCircle(centerX, faceY + 5, 8, TFT_WHITE);
            M5.Lcd.fillCircle(centerX, faceY + 5, 8, TFT_BLACK);
            M5.Lcd.drawCircle(centerX, faceY + 3, 8, TFT_WHITE);
            break;
        case 4: // Sad
            M5.Lcd.drawCircle(centerX, faceY + 10, 8, TFT_WHITE);
            M5.Lcd.fillCircle(centerX, faceY + 10, 8, TFT_BLACK);
            M5.Lcd.drawCircle(centerX, faceY + 13, 8, TFT_WHITE);
            break;
        default: // Neutral
            M5.Lcd.drawLine(centerX - 8, faceY + 8, centerX + 8, faceY + 8, TFT_WHITE);
            break;
    }
}