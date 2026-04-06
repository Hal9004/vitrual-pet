#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "../lib/Pet/pet.h"

// Global pet instance
Pet myPet;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // Initialize serial for debugging
  
  // Initialize pet (already done in constructor)
  Serial.println("Virtual Pet initialized!");
  
  // Example: Check initial pet state
  Serial.print("Pet happiness: ");
  Serial.println(myPet.getHappy());
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Example pet interactions
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {  // Every 5 seconds
    // Simulate pet getting hungry over time
    myPet.setHungry(myPet.getHungry() + 5);
    
    // Print current dominant mood
    int mood = myPet.getDominantMood();
    Serial.print("Pet mood: ");
    switch(mood) {
      case 0: Serial.println("Hungry"); break;
      case 1: Serial.println("Tired"); break;
      case 2: Serial.println("Happy"); break;
      case 3: Serial.println("Sick"); break;
      case 4: Serial.println("Sad"); break;
      case 5: Serial.println("Clean"); break;
      case 6: Serial.println("Energised"); break;
    }
    
    lastUpdate = millis();
  }
  
  // Example: Feed pet when button A is pressed (you'll need to add button handling)
  // if (M5.BtnA.wasPressed()) {
  //   myPet.feed();
  //   Serial.println("Fed the pet!");
  // }
}
