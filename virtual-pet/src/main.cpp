#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"
#include "../lib/Button/button_handler.h"
#include "../lib/Actions/action_menu.h"
#include "../lib/Navigation/navigation_manager.h"
#include "../lib/Timer/time_manager.h"
#include "../lib/Imu/imu_manager.h"
#include "../lib/Speaker/speaker_manager.h"
#include "../lib/Storage/storage_manager.h"

// -------------------------------------------------------------------------
// SPRITE_TEST — quick render test for Task 12.
// When this is defined, setup() draws one sprite to the screen and stops.
// The entire game loop is bypassed — nothing else runs.
// To return to normal operation, comment out or delete this #define.
// -------------------------------------------------------------------------
// #define SPRITE_TEST
#include "../lib/Display/sprites/newpiskel2.h"

// Global instances — one object per system area.
// Each manager is responsible for exactly one job.
Pet             myPet;    // Holds all of the pet's stats and care actions.
DisplayManager  display;  // Draws everything to the screen.
ButtonHandler   buttons;  // Reads and tracks button presses.
ActionMenu      menu;     // Manages the list of actions the player can choose.
NavigationManager navManager; // Tracks which screen is active and routes button input.
TimerManager    timers;   // Handles all automatic stat changes over time.
ImuManager      imu;      // Reads accelerometer data and detects shake gestures.
SpeakerManager  speaker;  // Plays buzzer melodies for pet events and alerts.
StorageManager  storage;  // Saves and loads pet stats to NVS flash storage.

// playPendingAlertSounds() — checks every pet alert flag once per frame and
// plays the matching speaker melody. Bundles the repeated check+play pattern
// so loop() reads as a single line instead of three near-identical blocks.
//
// This helper lives in main.cpp because alert coordination is exactly
// main.cpp's job — Pet does not know about SpeakerManager (it just sets
// flags), and SpeakerManager does not know about Pet (it just plays sounds).
// main.cpp is the file that owns both globals and is the right place to
// bridge them.
//
// Each Pet::checkXAlert() method follows a read-and-clear pattern: it
// returns true at most once per alert event and then resets the flag, so it
// is safe to call every frame without re-triggering a sound that has
// already played.
void playPendingAlertSounds() {
    if (myPet.checkHungerAlert()) {
        speaker.playHungerAlertSound();
    }
    if (myPet.checkSicknessAlert()) {
        speaker.playSicknessAlertSound();
    }
    if (myPet.checkDeathAlert()) {
        speaker.playDeathSound();
    }
}

void setup() {
    M5.begin();

    #ifdef SPRITE_TEST
    // Clear the screen to black so transparent pixels show the background colour.
    M5.Lcd.fillScreen(TFT_BLACK);

    // Calculate the top-left corner that centres the sprite on the screen.
    int spriteX = (135 - SPRITE_NEWPISKEL2_WIDTH)  / 2;
    int spriteY = (240 - SPRITE_NEWPISKEL2_HEIGHT) / 2;

    // Draw the sprite. The last argument is the transparent colour key —
    // any pixel matching 0xF81F (magenta) is skipped, letting black show through.
    M5.Lcd.pushImage(spriteX, spriteY,
                     SPRITE_NEWPISKEL2_WIDTH,
                     SPRITE_NEWPISKEL2_HEIGHT,
                     sprite_newpiskel2[0],
                     SPRITE_TRANSPARENT_COLOR);
    return; // Skip all normal initialisation.
    #endif

    #ifdef DEBUG
    Serial.begin(115200);
    #endif

    display.init();
    speaker.init();

    // Restore the pet's stats from NVS flash storage.
    // If no save data exists yet (first boot), load() falls back to healthy defaults.
    storage.load(myPet);

    display.showMessage("Virtual Pet initialized!");
    delay(2000);
}

void loop() {
    #ifdef SPRITE_TEST
    return; // Nothing to do — sprite is already drawn in setup().
    #endif

    M5.update();      // Read the latest hardware state (buttons, IMU, etc.)
    buttons.update(); // Detect which buttons were pressed this frame
    imu.update();     // Read fresh accelerometer data and update shake detection

    // Run the state machine — sets STATE_DEAD when hunger or energy hit a fatal
    // level, and raises the hunger/sickness/death alert flags when a stat
    // crosses its warning threshold.
    // This must run before the dead check below so the dead state is always up to date.
    myPet.updateState();

    // Play any alert sounds the state machine just raised this frame.
    // Doing this once, in one place, replaces three separate check/play blocks
    // that used to live inline in the dead and alive branches below.
    playPendingAlertSounds();

    bool petIsDead = (myPet.getState() == STATE_DEAD);

    if (petIsDead) {
        // Button A restarts the game. Clear saved data so the dead state is not
        // reloaded on the next boot, then play the reset fanfare.
        if (buttons.wasButtonAPressed()) {
            myPet.reset();
            storage.clear();
            speaker.playResetSound();
        }
    } else {
        // Run all automatic stat changes (hunger increase, happiness decay, energy drain).
        // The rules for what changes and how fast live in TimerManager, not here.
        timers.update(myPet);

        // Only cycle through menu actions when the Interact screen is visible.
        // Calling menu.update() on other screens would silently change the selected
        // action while the player cannot even see the menu.
        if (navManager.getCurrentScreen() == SCREEN_INTERACT) {
            menu.update(buttons);
        }

        // Update navigation — reads button input and switches screens if needed.
        // This must run AFTER menu.update() so the latest "is Back highlighted?"
        // value is what NavigationManager sees. We pass that single fact as a
        // bool rather than handing over the whole menu object — NavigationManager
        // does not need to know what an ActionMenu is.
        navManager.update(buttons, menu.isBackSelected());

        // shouldConfirmAction() is true for exactly one frame when the player pressed A
        // on a non-Back action. Calling confirmAction() here keeps the pet/speaker/storage
        // logic out of NavigationManager, which only handles screen transitions.
        if (navManager.shouldConfirmAction()) {
            menu.confirmAction(myPet, display, speaker, storage);
        }

        // A shake gesture triggers play from any screen.
        // wasShaken() fires only on the first frame of a gesture so play() is called once.
        if (imu.wasShaken()) {
            myPet.play();
        }
    }

    // Render at the end of every frame with the fully updated state.
    // NavigationManager tells DisplayManager which screen to draw.
    //
    // We extract the three pieces of action-menu information DisplayManager
    // actually uses (name, relevant stat, current index) and pass them as
    // primitives. DisplayManager has no idea what an ActionMenu is — this
    // keeps the dependency between the two modules one-directional.
    Action selectedAction = menu.getSelectedAction();
    display.renderDisplay(
        myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(),
        myPet.getCleanliness(), myPet.getSick(), myPet.getDominantMood(),
        selectedAction.name,
        selectedAction.relevantStat,
        menu.getCurrentActionIndex(),
        petIsDead, myPet.getPetName(),
        navManager.getCurrentScreen()
    );
}
