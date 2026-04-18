#include <Arduino.h>
#include "pet.h"

// Constructor - Initialize pet with neutral values
Pet::Pet()
    : hungry(30), tired(20), happy(70), sick(0), sad(10),
      cleanliness(60), energised(80), currentState(STATE_IDLE) {
}

// Getters
int Pet::getHungry() const { return hungry; }
int Pet::getTired() const { return tired; }
int Pet::getHappy() const { return happy; }
int Pet::getSick() const { return sick; }
int Pet::getSad() const { return sad; }
int Pet::getCleanliness() const { return cleanliness; }
int Pet::getEnergised() const { return energised; }

// Setters
void Pet::setHungry(int value) { hungry = value; constrainValues(); }
void Pet::setTired(int value) { tired = value; constrainValues(); }
void Pet::setHappy(int value) { happy = value; constrainValues(); }
void Pet::setSick(int value) { sick = value; constrainValues(); }
void Pet::setSad(int value) { sad = value; constrainValues(); }
void Pet::setCleanliness(int value) { cleanliness = value; constrainValues(); }
void Pet::setEnergised(int value) { energised = value; constrainValues(); }

// Pet care actions
void Pet::feed() {
    setState(STATE_EATING);    // Signal that the pet is now eating
    hungry = hungry - 30;      // Reduce hunger
    happy = happy + 10;        // Slightly increase happiness
    energised = energised - 5; // Small energy cost
    constrainValues();
}

void Pet::sleep() {
    setState(STATE_SLEEPING);  // Signal that the pet is now sleeping
    tired = tired - 40;        // Reduce tiredness significantly
    energised = energised + 30; // Restore energy
    hungry = hungry + 10;      // Wake up hungry
    constrainValues();
}

void Pet::play() {
    setState(STATE_PLAYING);        // Signal that the pet is now playing
    happy = happy + 25;             // Significantly increase happiness
    tired = tired + 20;             // Increase tiredness
    energised = energised - 20;     // Use energy
    hungry = hungry + 15;           // Playing makes them hungry
    constrainValues();
    Serial.println("DEBUG: play() triggered — happy=" + String(happy) + " energised=" + String(energised));
}

void Pet::bathe() {
    setState(STATE_BATHING);            // Signal that the pet is now being cleaned
    cleanliness = cleanliness + 30;     // Improve cleanliness
    tired = tired + 10;                 // Bathing is tiring
    energised = energised - 10;         // Uses some energy
    constrainValues();
}

void Pet::heal() {
    setState(STATE_HEALING);    // Signal that the pet is now receiving treatment
    sick = sick - 50;           // Reduce sickness
    tired = tired + 20;         // Medicine makes pet tired
    happy = happy - 5;          // Unpleasant experience
    constrainValues();
}

// getState()
// Returns the behaviour the pet is currently in so other systems can react to it.
PetState Pet::getState() const {
    return currentState;
}

// setState()
// Changes the pet's current behaviour. All state transitions go through here
// so there is always one place to look when debugging unexpected state changes.
void Pet::setState(PetState newState) {
    currentState = newState;
}

// updateState()
// Runs once per loop. Checks the current state and applies any
// behaviour that belongs to it. Add new states here as the game grows.
void Pet::updateState() {
    switch (currentState) {
        case STATE_IDLE:
            // If the sick stat is dangerously high, transition to sick automatically
            if (sick >= 50) {
                setState(STATE_SICK);
            }
            break;

        case STATE_EATING:
            // Eating is handled instantly by feed() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_SLEEPING:
            // Sleeping is handled instantly by sleep() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_PLAYING:
            // Playing is handled instantly by play() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_SICK:
            // Pet stays sick until heal() is called — no automatic return to idle
            break;

        case STATE_HEALING:
            // Healing is handled instantly by heal() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_BATHING:
            // Bathing is handled instantly by bathe() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_EVOLVING:
            // Placeholder — evolution logic added in task 9
            setState(STATE_IDLE);
            break;
    }
}

// isDead()
// Checks whether any critical stat has reached a fatal level.
// Hunger at 100 means the pet has starved. Energy or happiness at 0 means
// the pet has given up. Any one of these ends the game.
bool Pet::isDead() const {
    if (hungry >= 100) {
        return true;
    }
    if (energised <= 0) {
        return true;
    }
    if (happy <= 0) {
        return true;
    }
    return false;
}

// reset()
// Restores all stats to the same starting values used in the constructor.
// Called when the player chooses to restart after the pet has died.
void Pet::reset() {
    hungry     = 30;
    tired      = 20;
    happy      = 70;
    sick       = 0;
    sad        = 10;
    cleanliness = 60;
    energised  = 80;
}

// Get dominant mood (returns which condition is highest)
int Pet::getDominantMood() const {
    int conditions[] = {hungry, tired, happy, sick, sad, cleanliness, energised};
    int maxValue = 0;
    int maxIndex = 0;
    
    for (int i = 0; i < 7; i++) {
        if (conditions[i] > maxValue) {
            maxValue = conditions[i];
            maxIndex = i;
        }
    }
    return maxIndex; // 0=hungry, 1=tired, 2=happy, 3=sick, 4=sad, 5=clean, 6=energised
}

// Constrain all values between 0-100
void Pet::constrainValues() {
    auto constrainStat = [](int& value) {
        if (value < 0) value = 0;
        if (value > 100) value = 100;
    };

    constrainStat(hungry);
    constrainStat(tired);
    constrainStat(happy);
    constrainStat(sick);
    constrainStat(sad);
    constrainStat(cleanliness);
    constrainStat(energised);
}
