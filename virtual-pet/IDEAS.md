# Virtual Pet — Ideas & Future Directions

This file captures ideas that are worth revisiting once the core Tamagotchi is fully working.
None of these are planned tasks — they are starting points for exploration.

---

## IMU-Driven Pet Movement (Play Mechanic Upgrade)

**The idea:**
Replace (or supplement) the current shake-to-play mechanic with tilt-based pet movement.
The MPU6886 accelerometer gives continuous X/Y tilt data, not just a shake event. That data
could be mapped to the pet's position within `ZONE_PET_FACE` — tilt the device left and the
pet drifts left, tilt right and it drifts right. The player "plays" by keeping the pet centred,
or by chasing a moving target.

**Why this is now possible:**
`drawPetFace()` was updated in Task 11 to accept `centerX`, `centerY`, and `radius` as
parameters instead of using hardcoded constants. This means the face can be repositioned
every frame by passing different values — the drawing function itself does not need to change.

**How it would work:**
1. Read raw accelerometer values from `ImuManager` each frame (X and Y tilt angles).
2. Map the tilt range (e.g. -30° to +30°) to a pixel offset within `ZONE_PET_FACE`.
3. Clamp the result so the face never leaves the zone boundary.
4. Pass the computed `centerX` and `centerY` to `drawPetFace()` each frame.

**What to revisit:**
- `lib/Imu/imu_manager.h/.cpp` — currently only exposes `wasShaken()`. Would need a
  `getTiltX()` and `getTiltY()` method that returns the raw or smoothed angle.
- `lib/Display/display_manager.cpp` → `drawPetView()` — this is where the tilt-to-position
  mapping would live, or in a new `AnimationManager` module.
- Consider adding a deadzone (small tilts do nothing) so the pet does not jitter when
  the device is held still.

**Teaching value:**
This is a great Level 5–6 task for students — it combines hardware reading (IMU), coordinate
maths (mapping a sensor range to screen pixels), and animation (moving a sprite each frame).

---

## Idle Animations

**The idea:**
Give the pet a subtle idle animation when nothing is happening — a gentle bob up and down,
blinking eyes, or a small breathing motion. This makes the pet feel alive even when no
buttons are being pressed.

**How it would work:**
Use `millis()` to drive a sine-wave offset applied to `centerY` in `drawPetFace()`. A small
amplitude (±5px) at a slow period (2–3 seconds) gives a natural breathing feel.

**What to revisit:**
- Needs the M5Canvas double-buffering approach (see USEFUL_NOTES.md — Gotcha 3) to avoid
  flicker at animation speeds faster than the current 5-second status refresh.
- The animation timer should be separate from `STATUS_UPDATE_INTERVAL` so it can run faster.

---

## Sprite-Based Mood Expressions

**The idea:**
Replace the current circle-and-lines pet face with bitmap sprites — one sprite per mood state
(happy, sad, hungry, sick, energised, neutral). The sprite renderer passes `ZONE_PET_FACE`
directly to the draw call so the sprite always fills the correct region.

**What to revisit:**
- Task 12 (Asset Pipeline) and Task 13 (Basic Sprite Rendering) are the planned path to this.
- `drawPetFace()` is the function to replace — or keep it as a fallback for boards without
  enough flash to store sprites.

---

## Time-Based Events (RTC Integration)

**The idea:**
Use the BM8563 RTC chip (already on the M5StickC Plus 2) to trigger time-aware events:
- Pet is grumpy in the morning until "fed breakfast".
- Pet gets tired automatically at night and needs sleep.
- Missed care events accumulate if the device is off for hours.

**What to revisit:**
- Task 14 (RTC overnight logic) is the planned path to this.
- Pairs well with the NVS persistence already implemented in Task 10 — save the last-seen
  timestamp so the device can calculate how much time passed while it was off.

---

## Evolution / Growth Stages

**The idea:**
The pet grows through stages (egg → baby → child → teen → adult) based on how well it has
been cared for over time. Different stages show different sprites and unlock different stats.

**What to revisit:**
- Task 9a (Evolution Logic) is already on the roadmap — deferred until sprites are ready.
- Pairs with Task 12/13 (Asset Pipeline + Sprite Rendering).
