#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <M5StickCPlus2.h>
#include "screen_layout.h"
#include "sprites/newpiskel2.h"   // 32x32 sprite — used on the Stats screen
#include "sprites/48x48_test.h"   // 48x48 sprite — used on the Interact screen
#include "sprites/64x64_test.h"   // 64x64 sprite — used on the Main screen
#include "sprites/80x80_test.h"   // 80x80 sprite — the single shipped size (Task 14d)

// DisplayManager only deals in primitive values (ints, strings, enums).
// It never receives whole manager objects, so it does not need to know
// what an ActionMenu or a Pet is. Whoever calls renderDisplay is
// responsible for pulling the values out and passing them in.

class DisplayManager {
private:
    // Display dimensions for M5Stick C Plus 2
    static const int SCREEN_WIDTH  = 135;
    static const int SCREEN_HEIGHT = 240;

    // -----------------------------------------------------------------------
    // Stats screen zone constants — these match the original layout exactly.
    // Stats bars sit above the face so the user can see all five values.
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
    // Main screen zone constants — no stat bars, so the face gets more room.
    // The bottom nav bar lets the user pick Stats or Interact.
    // -----------------------------------------------------------------------
    static constexpr int         MAIN_FACE_CENTER_Y = 110;
    static constexpr int         MAIN_MOOD_Y        = 155;
    // Nav bar shares the same Y/height as MENU_ZONE so the bottom bar does not
    // appear to move when switching between the Main, Stats, and Interact screens.
    static constexpr ScreenZone  MAIN_NAV_ZONE      = {  5, 220, 125, 20 };

    // -----------------------------------------------------------------------
    // Interact screen zone constants. The face and mood text are positioned to
    // match the Main screen exactly (MAIN_FACE_CENTER_Y / MAIN_MOOD_Y) so the
    // pet does not appear to jump when switching screens. The contextual stat
    // bar and the action menu indicator then fill the region below the mood.
    // -----------------------------------------------------------------------
    static constexpr int         INTERACT_FACE_CENTER_Y = 110;  // = MAIN_FACE_CENTER_Y
    static constexpr int         INTERACT_MOOD_Y        = 155;  // = MAIN_MOOD_Y
    static constexpr int         INTERACT_STAT_LABEL_Y  = 177;
    static constexpr int         INTERACT_STAT_BAR_Y    = 190;
    static constexpr ScreenZone  INTERACT_STAT_ZONE     = {  5, 174, 125, 30 };

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
    // The Interact screen needs three pieces of information about the action menu
    // (the action name to display, which stat bar to highlight, and the current
    // index so it can detect when the user has scrolled). We pass these as
    // primitives so DisplayManager does not need to know what an ActionMenu is.
    void renderInteractScreen(int happiness, int hunger, int energy, int cleanliness, int sick,
                              int moodIndex, const char* selectedActionName,
                              RelevantStat relevantStat, int currentActionIndex,
                              const char* petName);

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
    // Default colour is TFT_BLACK — callers can override to clear with any colour.
    // Using a default parameter (familiar from JavaScript) keeps both call styles
    // working without needing two separate methods.
    void clearScreen(uint32_t color = TFT_BLACK);
    void fillRect(int x, int y, int width, int height, uint32_t color);

    // Text display
    void printText(const char* text, int x, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);
    void printCenteredText(const char* text, int y, uint32_t color = TFT_WHITE, uint8_t size = 2);

    // renderDisplay() — the single call that loop() makes every frame.
    // screenState comes from NavigationManager.
    // petIsDead bypasses the normal screen routing and shows the death screen.
    // selectedActionName, relevantStat, and currentActionIndex are extracted
    // from ActionMenu by the caller — keeping DisplayManager unaware of it.
    void renderDisplay(int happiness, int hunger, int energy, int cleanliness, int sick,
                       int moodIndex, const char* selectedActionName,
                       RelevantStat relevantStat, int currentActionIndex,
                       bool petIsDead, const char* petName, ScreenState screenState);

    // Pet display helpers — used internally and by the three private render methods
    void showPetStatus(int happiness, int hunger, int energy, int cleanliness, int sick, const char* petName);
    void showPetMood(int moodIndex);
    void showMessage(const char* message);
    void showActionFeedback(const char* action);
    void showDeathScreen();

    // drawMenuIndicator() — draws the compact action name overlay at the bottom.
    // Used on the Interact screen. Takes the action name as a plain string so
    // this helper has no knowledge of ActionMenu's internals.
    void drawMenuIndicator(const char* selectedActionName, int x, int y);

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
