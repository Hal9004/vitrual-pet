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

void setup() {
    M5.begin();

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
    M5.update();      // Read the latest hardware state (buttons, IMU, etc.)
    buttons.update(); // Detect which buttons were pressed this frame
    imu.update();     // Read fresh accelerometer data and update shake detection

    // Run the state machine — sets STATE_DEAD when hunger or energy hit a fatal level.
    // This must run before the dead check below so the dead state is always up to date.
    myPet.updateState();

    bool petIsDead = (myPet.getState() == STATE_DEAD);

    if (petIsDead) {
        // Play the death sound once on the first frame of death.
        if (myPet.checkDeathAlert()) {
            speaker.playDeathSound();
        }

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

        // The state machine sets alert flags when hunger or sickness cross their thresholds.
        // checkHungerAlert() and checkSicknessAlert() return true once per alert event, then reset.
        if (myPet.checkHungerAlert()) {
            speaker.playHungerAlertSound();
        }
        if (myPet.checkSicknessAlert()) {
            speaker.playSicknessAlertSound();
        }

        // Only cycle through menu actions when the Interact screen is visible.
        // Calling menu.update() on other screens would silently change the selected
        // action while the player cannot even see the menu.
        if (navManager.getCurrentScreen() == SCREEN_INTERACT) {
            menu.update(buttons);
        }

        // Update navigation — reads button input and switches screens if needed.
        // This must run AFTER menu.update() so NavigationManager can read the
        // latest selected action when deciding whether A means "Back" or "Confirm".
        navManager.update(buttons, menu);

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
    display.renderDisplay(
        myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(),
        myPet.getCleanliness(), myPet.getSick(), myPet.getDominantMood(),
        menu, petIsDead, myPet.getPetName(),
        navManager.getCurrentScreen(), navManager.getMainNavIndex()
    );
}
