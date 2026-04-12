#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <M5StickCPlus2.h>

// Forward declaration
class ActionMenu;

// Display state enum for coordinating what should be rendered
enum class DisplayState {
    STATUS_VIEW,      // Showing pet status as primary view
    MENU_INDICATOR    // Showing menu indicator overlay
};

class DisplayManager {
private:
    // Display dimensions for M5Stick C Plus2
    static const int SCREEN_WIDTH = 135;
    static const int SCREEN_HEIGHT = 240;
    
    // Display state management
    DisplayState currentState;
    unsigned long lastStatusUpdate;
    static const unsigned long STATUS_UPDATE_INTERVAL = 5000;  // 5 seconds
    
    // Current pet stats for comparison
    int lastHappiness;
    int lastHunger;
    int lastEnergy;
    int lastMoodIndex;

public:
    // Constructor
    DisplayManager();

    // Initialization
    void init();

    // Screen management
    void clearScreen();
    void clearScreen(uint32_t color);
    void fillRect(int x, int y, int width, int height, uint32_t color);

    // Text display functions
    void printText(const char* text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printText(String text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printCenteredText(const char* text, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);

    // Display state management
    void setDisplayState(DisplayState newState);
    DisplayState getDisplayState() const;
    bool shouldUpdateStatusDisplay();
    
    // Coordinated render function
    void renderFrame(int happiness, int hunger, int energy, int moodIndex);

    // Unified display render - handles both pet status and menu indicator
    // Call this once per loop to render everything at the same refresh rate
    void renderDisplay(int happiness, int hunger, int energy, int moodIndex, const ActionMenu& menu);

    // Pet-specific display functions
    void showPetStatus(int happiness, int hunger, int energy);
    void showPetMood(int moodIndex);
    void showMessage(const char* message);
    void showActionFeedback(const char* action);

    // Menu indicator rendering
    void drawMenuIndicator(const ActionMenu& menu, int x, int y);

    // UI layout helpers
    void drawStatusBar(int value, int maxValue, int x, int y, int width, uint32_t color);
    void drawPetFace(int moodIndex);

};

#endif