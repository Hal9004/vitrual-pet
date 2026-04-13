#include <Arduino.h>
#include "time_manager.h"

// How many milliseconds between each hunger increase.
// 3000 ms = 3 seconds. Change this number to make hunger grow faster or slower.
const unsigned long HUNGER_INCREASE_INTERVAL = 3000;

// How many points hunger goes up each interval.
const int HUNGER_INCREASE_AMOUNT = 2;

// How many milliseconds between each happiness decrease.
// 5000 ms = 5 seconds.
const unsigned long HAPPINESS_DECAY_INTERVAL = 5000;

// How many points happiness drops each interval.
const int HAPPINESS_DECAY_AMOUNT = 1;


// Constructor — initialise all timestamps to 0.
// Setting them to 0 means the first check in update() will always find
// that "enough time has passed", so the first decay fires immediately.
TimerManager::TimerManager()
    : lastHungerIncreaseTime(0),
      lastHappinessDecayTime(0) {
}


// update() — the single function main.cpp calls every loop().
// It delegates each timed job to its own private helper method.
// To add a new automatic stat change, add a method and call it here.
void TimerManager::update(Pet& pet) {
    applyHungerIncrease(pet);
    applyHappinessDecay(pet);
}


// applyHungerIncrease()
// Checks whether HUNGER_INCREASE_INTERVAL milliseconds have passed since
// hunger was last increased. If yes, increases hunger and resets the timer.
void TimerManager::applyHungerIncrease(Pet& pet) {
    unsigned long currentTime = millis();

    if (currentTime - lastHungerIncreaseTime > HUNGER_INCREASE_INTERVAL) {
        pet.setHungry(pet.getHungry() + HUNGER_INCREASE_AMOUNT);
        lastHungerIncreaseTime = currentTime;
    }
}


// applyHappinessDecay()
// Checks whether HAPPINESS_DECAY_INTERVAL milliseconds have passed since
// happiness was last decreased. If yes, decreases happiness and resets the timer.
void TimerManager::applyHappinessDecay(Pet& pet) {
    unsigned long currentTime = millis();

    if (currentTime - lastHappinessDecayTime > HAPPINESS_DECAY_INTERVAL) {
        pet.setHappy(pet.getHappy() - HAPPINESS_DECAY_AMOUNT);
        lastHappinessDecayTime = currentTime;
    }
}
