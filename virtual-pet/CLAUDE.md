# Virtual Pet — Claude Code Context

## Project Summary

This is a Tamagotchi-style virtual pet for the **M5StickC Plus 2** (ESP32-PICO-V3-02).
It is a **teaching project** for students of varying skill levels. The code itself is the curriculum — students read it to learn embedded C++. Every decision must prioritise clarity over cleverness.

## Pedagogical Rules (Non-Negotiable)

These rules apply to every line of code written in this project:

1. **Readability first.** No clever or compact syntax. Expand every operation to its simplest readable form. No lambdas, ternaries, or one-liners where a plain `if` block would be clearer.
2. **Full descriptive variable names.** `hungerDecayInterval` not `hdi`. `lastHappinessDecayTime` not `lastHappy`.
3. **Every function gets a comment.** One sentence explaining what the function does AND why it exists. No exceptions.

## Hardware Quick-Reference

| Feature | Detail |
|---|---|
| Display | LCD 135×240 px (portrait) |
| Button A | Confirm / execute selected action |
| Button B | Next menu item |
| Button C | Previous menu item |
| Motion | MPU6886 accelerometer — used for Play mode (shake gesture). INT pin not routed on this board so interrupt-driven wake is not possible |
| Audio out | Built-in buzzer — use `M5.Speaker` |
| Audio in | Built-in microphone — use heap buffers only (see Gotchas) |
| Persistence | ESP32 NVS via Arduino `Preferences` library |

## Architecture Map

Each `lib/` module has exactly one job. Do not add logic to a module that belongs elsewhere.

| Module | File(s) | Responsibility |
|---|---|---|
| `Pet` | `lib/Pet/pet.h/.cpp` | All pet stats (0–100) and manual care actions (feed, play, sleep, bathe, heal). Computes its visual mood with `computeMood()` (a prioritised threshold ladder returning a `MoodSprite`). Plays its own alert/death/reset sounds — `updateState()` and `reset()` take a `SpeakerManager&` |
| `Display` | `lib/Display/display_manager.h/.cpp` | Everything drawn to the screen. Single unified render call to prevent flicker. Picks the pet's face from its `MoodSprite` via `spriteForMood()` (one sprite per mood) and shows the matching mood word |
| `Display` | `lib/Display/animation_manager.h/.cpp` | Sprite frame-cycling — picks which sprite frame to draw using the non-blocking `millis()` pattern. Pure timing logic (no M5/display deps); `DisplayManager` owns one and queries `getCurrentFrame()` |
| `Display` | `lib/Display/tilt_motion.h/.cpp` | Optional tilt-movement demo — maps live accelerometer tilt → a low-pass-smoothed, clamped (x, y) pixel offset for the pet sprite. Pure float maths (no M5/display deps); `main.cpp` owns one, feeds it `imu.getAccelX/Y()`, and passes its offset into `DisplayManager::drawPetSprite()`. Gated by the `TILT_MOVEMENT_ENABLED` flag in `main.cpp` |
| `Button` | `lib/Button/button_handler.h/.cpp` | Edge-detection for buttons A, B, C. Call `update()` once per loop |
| `Actions` | `lib/Actions/action_menu.h/.cpp` | The 5-action menu (Feed/Play/Sleep/Bathe/Heal), cycling and confirmation |
| `Imu` | `lib/Imu/imu_manager.h/.cpp` | MPU6886 accelerometer — detects shake gestures. Call `update()` once per loop, query `wasShaken()` |
| `Timer` | `lib/Timer/time_manager.h/.cpp` | All automatic stat changes over time (hunger increase, happiness decay). Add new decay rules here |
| `Speaker` | `lib/Speaker/speaker_manager.h/.cpp` | Buzzer melodies and sound alerts (Task 8 — implemented) |
| `Microphone` | (out of scope) | Module moved to bonus during the curriculum realignment. See `CURRICULUM_REALIGNMENT.md`. The original stub files were deleted during Task 14a and are not being re-created in the active queue |
| `Storage` | `lib/Storage/storage_manager.h/.cpp` | NVS persistence via Arduino `Preferences` — saves and loads all pet stats |
| `Config` | `lib/Config/scaffold_config.h` | The six `ENABLE_*` curriculum feature switches (commented-`#define` style, all ON on this reference branch). Included by every gated file so all separately-compiled translation units agree on which features are built in. Holds the `#error` guard for the one inter-flag rule: `ENABLE_PERSISTENCE` requires `ENABLE_ACTION_MENU` (the Save action lives in the menu) |

Each major feature is wrapped in `#ifdef ENABLE_*` (added in Task 21) so it can be compiled out for a given teaching session — see `lib/Config/scaffold_config.h`. A flag OFF means the feature is genuinely **absent, not inert**: e.g. with `ENABLE_SOUND` off, the `SpeakerManager&` parameter leaves `Pet::updateState()/reset()` and `ActionMenu::confirmAction()` entirely. The flags are verified as a cumulative staircase (Session 1 all-off → Session 6 all-on), not as all 2⁶ combinations.

## Current Progress

> **Active migration:** `CURRICULUM_REALIGNMENT.md` is the source of truth for current work.
> Read it first to understand the four-phase plan that takes this repo from "engineer-facing
> reference implementation" to "frozen curriculum reference + a separate teaching repo."

Also read `DEV_ROADMAP.md` for task-level detail and `COURSE_CHECKLIST.md` for per-feature status.

**Next task on the queue:** **Define Session 1 lesson plan** (Phase 2 of the realignment). Task 22 (Doc Sweep) is complete, so Phase 1 is closed and this repo is the frozen curriculum reference. Next: draft `LESSON_PLANS/SESSION_01.md` in the minute-budgeted lesson-plan format, build with Session 1's `ENABLE_*` set and flash to verify the day-start behaviour matches the plan, then draft `PROMOTION_GUIDE.md` notes for the new repo. Full scope in `CURRICULUM_REALIGNMENT.md` → Phase 2.

**After Task 22**, Phase 1 is complete and work moves to the `virtual-pet-learning-lab` repo (Phases 2–4 of the realignment). See `DEV_ROADMAP.md` and `CURRICULUM_REALIGNMENT.md`.

**Execution order from here (Phase 1 of the realignment):**
Task 14c (Gameplay Balance) → Task 14d (Sprite Display Simplification) → Task 13a (Sprite Animation) → Task 13b (Tilt-Reactive Sprite Movement) → Task 20 (Mood Sprite System) → Task 21 (Curriculum Scaffolding Refactor) → Task 22 (Doc Sweep) → move to `virtual-pet-learning-lab` repo (Phases 2–4 of the realignment).

**Just completed:** Task 22 — Doc Sweep. Brought the docs into line with the now-frozen code. **`COURSE_CHECKLIST.md`** rewritten around the 10-session arc (each session tagged with its day-start state + the `ENABLE_*` flag that activates it; microphone/RTC/wireless/evolution/dashboard/sadness moved to a Bonus section; the stale "template per complexity level" Phase 6 replaced with the real session-branch + lesson-plan model). **`IDEAS.md`** — shipped items (IMU movement, mood sprites) trimmed to one-line pointers and the Idle-Animations stale refs fixed. **`USEFUL_NOTES.md`** — a full section-by-section accuracy audit against the source fixed: the sprite converter (it reads **ABGR8888**, not ARGB; `convertAbgrToRgb565`), a rewrite of the screen-redraw section around the M5Canvas double-buffer (the 5s `STATUS_UPDATE_INTERVAL` throttle + fast-path were removed in Task 13a), the `enum class`→plain-enum and non-existent `ZONE_PET_FACE` examples, the `NavigationManager::update(buttons, bool backSelected)` signatures, the removed `Pet::checkDeathAlert()` and `confirmAction()` `delay(1000)`, adding `AnimationManager`+`TiltMotion` to the module overview, correcting the `Pet` "one job" row (it owns mood and triggers its own sounds), and finishing the Task 19 `player`→`user` rename. **`DEV_ROADMAP.md`** — resolved the stale 2^6 "SETTLE THESE FIRST" design note. Known follow-ups discovered but out of Task 22's scope: the empty `lib/Microphone/` stub files contradict the architecture-map note below (they should be deleted), and the `screen_layout.h:44` comment still says the Stats screen shows a pet face. Branch: `task/22-doc-sweep`.

**Previously completed:** Task 21 — Curriculum Scaffolding Refactor. Added `lib/Config/scaffold_config.h` holding six commented-`#define` `ENABLE_*` switches (all ON on this reference branch; session branches comment some out) plus a `#error` guard enforcing the one inter-flag rule — `ENABLE_PERSISTENCE` requires `ENABLE_ACTION_MENU` because the Save action lives in the menu. Each feature is compiled out when its flag is off (not merely inert): **ENABLE_SOUND** — the `SpeakerManager&` parameter leaves `Pet::updateState/reset` and `ActionMenu::confirmAction` entirely, and the alert/play calls are gated; **ENABLE_PERSISTENCE** — storage + the Save menu action, with the action array kept dense (Back shifts to index 5, `NUM_ACTIONS` → 6) so the B/C wrap-around still works; **ENABLE_ACTION_MENU** — the menu object + the whole Interact screen (NavigationManager case/handler, DisplayManager render path, B-from-Main transition, the "Interact" nav tab); `main.cpp` falls back to placeholder render values through a named-local seam so `renderDisplay()`'s signature never changes; **ENABLE_IMU_PLAY** — only the shake→`play()` link (ImuManager keeps running because the tilt demo also reads it); **ENABLE_MULTISCREEN** — the Stats screen + its mood word ONLY (Main and Interact always exist, so the menu never has to move screens — design decision 3); **ENABLE_MOOD_SPRITES** — the non-NEUTRAL rungs of `computeMood()` and the non-NEUTRAL cases of `spriteForMood()`/`showPetMoodText()`, all falling back to the NEUTRAL face + word. The flags live in one shared header (not `main.cpp`) because a `#define` in `main.cpp` does not reach the separately-compiled `lib/*.cpp` units. Verification is a **cumulative staircase** (Session 1 all-off → Session 6 all-on), NOT all 2^6 combos; every tier compiles and the full staircase was device-verified. Branch `task/21-scaffolding-refactor`, merged to `main`.

**Earlier:** Task 20 — Mood Sprite System. Added a `MoodSprite` enum (`MOOD_NEUTRAL / MOOD_HAPPY / MOOD_UNWELL / MOOD_HUNGRY`) in `lib/Display/screen_layout.h` — placed there (not in `pet.h`) alongside `ScreenState`/`RelevantStat` so `DisplayManager` can name moods without depending on `Pet`; `Pet` includes the lightweight header to return one, mirroring how the action menu uses `RelevantStat`. New `Pet::computeMood()` replaces the old `getDominantMood()` "highest stat wins" scan with a prioritised threshold ladder — `sick > 50 → MOOD_UNWELL`, else `hungry > 70 → MOOD_HUNGRY`, else `happy > 70 → MOOD_HAPPY`, else `MOOD_NEUTRAL` (first match wins). The whole display chain (`renderDisplay → renderMain/Stats/InteractScreen → drawPetSprite`/`showPetMoodText`) now carries a typed `MoodSprite` instead of an `int moodIndex`, so the sprite and the mood word can never disagree. A new `DisplayManager::spriteForMood()` switch is the single place mapping a mood to its artwork; `drawPetSprite()` now performs that lookup itself (the previously-reserved `moodIndex` param is realised, and the redundant `spriteData` param dropped). Four single-frame 80×80 placeholder sprites live in `lib/Display/sprites/{neutral,happy,unwell,hungry}_placeholder.h` (the `unwell` one reuses sad-faced placeholder art); the `AnimationManager` is set to frame count 1 for now, holding frame 0 — ready to bump to 2 when animated per-mood art lands. Known scope limit (accepted): the four moods give no sprite warning for the two *low* fatal stats (`happy → 0`, `energised → 0`) — the face stays NEUTRAL until death; a future `MOOD_SAD`/`MOOD_TIRED` is the natural "add your own mood" student exercise (enum value + `computeMood()` rule + `spriteForMood()` case). Verified on device (each mood's face + word, priority, transparency). Branch: `task/20-mood-sprite-system`.

**Earlier still:** Task 13b — Tilt-Reactive Sprite Movement. Added `lib/Display/tilt_motion.h/.cpp` — a standalone `TiltMotion` helper (pure float maths, no M5/display deps) that turns live accelerometer X/Y into a smoothed, clamped pet-sprite offset. Pipeline per loop: scale tilt by `TILT_SCALE` → `constrain()` to `MAX_OFFSET_X/Y` (so the pet stays on screen) → low-pass filter (`smoothed += (target - smoothed) * SMOOTHING_FACTOR`, the position analogue of the `millis()` timer) → `getOffsetX/Y()` round to whole pixels. Threaded two `spriteOffsetX/Y` primitives through `DisplayManager::renderDisplay()` → `renderMainScreen`/`renderInteractScreen` → `drawPetSprite()` (Stats screen untouched — no sprite). `main.cpp` owns the `TiltMotion` instance, feeds it `imu.getAccelX/Y()` (finally using those getters), and gates the whole effect behind a single `TILT_MOVEMENT_ENABLED` flag — off pins the offset to `0, 0` and the pet draws dead-centre exactly as before. Axis/sign/sensitivity verified correct on-device on the first try (no constant changes needed; to invert an axis, negate `accelX`/`accelY` in `update()`). Student-facing walkthrough added as `SPRITE_GUIDE.md` Part 7. Branch: `task/13b-tilt-movement`.

**Out of scope** (for the learning lab, but kept as design notes / bonus):
Task 16 (Microphone), Task 17 (Wireless AP), Task 18 (Remote Dashboard), Task 9a (Evolution), Task 15 (RTC).

**Open questions tracked in `CURRICULUM_REALIGNMENT.md`:** other cleanup candidates (for Task 19 — capture as discovered). (Mood threshold values for Task 20 — resolved and shipped.)

## Hardware Gotchas

**1 — Audio buffers must use the heap.**
Never declare a large audio array inside a function (stack overflow). Always use `malloc()` / `free()` and null-check the result. See DEV_ROADMAP.md Part 3 for full example.

**2 — Never use `delay()` inside `loop()`.**
`delay()` freezes button input and the display. Use the `millis()` non-blocking timer pattern instead — the same pattern used in `lib/Timer/time_manager.cpp`.

**3 — Use M5Canvas to prevent screen flicker.**
For any animation faster than the current 5-second status refresh, draw to an off-screen `M5Canvas` sprite first, then call `pushSprite()` once. A full 135×240 canvas costs ~63 KB of heap. See DEV_ROADMAP.md Part 3 for full example.

**4 — Every `lib/` `.cpp` file needs `#include <Arduino.h>`.**
PlatformIO does not inject Arduino functions automatically into library files. `millis()`, `delay()`, `pinMode()` etc. will fail to compile without this include at the top of the file.

## Git Strategy

The git history is part of the curriculum. A student reading `git log` should be able to see exactly what changed at each step and understand why. Clear branches and commit messages also make it safe to experiment — if something goes wrong on a branch, you can delete it and start the task again without touching working code on `main`.

### Branch naming

Every task gets its own branch before any code is written. Use the task number from the complexity queue in `DEV_ROADMAP.md`:

```
task/N-short-description
```

Examples: `task/3-death-reset-condition`, `task/4-state-machine`, `task/8-buzzer-sound-feedback`

```
git checkout -b task/3-death-reset-condition
```

### Commit message format

```
<type>: <short summary in present tense, under 72 characters>

<1–3 sentences explaining what changed and why it was necessary>
```

The body answers "why was this necessary?" not just "what did I type?". The type prefix must be one of:

| Type | Use it when |
|---|---|
| `feat:` | Adding new working functionality |
| `fix:` | Correcting something broken or incorrect |
| `docs:` | Updating comments, README, CLAUDE.md, or roadmap files only |
| `chore:` | Build config, `.gitignore`, or file organisation with no behaviour change |
| `refactor:` | Restructuring code without changing its observable behaviour |

### Atomic commits — one logical change per commit

Commit one complete, logical change at a time. If you find yourself writing "and" in the commit subject line, split it into two commits.

### Merge rule

`main` always contains working code. Test the feature on the device before merging a branch back to `main`. If you discover a bug after merging, create a `fix/` branch to fix it rather than pushing directly to `main`.

### Everyday workflow

```
# 1. Start from a clean main
git checkout main

# 2. Create a branch for the task
git checkout -b task/3-death-reset-condition

# 3. Work, committing one logical change at a time
git add lib/Pet/pet.h lib/Pet/pet.cpp
git commit -m "feat: add isDead() — returns true when hunger hits 100 or energy hits 0

The pet had no way to detect a critical stat. This is the first step
toward a proper death and reset flow so the game loop can respond."

# 4. Test on device, then merge back when working
git checkout main
git merge task/3-death-reset-condition
```
