#include "display_manager.h"

// Constructor
DisplayManager::DisplayManager() {
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

// Show pet status information
void DisplayManager::showPetStatus(int happiness, int hunger, int energy) {
    clearScreen();

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

// Show current pet mood
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