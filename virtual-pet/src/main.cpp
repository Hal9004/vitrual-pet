#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"
#include "../lib/Button/button_handler.h"
#include "../lib/Actions/action_menu.h"
#include "../lib/Timer/time_manager.h"
#include "../lib/Imu/imu_manager.h"
#include "../lib/Speaker/speaker_manager.h"
#include "../lib/Storage/storage_manager.h"

// Global instances — one object per system area.
// Each manager is responsible for exactly one job.
Pet myPet;                // Holds all of the pet's stats and care actions.
DisplayManager display;   // Draws everything to the screen.
ButtonHandler buttons;    // Reads and tracks button presses.
ActionMenu menu;          // Manages the list of actions the player can choose.
TimerManager timers;      // Handles all automatic stat changes over time.
ImuManager imu;           // Reads accelerometer data and detects shake gestures.
SpeakerManager speaker;   // Plays buzzer melodies for pet events and alerts.
StorageManager storage;   // Saves and loads pet stats to NVS flash storage.

void setup() {
  // Initialize M5Stick C Plus2
  M5.begin();
  #ifdef DEBUG
  Serial.begin(115200);   // Start serial for debugging (optional)
  #endif
  // Initialize display
  display.init();

  // Initialize speaker — sets volume so buzzer melodies play at a comfortable level.
  speaker.init();

  // Restore the pet's stats from NVS flash storage.
  // If no save data exists yet (first boot), load() falls back to healthy default values.
  storage.load(myPet);

  // Show initialization message
  display.showMessage("Virtual Pet initialized!");

  // Small delay to show message
  delay(2000);
}

void loop() {
  M5.update();      // Read the latest hardware state (buttons, IMU, etc.)
  buttons.update(); // Detect which buttons were pressed this frame
  imu.update();     // Read fresh accelerometer data and update shake detection

  // Render the display first — passing the current state lets DisplayManager decide what to show.
  // This must run every frame regardless of pet state so the screen is never skipped.
  display.renderDisplay(myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(), myPet.getCleanliness(), myPet.getSick(), myPet.getDominantMood(), menu, myPet.getState() == STATE_DEAD, myPet.getPetName());

  // Run the state machine before anything else — it sets STATE_DEAD when stats are fatal,
  // so the dead check below always sees an up-to-date state.
  myPet.updateState();

  // If the pet is dead, only check for the restart button then exit early.
  // The early return skips timers and menu navigation while the pet is in the dead state.
  if (myPet.getState() == STATE_DEAD) {
    // The state machine sets deathSoundReady on the first frame of death.
    // checkDeathAlert() returns true once and then resets the flag — same pattern as the alerts.
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
    return;
  }

  // Handle menu navigation (buttons B and C cycle through actions)
  menu.update(buttons);

  // Run all automatic stat changes (hunger increase, happiness decay, energy drain).
  // The rules for what changes and how fast live in TimerManager, not here.
  timers.update(myPet);

  // The state machine sets alert flags when hunger or sickness cross their thresholds.
  // checkHungerAlert() and checkSicknessAlert() return true once per alert event, then reset.
  // Keeping the speaker call here means Pet does not need to know about SpeakerManager.
  if (myPet.checkHungerAlert()) {
    speaker.playHungerAlertSound();
  }

  if (myPet.checkSicknessAlert()) {
    speaker.playSicknessAlertSound();
  }

  // Confirm action with Button A — executes the selected menu action, plays its sound,
  // and shows a brief feedback message on screen.
  if (buttons.wasButtonAPressed()) {
    menu.confirmAction(myPet, display, speaker, storage);
  }

  // If the device was shaken, trigger the play action directly.
  // wasShaken() fires only on the first frame of a shake so play() is called once per gesture.
  if (imu.wasShaken()) {
    myPet.play();
  }
}

