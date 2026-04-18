#ifndef PET_H
#define PET_H

// PetState — the list of behaviours the pet can currently be doing.
// Only one state is active at a time.
enum PetState {
    STATE_IDLE,      // Default — pet is awake but doing nothing
    STATE_EATING,    // Triggered by feed() — pet is eating
    STATE_SLEEPING,  // Triggered by sleep() — pet is resting
    STATE_EVOLVING   // Reserved for future evolution logic (task 9)
};

class Pet {
private:
    // Pet condition values (0-100 scale)
    int hungry;       // 0 = not hungry, 100 = starving
    int tired;        // 0 = not tired, 100 = exhausted
    int happy;        // 0 = sad, 100 = very happy
    int sick;         // 0 = healthy, 100 = very sick
    int sad;          // 0 = neutral, 100 = very sad
    int cleanliness;  // 0 = dirty, 100 = very clean
    int energised;    // 0 = no energy, 100 = full energy

    PetState currentState;  // Which behaviour the pet is currently in

public:
    // Constructor
    Pet();

    // Getters for each condition
    int getHungry() const;
    int getTired() const;
    int getHappy() const;
    int getSick() const;
    int getSad() const;
    int getCleanliness() const;
    int getEnergised() const;

    // Setters for each condition
    void setHungry(int value);
    void setTired(int value);
    void setHappy(int value);
    void setSick(int value);
    void setSad(int value);
    void setCleanliness(int value);
    void setEnergised(int value);

    // Utility methods for pet care
    void feed();           // Decreases hunger, increases happy
    void sleep();          // Decreases tired, increases energised
    void play();           // Increases happy, increases tired, decreases energised
    void bathe();          // Increases cleanliness, decreases tired
    void heal();           // Decreases sick
    
    // Get the pet's dominant mood state
    int getDominantMood() const;  // Returns index of highest condition value

    // Returns true if any stat has reached a fatal level (hunger=100, energy=0, or happy=0)
    bool isDead() const;

    // Returns the current behavioural state of the pet
    PetState getState() const;

    // Changes the pet's current behavioural state
    void setState(PetState newState);

    // Runs once per loop — checks the current state and applies any behaviour that belongs to it
    void updateState();

    // Restores all stats to their starting values so the game can begin again
    void reset();

    // Clamp values between 0-100
    void constrainValues();

};

#endif
