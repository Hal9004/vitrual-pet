#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <M5StickCPlus2.h>

class DisplayManager {
private:
    // Display dimensions for M5Stick C Plus2
    static const int SCREEN_WIDTH = 135;
    static const int SCREEN_HEIGHT = 240;

public:
    // Constructor
    DisplayManager();

    // Initialization
    void init();

    // Screen management
    void clearScreen();
    void clearScreen(uint32_t color);

    // Text display functions
    void printText(const char* text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printText(String text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printCenteredText(const char* text, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);

    // Pet-specific display functions
    void showPetStatus(int happiness, int hunger, int energy);
    void showPetMood(int moodIndex);
    void showMessage(const char* message);
    void showActionFeedback(const char* action);

    // UI layout helpers
    void drawStatusBar(int value, int maxValue, int x, int y, int width, uint32_t color);
    void drawPetFace(int moodIndex);

};

#endif