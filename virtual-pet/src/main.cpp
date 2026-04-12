#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"
#include "../lib/Button/button_handler.h"
#include "../lib/Actions/action_menu.h"

// Global instances
Pet myPet;
DisplayManager display;
ButtonHandler buttons;
ActionMenu menu;

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
  
  // Regularly update hunger simulation
  static unsigned long lastHungerUpdate = 0;
  if (millis() - lastHungerUpdate > 3000) {  // Every 3 seconds
    // Simulate pet getting hungry over time
    myPet.setHungry(myPet.getHungry() + 2);
    lastHungerUpdate = millis();
  }

  // Confirm action with Button A
  if (buttons.wasButtonAPressed()) {
    menu.confirmAction(myPet, display);
  }
}
  // }

