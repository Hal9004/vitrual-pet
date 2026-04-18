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

// How many milliseconds between each energy drain.
// 8000 ms = 8 seconds.
const unsigned long ENERGY_DRAIN_INTERVAL = 8000;

// How many points energy drops each interval.
const int ENERGY_DRAIN_AMOUNT = 1;

// How many milliseconds between each cleanliness decrease.
// 10000 ms = 10 seconds.
const unsigned long CLEANLINESS_DECAY_INTERVAL = 10000;

// How many points cleanliness drops each interval.
const int CLEANLINESS_DECAY_AMOUNT = 1;

// How many milliseconds between each sickness increase (when cleanliness is low).
// 12000 ms = 12 seconds.
const unsigned long SICKNESS_ACCUMULATION_INTERVAL = 12000;

// How many points sick rises each interval.
const int SICKNESS_ACCUMULATION_AMOUNT = 1;

// Cleanliness must fall below this value before sickness starts accumulating.
const int CLEANLINESS_DANGER_THRESHOLD = 30;


// Constructor — initialise all timestamps to 0.
// Setting them to 0 means the first check in update() will always find
// that "enough time has passed", so the first decay fires immediately.
TimerManager::TimerManager()
    : lastHungerIncreaseTime(0),
      lastHappinessDecayTime(0),
      lastEnergyDrainTime(0),
      lastCleanlinessDecayTime(0),
      lastSicknessAccumulationTime(0) {
}


// update() — the single function main.cpp calls every loop().
// It delegates each timed job to its own private helper method.
// To add a new automatic stat change, add a method and call it here.
void TimerManager::update(Pet& pet) {
    applyHungerIncrease(pet);
    applyHappinessDecay(pet);
    applyEnergyDrain(pet);
    applyCleanlinessDecay(pet);
    applySicknessAccumulation(pet);
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


// applyEnergyDrain()
// Checks whether ENERGY_DRAIN_INTERVAL milliseconds have passed since
// energy was last decreased. If yes, decreases energy and resets the timer.
void TimerManager::applyEnergyDrain(Pet& pet) {
    unsigned long currentTime = millis();

    if (currentTime - lastEnergyDrainTime > ENERGY_DRAIN_INTERVAL) {
        pet.setEnergised(pet.getEnergised() - ENERGY_DRAIN_AMOUNT);
        lastEnergyDrainTime = currentTime;
    }
}


// applyCleanlinessDecay()
// Checks whether CLEANLINESS_DECAY_INTERVAL milliseconds have passed since
// cleanliness was last decreased. If yes, decreases cleanliness and resets the timer.
// The pet gets dirty over time — bathing is the only way to keep it clean.
void TimerManager::applyCleanlinessDecay(Pet& pet) {
    unsigned long currentTime = millis();

    if (currentTime - lastCleanlinessDecayTime > CLEANLINESS_DECAY_INTERVAL) {
        pet.setCleanliness(pet.getCleanliness() - CLEANLINESS_DECAY_AMOUNT);
        lastCleanlinessDecayTime = currentTime;
    }
}


// applySicknessAccumulation()
// Increases sick only when cleanliness has fallen below CLEANLINESS_DANGER_THRESHOLD.
// A dirty pet gradually becomes unwell — the player must bathe it to stop this.
void TimerManager::applySicknessAccumulation(Pet& pet) {
    unsigned long currentTime = millis();

    if (pet.getCleanliness() < CLEANLINESS_DANGER_THRESHOLD) {
        if (currentTime - lastSicknessAccumulationTime > SICKNESS_ACCUMULATION_INTERVAL) {
            pet.setSick(pet.getSick() + SICKNESS_ACCUMULATION_AMOUNT);
            lastSicknessAccumulationTime = currentTime;
        }
    }
}
