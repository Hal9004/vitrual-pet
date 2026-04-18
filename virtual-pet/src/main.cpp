#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"
#include "../lib/Button/button_handler.h"
#include "../lib/Actions/action_menu.h"
#include "../lib/Timer/time_manager.h"
#include "../lib/Imu/imu_manager.h"
#include "../lib/Speaker/speaker_manager.h"

// Global instances — one object per system area.
// Each manager is responsible for exactly one job.
Pet myPet;               // Holds all of the pet's stats and care actions.
DisplayManager display;  // Draws everything to the screen.
ButtonHandler buttons;   // Reads and tracks button presses.
ActionMenu menu;         // Manages the list of actions the player can choose.
TimerManager timers;     // Handles all automatic stat changes over time.
ImuManager imu;          // Reads accelerometer data and detects shake gestures.
SpeakerManager speaker;  // Plays buzzer melodies for pet events and alerts.

void setup() {
  // Initialize M5Stick C Plus2
  M5.begin();
  Serial.begin(115200);   // Start serial for debugging (optional)

  // Initialize display
  display.init();

  // Initialize speaker — sets volume so buzzer melodies play at a comfortable level.
  speaker.init();

  // Show initialization message
  display.showMessage("Virtual Pet initialized!");

  // Small delay to show message
  delay(2000);
}

void loop() {
  M5.update();      // Read the latest hardware state (buttons, IMU, etc.)
  buttons.update(); // Detect which buttons were pressed this frame
  imu.update();     // Read fresh accelerometer data and update shake detection

  // Render the display first — passing isDead() lets DisplayManager decide what to show.
  // This must run every frame regardless of pet state so the screen is never skipped.
  display.renderDisplay(myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(), myPet.getCleanliness(), myPet.getSick(), myPet.getDominantMood(), menu, myPet.isDead());

  // If the pet is dead, only check for the restart button then exit early.
  // The early return skips timers and menu navigation while the pet is in the dead state.
  if (myPet.isDead()) {
    // Play the death melody exactly once — the static flag prevents it from replaying every frame.
    static bool deathSoundPlayed = false;
    if (deathSoundPlayed == false) {
      speaker.playDeathSound();
      deathSoundPlayed = true;
    }

    // Button A restarts the game. Play the reset fanfare so the player knows a new life has begun.
    if (buttons.wasButtonAPressed()) {
      myPet.reset();
      speaker.playResetSound();
      deathSoundPlayed = false;  // Clear the flag so the melody can play again on the next death.
    }
    return;
  }

  // Handle menu navigation (buttons B and C cycle through actions)
  menu.update(buttons);

  // Run all automatic stat changes (hunger increase, happiness decay, energy drain).
  // The rules for what changes and how fast live in TimerManager, not here.
  timers.update(myPet);

  // Run the state machine — checks the current state and handles any behaviour tied to it.
  myPet.updateState();

  // Play a hunger alert when the pet is dangerously hungry.
  // The millis() timer ensures the two-beep warning sounds at most once every 15 seconds
  // rather than on every frame — the same non-blocking pattern used in TimerManager.
  static unsigned long lastHungerAlertTime = 0;
  unsigned long hungerAlertInterval = 15000;
  int hungerAlertThreshold = 80;

  if (myPet.getHungry() >= hungerAlertThreshold) {
    if (millis() - lastHungerAlertTime >= hungerAlertInterval) {
      speaker.playHungerAlertSound();
      lastHungerAlertTime = millis();
    }
  }

  // Play a sickness alert when the pet is dangerously sick.
  // Uses the same millis() debounce pattern as the hunger alert above.
  // The lower-pitched beep sounds distinct from the hunger alert so the player
  // can tell them apart without checking the screen.
  static unsigned long lastSicknessAlertTime = 0;
  unsigned long sicknessAlertInterval = 15000;
  int sicknessAlertThreshold = 80;

  if (myPet.getSick() >= sicknessAlertThreshold) {
    if (millis() - lastSicknessAlertTime >= sicknessAlertInterval) {
      speaker.playSicknessAlertSound();
      lastSicknessAlertTime = millis();
    }
  }

  // Confirm action with Button A — executes the selected menu action, plays its sound,
  // and shows a brief feedback message on screen.
  if (buttons.wasButtonAPressed()) {
    menu.confirmAction(myPet, display, speaker);
  }

  // If the device was shaken, trigger the play action directly.
  // wasShaken() fires only on the first frame of a shake so play() is called once per gesture.
  if (imu.wasShaken()) {
    myPet.play();
  }
}

