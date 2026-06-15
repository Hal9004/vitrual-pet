# Tamagotchi Project — Course Checklist

Organised around the **10-session curriculum arc** (the source-of-truth table lives in
`CURRICULUM_REALIGNMENT.md`). This repo is the **frozen reference implementation**: every
core item below is built and working on `main`. Each session's day-start branch in the
`virtual-pet-learning-lab` repo turns features on or off with the `ENABLE_*` switches in
`lib/Config/scaffold_config.h`, so a session's codebase contains only what students have
reached so far.

> A flag OFF means the feature is genuinely **absent**, not inert. Verification is a
> cumulative staircase (Session 1 all-off → Session 6 all-on), NOT all 2⁶ combinations —
> the flags are not independent: `ENABLE_PERSISTENCE` requires `ENABLE_ACTION_MENU` because
> the Save action lives in the menu.

---

## Session 1 — First Pet on Screen
**Day-start:** boot + screen + one button + one stat + bar + sprite-display path.
**Students build:** Git basics, the Piskel → C++ sprite pipeline, their own sprite + pet name, one decay timer.

- [x] Hardware init (M5.begin, LCD, Serial) — `src/main.cpp` → `setup()`, `DisplayManager::init()`
- [x] Asset pipeline (image → C++ array) — `tools/piskel_converter`, `SPRITE_GUIDE.md`. Piskel exports **ABGR8888**; the tool converts to RGB565 and pre-byte-swaps for the M5StickC Plus 2's big-endian SPI byte order (transparent key `0x1FF8`)
- [x] Basic sprite rendering — `DisplayManager::drawPetSprite()` via `pushImage()`; a single 80×80 sprite on the Main (and later Interact) screen
- [x] One decay timer (the non-blocking `millis()` pattern) — `lib/Timer/time_manager.cpp`

## Session 2 — Stats, Menu & Interactions
**Day-start:** + decay timer + multi-stat scaffolding (stubbed).
**Activated by:** `ENABLE_ACTION_MENU`.

- [x] Five stats decay independently — `TimerManager` (hunger, happiness, energy, cleanliness, sickness)
- [x] Action menu — Feed / Play / Sleep / Bathe / Heal — `lib/Actions/action_menu.cpp`; B & C cycle, A confirms
- [x] State machine (IDLE / EATING / SLEEPING / PLAYING / SICK / HEALING / BATHING / DEAD) — `lib/Pet/pet.*`
- [x] Death / reset condition (handling 0 stats) — `Pet::isDead()` / `reset()`, death screen via `renderDisplay()`

## Session 3 — Motion
**Day-start:** + IMU init.
**Activated by:** `ENABLE_IMU_PLAY`.

- [x] Shake-to-play — `lib/Imu/imu_manager.cpp` → `wasShaken()` triggers `pet.play()`
- [x] Tilt-reactive sprite movement (optional demo) — `lib/Display/tilt_motion.*`, gated by `TILT_MOVEMENT_ENABLED`

## Session 4 — Sound
**Day-start:** + buzzer init.
**Activated by:** `ENABLE_SOUND`.

- [x] Buzzer melodies for all five actions + hunger/sickness alerts — `lib/Speaker/speaker_manager.cpp`
- [x] Pet plays its own death / reset / hunger / sickness sounds — `Pet::updateState()` / `reset()` take a `SpeakerManager&`

## Session 5 — Persistence
**Day-start:** + NVS persistence stubs.
**Activated by:** `ENABLE_PERSISTENCE` (requires `ENABLE_ACTION_MENU` — the Save action lives in the menu).

- [x] NVS save/load via Arduino `Preferences` — `lib/Storage/storage_manager.cpp`; Save action writes, `setup()` loads. (Originally labelled "EEPROM", but the ESP32 has no real EEPROM — NVS is the correct native mechanism.)

## Session 6 — Screens & Mood
**Day-start:** Main + Interact screens + one mood sprite.
**Activated by:** `ENABLE_MULTISCREEN` + `ENABLE_MOOD_SPRITES`.

- [x] Stats screen — a read-only data view: five stat bars + mood word, **no pet sprite** — `DisplayManager::renderStatsScreen()`
- [x] 4-mood sprite system — `Pet::computeMood()` → `MoodSprite` (NEUTRAL/HAPPY/UNWELL/HUNGRY); `DisplayManager::spriteForMood()` is the one place mapping mood → artwork
- [x] 2-frame sprite animation — `lib/Display/animation_manager.cpp` (mood placeholders are single-frame for now; bump the frame count when animated art lands)

## Sessions 7–9 — Open Development & Final Project
**Day-start:** the full pet.
Students extend in their own direction (Session 7), continue and begin polishing (Session 8), then debug, finish, and prepare a presentation (Session 9). No new core features are required from the reference — these sessions draw on the bonus/extension ideas below and `IDEAS.md`.

## Session 10 — Showcase & Reflection
No code — demo and reflection.

---

## Bonus / Out of Scope (not core sessions)

Right-sized out of the critical path during the Task 14b audit / curriculum realignment.
Kept as opt-in extensions and design references. Full design notes live in
`DEV_ROADMAP.md` Appendix B.

- [ ] Sadness logic — `sad` rises automatically when `happy` is low; needs a `TimerManager` rule + a sad sprite. The natural "add your own mood" (`MOOD_SAD` / `MOOD_TIRED`) extension exercise.
- [ ] Microphone input (detect & react) — Level-6 heap allocation, too dense for cold students; bonus.
- [ ] RTC overnight logic — bonus; without overnight-decay integration a clock is a stand-alone widget.
- [ ] Evolution / growth stages — bonus.
- [ ] Wireless access-point primitive — bonus (Sessions 7–8 extension path, not a core task).
- [ ] Remote dashboard (web/app) — bonus.
- [ ] `SpeakerManager::playNote()` helper — deferred polish; melodies kept verbose so each reads top-to-bottom.

---

## After the reference freeze — building the lab
(See `CURRICULUM_REALIGNMENT.md` Phases 2–4.)

- [ ] Write Session 1's lesson plan — `LESSON_PLANS/SESSION_01.md` — and verify its day-start build flashes
- [ ] Stand up `virtual-pet-learning-lab`; push cleaned `main`; cut `session-1-start` … `session-9-start` branches
- [ ] Write lesson plans `SESSION_02.md` … `SESSION_10.md`; verify each branch compiles and behaves as its plan promises
