#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <M5StickCPlus2.h>
#include "screen_layout.h"
#include "sprites/newpiskel2.h"   // 32x32 sprite — used on the Stats screen
#include "sprites/48x48_test.h"   // 48x48 sprite — used on the Interact screen
#include "sprites/64x64_test.h"   // 64x64 sprite — used on the Main screen

// Forward declaration — lets us reference ActionMenu in function signatures
// without including action_menu.h here. The full include lives in the .cpp file.
// This avoids a circular include: action_menu.h already includes display_manager.h.
class ActionMenu;

class DisplayManager {
private:
    // Display dimensions for M5Stick C Plus 2
    static const int SCREEN_WIDTH  = 135;
    static const int SCREEN_HEIGHT = 240;

    // -----------------------------------------------------------------------
    // SCREEN_STATS zone constants — these match the original layout exactly.
    // Stats bars sit above the face so the player can see all five values.
    // -----------------------------------------------------------------------
    static constexpr ScreenZone TITLE_ZONE    = {  0,   5, 135, 19 };
    static constexpr ScreenZone STATS_ZONE    = {  5,  26, 125, 98 };
    static constexpr ScreenZone PET_FACE_ZONE = {  0, 134, 135, 36 };
    static constexpr ScreenZone MOOD_ZONE     = {  0, 180, 135, 18 };
    static constexpr ScreenZone MENU_ZONE     = {  5, 220, 125, 20 };

    // Per-stat bar positions within STATS_ZONE
    static constexpr StatBarZone HAPPY_BAR_ZONE  = {  26,  36 };
    static constexpr StatBarZone HUNGER_BAR_ZONE = {  48,  58 };
    static constexpr StatBarZone ENERGY_BAR_ZONE = {  70,  80 };
    static constexpr StatBarZone CLEAN_BAR_ZONE  = {  92, 102 };
    static constexpr StatBarZone SICK_BAR_ZONE   = { 114, 124 };

    // -----------------------------------------------------------------------
    // SCREEN_MAIN zone constants — no stat bars, so the face gets more room.
    // The bottom nav bar lets the player pick Stats or Interact.
    // -----------------------------------------------------------------------
    static constexpr int         MAIN_FACE_CENTER_Y = 110;
    static constexpr int         MAIN_MOOD_Y        = 155;
    static constexpr ScreenZone  MAIN_NAV_ZONE      = {  5, 213, 125, 22 };

    // -----------------------------------------------------------------------
    // SCREEN_INTERACT zone constants — face sits at the top, contextual stat
    // bar in the middle, and the action menu indicator at the very bottom.
    // -----------------------------------------------------------------------
    static constexpr int         INTERACT_FACE_CENTER_Y = 90;
    static constexpr int         INTERACT_MOOD_Y        = 128;
    static constexpr int         INTERACT_STAT_LABEL_Y  = 156;
    static constexpr int         INTERACT_STAT_BAR_Y    = 168;
    static constexpr ScreenZone  INTERACT_STAT_ZONE     = {  5, 153, 125, 28 };

    // Geometry constants shared across all screens
    static const int STAT_BAR_HEIGHT = 10;

    // Redraw throttle — only redraw the full screen every 5 seconds to avoid
    // LCD flicker. Fast-changing elements (menu selection, contextual stat bar)
    // are redrawn immediately in their own update paths.
    static const unsigned long STATUS_UPDATE_INTERVAL = 5000;

    // State tracked between frames to detect when something changed
    ScreenState  lastRenderedScreen;   // Detects screen transitions — forces a full redraw
    unsigned long lastFullRedrawTime;  // Timestamp of the last full redraw
    int  lastMenuActionIndex;  // Detects action selection changes on Interact screen
    bool petWasDeadLastFrame;  // Used to force a redraw immediately after revival

    // -----------------------------------------------------------------------
    // Private render methods — one per screen.
    // Each owns the layout and redraw logic for its screen only.
    // -----------------------------------------------------------------------
    void renderMainScreen(int moodIndex, const char* petName);
    void renderStatsScreen(int happiness, int hunger, int energy, int cleanliness, int sick, int moodIndex, const char* petName);
    void renderInteractScreen(int happiness, int hunger, int energy, int cleanliness, int sick, int moodIndex, const ActionMenu& menu, const char* petName);

    // Draws the two-tab nav bar at the bottom of the Main screen.
    // The highlighted tab (mainNavIndex) gets a filled background.
    void drawMainNavBar();

    // Draws a single stat bar for the action currently selected on the Interact screen.
    // The bar label, value, and colour are determined by relevantStat.
    // Draws nothing if relevantStat is STAT_NONE (Save / Back actions).
    void drawContextualStatBar(int happiness, int hunger, int energy, int cleanliness, int sick, RelevantStat relevantStat);

    // Draws just the mood label text at the given Y position.
    // Separated from drawPetSprite() so each screen can position the text independently.
    void showPetMoodText(int moodIndex, int textY);

public:
    DisplayManager();

    void init();

    // Screen management
    void clearScreen();
    void clearScreen(uint32_t color);
    void fillRect(int x, int y, int width, int height, uint32_t color);

    // Text display
    void printText(const char* text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printText(String text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printCenteredText(const char* text, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);

    // renderDisplay() — the single call that loop() makes every frame.
    // screenState and mainNavIndex come from NavigationManager.
    // petIsDead bypasses the normal screen routing and shows the death screen.
    void renderDisplay(int happiness, int hunger, int energy, int cleanliness, int sick,
                       int moodIndex, const ActionMenu& menu, bool petIsDead,
                       const char* petName, ScreenState screenState);

    // Pet display helpers — used internally and by the three private render methods
    void showPetStatus(int happiness, int hunger, int energy, int cleanliness, int sick, const char* petName);
    void showPetMood(int moodIndex);
    void showMessage(const char* message);
    void showActionFeedback(const char* action);
    void showDeathScreen();

    // drawMenuIndicator() — draws the compact action name overlay at the bottom.
    // Used on the Interact screen.
    void drawMenuIndicator(const ActionMenu& menu, int x, int y);

    // drawStatusBar() — draws a single labelled progress bar.
    void drawStatusBar(int value, int maxValue, int x, int y, int width, uint32_t color);

    // drawPetSprite() — draws a bitmap sprite as the pet's face. The sprite is
    // centred horizontally on the screen, with its vertical centre at faceCenterY.
    // Each screen passes its own sprite dimensions and pixel data, so this single
    // function works for the small Stats-screen sprite, the medium Interact-screen
    // sprite, and the large Main-screen sprite without needing a per-screen branch.
    // moodIndex is reserved for a future task that will pick a mood-specific sprite.
    void drawPetSprite(int moodIndex, int faceCenterY, int spriteWidth, int spriteHeight, const uint16_t* spriteData);
};

#endif
