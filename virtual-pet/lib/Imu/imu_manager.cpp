#include <Arduino.h>
#include "imu_manager.h"
#include <math.h>

// ImuManager()
// Sets all acceleration values and shake states to safe defaults so that
// the first call to wasShaken() never returns a false positive.
ImuManager::ImuManager()
    : accelX(0.0f),
      accelY(0.0f),
      accelZ(0.0f),
      prevShakeDetected(false),
      currentShakeDetected(false) {
}

// update()
// Reads the current X, Y, Z acceleration from the MPU6886 via the M5 library,
// then computes the total magnitude and compares it to SHAKE_THRESHOLD.
// Must be called once per loop, after M5.update(), so the data is always fresh.
void ImuManager::update() {
    prevShakeDetected = currentShakeDetected;

    M5.Imu.getAccel(&accelX, &accelY, &accelZ);

    float magnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

    currentShakeDetected = (magnitude > SHAKE_THRESHOLD);
}

// wasShaken()
// Returns true only on the single frame when a shake starts — the magnitude
// crossed the threshold this frame but was below it the previous frame.
// This prevents a sustained shake from triggering play() on every loop iteration.
bool ImuManager::wasShaken() const {
    return currentShakeDetected && !prevShakeDetected;
}

// getAccelX()
// Returns the most recent X-axis acceleration in G-force units.
float ImuManager::getAccelX() const {
    return accelX;
}

// getAccelY()
// Returns the most recent Y-axis acceleration in G-force units.
float ImuManager::getAccelY() const {
    return accelY;
}

// getAccelZ()
// Returns the most recent Z-axis acceleration in G-force units.
float ImuManager::getAccelZ() const {
    return accelZ;
}
