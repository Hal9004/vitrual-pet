#ifndef IMU_MANAGER_H
#define IMU_MANAGER_H

#include <M5StickCPlus2.h>

/**
 * ImuManager
 *
 * Reads acceleration data from the MPU6886 via the M5 library and detects
 * shake gestures. Follows the same edge-detection pattern as ButtonHandler:
 * call update() once per loop, then query wasShaken() to get a one-frame pulse.
 *
 * Usage:
 *   ImuManager imu;
 *
 *   // In your loop:
 *   imu.update();
 *   if (imu.wasShaken()) {
 *       myPet.play();
 *   }
 */
class ImuManager {
private:
    // G-force magnitude above which the device is considered to be shaking.
    // Total magnitude = sqrt(x² + y² + z²). At rest this is ~1.0 G (gravity).
    // A gentle shake typically peaks at 2.0–3.0 G; 1.8 is a comfortable trigger point.
    static constexpr float SHAKE_THRESHOLD = 1.8f;

    // Raw acceleration components read from the MPU6886 (units: G-force)
    float accelX;
    float accelY;
    float accelZ;

    // Edge-detection state: mirrors the pattern used in ButtonHandler.
    // prevShakeDetected holds the shake state from the last frame so that
    // wasShaken() can fire only on the frame the shake starts, not every frame.
    bool prevShakeDetected;
    bool currentShakeDetected;

public:
    // Constructor — initialises all values to safe defaults before first update()
    ImuManager();

    // update()
    // Must be called once per loop iteration, after M5.update().
    // Reads fresh acceleration data and updates shake detection state.
    void update();

    // wasShaken()
    // Returns true only on the single frame when a shake is first detected —
    // magnitude exceeded SHAKE_THRESHOLD this frame but not the previous frame.
    bool wasShaken() const;

    // Getters so other modules can read raw acceleration if needed
    float getAccelX() const;
    float getAccelY() const;
    float getAccelZ() const;
};

#endif
