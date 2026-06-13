# Virtual Pet — Ideas & Future Directions

This file captures ideas that are worth revisiting once the core Tamagotchi is fully working.
None of these are planned tasks — they are starting points for exploration.

---

## IMU-Driven Pet Movement (Play Mechanic Upgrade) — ✅ shipped

Shipped in Task 13b as the optional tilt-movement demo: `lib/Display/tilt_motion.{h,cpp}` maps
live accelerometer tilt → a low-pass-smoothed, clamped pixel offset that
`DisplayManager::drawPetSprite()` applies, gated behind the `TILT_MOVEMENT_ENABLED` flag in
`main.cpp`. See `SPRITE_GUIDE.md` Part 7 for the student walkthrough.

---

## Idle Animations

**The idea:**
Give the pet a subtle idle animation when nothing is happening — a gentle bob up and down,
blinking eyes, or a small breathing motion. This makes the pet feel alive even when no
buttons are being pressed. (This is distinct from the frame-cycling already shipped in
`AnimationManager` — it is a smooth positional bob, not a swap between drawn frames.)

**How it would work:**
Use `millis()` to drive a sine-wave offset applied to the pet sprite's Y position in
`DisplayManager::drawPetSprite()`. A small amplitude (±5px) at a slow period (2–3 seconds)
gives a natural breathing feel.

**What to revisit:**
- The plumbing already exists: all rendering goes through a full-screen `M5Canvas`
  double-buffer (pushed once per frame), so animation at any speed is already flicker-free —
  no extra work is needed there.
- `TiltMotion` (`lib/Display/tilt_motion.{h,cpp}`) already feeds a smoothed (x, y) offset into
  `drawPetSprite()`. The idle bob is the same idea: add a `millis()`-driven sine offset to the
  Y position — either inside `TiltMotion` or as a small sibling helper — so the two offsets sum.

---

## Sprite-Based Mood Expressions — ✅ shipped

Shipped in Task 20 (Mood Sprite System): `Pet::computeMood()` maps stats → a `MoodSprite`
(NEUTRAL/HAPPY/UNWELL/HUNGRY), drawn via `DisplayManager::spriteForMood()`. Shipped with four
moods; `MOOD_SAD`/`MOOD_TIRED` remain as the "add your own mood" student extension. See
`SPRITE_GUIDE.md` Part 8 for the student walkthrough.

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
