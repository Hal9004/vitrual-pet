# Virtual Pet — Claude Code Context

## Project Summary

This is a Tamagotchi-style virtual pet for the **M5StickC Plus 2** (ESP32-PICO-V3-02).
It is a **teaching project** for students of varying skill levels. The code itself is the curriculum — students read it to learn embedded C++. Every decision must prioritise clarity over cleverness.

## Pedagogical Rules (Non-Negotiable)

These rules apply to every line of code written in this project:

1. **Readability first.** No clever or compact syntax. Expand every operation to its simplest readable form. No lambdas, ternaries, or one-liners where a plain `if` block would be clearer.
2. **Full descriptive variable names.** `fullnessDecayInterval` not `fdi`. `lastHappinessDecayTime` not `lastHappy`.
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
| `Pet` | `lib/Pet/pet.h/.cpp` | All pet stats (0–100) and manual care actions (feed, play, sleep, bathe, heal). Hunger is modelled as **`fullness`** — it starts full, **decays toward 0** like the other stats, feeding **refills** it, and `0 = starved = death`. Computes its visual mood with `computeMood()` (a prioritised threshold ladder returning a `MoodSprite`; looks `MOOD_HUNGRY` when fullness is low). Plays its own alert/death/reset sounds — `updateState()` and `reset()` take a `SpeakerManager&` |
| `Display` | `lib/Display/display_manager.h/.cpp` | Everything drawn to the screen. Single unified render call to prevent flicker. Picks the pet's face from its `MoodSprite` via `spriteForMood()` (one sprite per mood) and shows the matching mood word. Draws a single **fullness bar on the Main screen** so the pet always has one visible stat (from Session 1, before any other screen exists) |
| `Display` | `lib/Display/animation_manager.h/.cpp` | Sprite frame-cycling — picks which sprite frame to draw using the non-blocking `millis()` pattern. Pure timing logic (no M5/display deps); `DisplayManager` owns one and queries `getCurrentFrame()` || `Button` | `lib/Button/button_handler.h/.cpp` | Edge-detection for buttons A, B, C. Call `update()` once per loop |
| `Actions` | `lib/Actions/action_menu.h/.cpp` | The 5-action menu (Feed/Play/Sleep/Bathe/Heal), cycling and confirmation |
| `Imu` | `lib/Imu/imu_manager.h/.cpp` | MPU6886 accelerometer — detects shake gestures. Call `update()` once per loop, query `wasShaken()` |
| `Imu` | `lib/Imu/tilt_motion.h/.cpp` | Reusable motion-smoothing helper (pure float maths, no M5/display deps) — low-pass-smooths and clamps live accelerometer tilt into a steady (x, y) value. Its first use is the pet-sprite screen offset (`main.cpp` feeds it `imu.getAccelX/Y()` and passes the offset to `DisplayManager::drawPetSprite()`), but the smoothed motion can drive other interactions too. Lives in `lib/Imu/` next to the raw reader; compiled only under `ENABLE_IMU_PLAY`, with an inner `TILT_MOVEMENT_ENABLED` on/off in `main.cpp` |
| `Timer` | `lib/Timer/time_manager.h/.cpp` | All automatic stat changes over time (fullness decay, happiness decay, energy drain, …). Add new decay rules here. A `FAST_TEST` compile switch swaps in a fast-decay set for development; fullness is tuned to empty first so the visible bar drives the death |
| `Speaker` | `lib/Speaker/speaker_manager.h/.cpp` | Buzzer melodies and sound alerts |
| `Microphone` | (out of scope) | Module deferred to bonus. See `DEV_ROADMAP.md` Appendix B. No stub files exist — it is not being re-created in the active queue |
| `Storage` | `lib/Storage/storage_manager.h/.cpp` | NVS persistence via Arduino `Preferences` — saves and loads all pet stats |
| `Config` | `lib/Config/scaffold_config.h` | The six `ENABLE_*` curriculum feature switches (commented-`#define` style, all ON on this reference branch). Included by every gated file so all separately-compiled translation units agree on which features are built in. Holds the `#error` guard for the one inter-flag rule: `ENABLE_PERSISTENCE` requires `ENABLE_ACTION_MENU` (the Save action lives in the menu) |

Each major feature is wrapped in `#ifdef ENABLE_*` so it can be compiled out for a given teaching session — see `lib/Config/scaffold_config.h`. A flag OFF means the feature is genuinely **absent, not inert**: e.g. with `ENABLE_SOUND` off, the `SpeakerManager&` parameter leaves `Pet::updateState()/reset()` and `ActionMenu::confirmAction()` entirely. The flags are verified as a cumulative staircase (Session 1 all-off → Session 6 all-on), not as all 2⁶ combinations.

## Curriculum, Lesson Plans & Student Workflow

The 10-session course is delivered through lesson plans in **`LESSON_PLANS/`**
(`SESSION_01.md` … `SESSION_10.md`) plus **`LESSON_PLANS/WORKFLOW.md`** (the student tooling
routine). **Author or revise any lesson with the `lesson-design` skill** (a user-level Claude
Code skill) — it encodes the rules below.

**Feature sessions (1–6) follow one shape — Reveal → Play → Learn → Build:**
1. **Reveal** — flip ONE `ENABLE_*` flag and watch the feature appear (Session 1 is the all-OFF baseline and previews the flag model).
2. **Play** — change a few labelled "dials" (no code) with an obvious on-device result. The floor everyone clears.
3. **Learn** — the ONE coding fundamental the feature teaches, in one sentence, pointed to in the code.
4. **Build** — optional, ordered ★→★★★ challenges that extend the revealed feature. The ceiling.

Navigation (the "meet the team" map + read-only/workshop zones) is taught up front each session.
Sessions 7–9 invert this into open-development/challenge tracks; Session 10 is a no-code showcase.

**Fundamentals pathway (the spine):** variables → functions → `if` → lists + loops → save/load
→ `if`-ladders → extend real code → polish → debug → present.

**Student workflow = Google Drive + VS Code + PlatformIO — NOT Git.** Students download the
starter project from a class Google Drive folder, open it in VS Code (PlatformIO auto-loads),
**Build/Upload** to the device, and **save the project back to Drive** at the end of every
session. Full steps in `LESSON_PLANS/WORKFLOW.md`. (The Git Strategy below is the *development*
workflow for this reference repo only — students never touch it.)

## Current Progress

> `DEV_ROADMAP.md` has per-task design detail; `COURSE_CHECKLIST.md` tracks
> per-session/feature status; `LESSON_PLANS/` holds the delivered curriculum.

**Phase 1 (frozen reference) is complete**, and **all 10 lesson plans are drafted and on
`main`** (`LESSON_PLANS/SESSION_01–10.md` + `WORKFLOW.md`), authored with the `lesson-design` skill.
The pet uses the **fullness** model (starts full, decays to 0, feeding refills, `0 = death`) with a
single fullness bar on the Main screen, and student onboarding is Google Drive + VS Code + PlatformIO.

**Next up:** stand up the separate **`virtual-pet-learning-lab`** repo — push the frozen `main`,
cut `session-1-start … session-9-start` branches matching each session's `ENABLE_*` set, add the
student/teacher docs, and device-verify each branch behaves as its lesson plan promises. See
`DEV_ROADMAP.md` for the per-task detail.

**Deferred:** the board-wide stat-bar colour quirk (e.g. the fullness bar renders green).

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

> **This is the workflow for *developing* the reference repo (author + Claude Code). Students
> never use Git — they use Google Drive + VS Code (see the Curriculum section above).**

Clear branches and commit messages keep `main` working and make it safe to experiment — if something goes wrong on a branch, you can delete it and start over without touching working code on `main`. The commit history also documents how the reference was built.

### Branch naming

Every task gets its own branch before any code is written. Use the task number from the complexity queue in `DEV_ROADMAP.md`:

```
task/N-short-description
```

Examples: `task/3-death-reset-condition`, `task/4-state-machine`, `task/8-buzzer-sound-feedback`

Phase 1 used numbered `task/N-…` branches from the `DEV_ROADMAP.md` queue. Post-Phase-1 work (lessons, docs, fixes) uses descriptive branches instead — e.g. `task/session-1-code-alignment`, `docs/onboarding-drive-vscode`.

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
git commit -m "feat: add isDead() — returns true when fullness or energy hits 0

The pet had no way to detect a critical stat. This is the first step
toward a proper death and reset flow so the game loop can respond."

# 4. Test on device, then merge back when working
git checkout main
git merge task/3-death-reset-condition
```
