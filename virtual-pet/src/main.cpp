#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"
#include "../lib/Display/display_manager.h"

// Global instances
Pet myPet;
DisplayManager display;

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

  // Example pet interactions
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {  // Every 5 seconds
    // Simulate pet getting hungry over time
    myPet.setHungry(myPet.getHungry() + 5);

    // Update display with current pet status
    display.showPetStatus(myPet.getHappy(), myPet.getHungry(), myPet.getEnergised());
    display.showPetMood(myPet.getDominantMood());

    lastUpdate = millis();
  }

  // Example: Feed pet when button A is pressed
  if (M5.BtnA.wasPressed()) {
    myPet.feed();
    display.showActionFeedback("Fed the pet!");
    delay(1000);  // Show feedback briefly
  }

  // Example: Play with pet when button B is pressed
  if (M5.BtnB.wasPressed()) {
    myPet.play();
    display.showActionFeedback("Played with pet!");
    delay(1000);  // Show feedback briefly
  }
}
  // }

