#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"
#include "../lib/Button/button_handler.h"
#include "../lib/Actions/action_menu.h"
#include "../lib/Timer/time_manager.h"

// Global instances — one object per system area.
// Each manager is responsible for exactly one job.
Pet myPet;               // Holds all of the pet's stats and care actions.
DisplayManager display;  // Draws everything to the screen.
ButtonHandler buttons;   // Reads and tracks button presses.
ActionMenu menu;         // Manages the list of actions the player can choose.
TimerManager timers;     // Handles all automatic stat changes over time.

void setup() {
  // Initialize M5Stick C Plus2
  M5.begin();
  // Serial.begin(115200);   // Start serial for debugging (optional)

  // Initialize display
  display.init();

  // Show initialization message
  display.showMessage("Virtual Pet initialized!");

  // Small delay to show message
  delay(2000);
}

void loop() {
  M5.update();      // Read the latest hardware state (buttons, IMU, etc.)
  buttons.update(); // Detect which buttons were pressed this frame

  // Render the display first — passing isDead() lets DisplayManager decide what to show.
  // This must run every frame regardless of pet state so the screen is never skipped.
  display.renderDisplay(myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(), myPet.getDominantMood(), menu, myPet.isDead());

  // If the pet is dead, only check for the restart button then exit early.
  // The early return skips timers and menu navigation while the pet is in the dead state.
  if (myPet.isDead()) {
    if (buttons.wasButtonAPressed()) {
      myPet.reset();
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

  // Confirm action with Button A
  if (buttons.wasButtonAPressed()) {
    menu.confirmAction(myPet, display);
  }
}

