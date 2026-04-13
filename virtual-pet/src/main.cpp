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

  // Initialize display
  display.init();

  // Show initialization message
  display.showMessage("Virtual Pet initialized!");

  // Small delay to show message
  delay(2000);
}

void loop() {
  M5.update();  // Update M5Stick C Plus2 state
  buttons.update();  // Update button states

  // Handle menu navigation (buttons A and B cycle through actions)
  menu.update(buttons);

  // Unified display render - renders pet status and menu indicator at same refresh rate
  // This eliminates flickering caused by separate render calls
  display.renderDisplay(myPet.getHappy(), myPet.getHungry(), myPet.getEnergised(), myPet.getDominantMood(), menu);
  
  // Run all automatic stat changes (hunger increase, happiness decay, etc.).
  // The rules for what changes and how fast live in TimerManager, not here.
  timers.update(myPet);

  // Confirm action with Button A
  if (buttons.wasButtonAPressed()) {
    menu.confirmAction(myPet, display);
  }
}
  // }

