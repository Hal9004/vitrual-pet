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
| `Pet` | `lib/Pet/pet.h/.cpp` | All pet stats (0–100) and manual care actions (feed, play, sleep, bathe, heal) |
| `Display` | `lib/Display/display_manager.h/.cpp` | Everything drawn to the screen. Single unified render call to prevent flicker |
| `Display` | `lib/Display/animation_manager.h/.cpp` | Sprite/bitmap animation — not yet implemented |
| `Button` | `lib/Button/button_handler.h/.cpp` | Edge-detection for buttons A, B, C. Call `update()` once per loop |
| `Actions` | `lib/Actions/action_menu.h/.cpp` | The 5-action menu (Feed/Play/Sleep/Bathe/Heal), cycling and confirmation |
| `Imu` | `lib/Imu/imu_manager.h/.cpp` | MPU6886 accelerometer — detects shake gestures. Call `update()` once per loop, query `wasShaken()` |
| `Timer` | `lib/Timer/time_manager.h/.cpp` | All automatic stat changes over time (hunger increase, happiness decay). Add new decay rules here |
| `Speaker` | `lib/Speaker/speaker_manager.h/.cpp` | Buzzer melodies and sound alerts — not yet implemented |
| `Microphone` | `lib/Microphone/microphone_manager.h/.cpp` | Voice memo record/playback — not yet implemented |
| `Storage` | `lib/Storage/storage_manager.h/.cpp` | NVS persistence via Arduino `Preferences` — saves and loads all pet stats |

## Current Progress

Read `DEV_ROADMAP.md` for the full 17-task complexity queue and detailed implementation guides.
Read `COURSE_CHECKLIST.md` for a quick view of which checklist items are done vs. pending.

**Next task on the queue:** Screen Real Estate layout zones (Task 11) — refactor the hardcoded pixel layout from Task 5 into reusable structs. See `DEV_ROADMAP.md` for details.

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
