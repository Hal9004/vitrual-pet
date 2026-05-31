# Curriculum Realignment Plan

> **Status:** Active. This is the source of truth for the work currently underway.
> Once Phase 4 is complete, this file is archived to `docs/archive/` — by then
> `DEV_ROADMAP.md`, `COURSE_CHECKLIST.md`, and the new repo's `LESSON_PLANS/`
> are authoritative.

---

## Why this exists

This repo (`vitrual-pet`) was the author's reference implementation for a
learning lab on the M5StickC Plus 2. Over time it accumulated scope creep
relative to what the curriculum actually needs:

- Task 16 (Microphone — heap allocation, Level 6) is over-scoped for cold
  students and is not in the course outline.
- Task 17 (WiFi AP) and Task 9a (Evolution) are similarly out of scope.
- `DEV_ROADMAP.md` is engineer-facing, not student- or teacher-facing.
- The Phase 6 "Student Template Creation" steps were unwritten.

**The plan:** clean up this repo so its `main` matches a single,
curriculum-driven "finished reference" spec, then stand up a *separate*
teaching repo (`virtual-pet-learning-lab`) for the student- and teacher-facing
artefacts.

The two repos stay decoupled forever: this repo is the author's sandbox; the
teaching repo is the curriculum artefact. Author changes get promoted by
cherry-pick or copy, not by merge.

---

## The lab: confirmed decisions

### Basic Tamagotchi spec
- Pet visible on screen via **sprite** (not text/shapes).
- **5 stats** decay independently: hunger, happiness, energy, cleanliness, sickness.
- End state: **death → "press A to reset"** (reuses existing reset flow).
- **4 mood sprite states**: `NEUTRAL / HAPPY / UNWELL / HUNGRY`.
- Critical-stat alert via **buzzer**.
- **No evolution, no RTC, no day-night cycle.** Powering off freezes decay;
  saved stats reload on boot.
- **Pet name** hardcoded by student in their own code.

### Scaffolding architecture
- **Stub functions + `#define ENABLE_X` flags** for module-level features.
  Modules are present in every session's codebase but routed around until
  students activate them.
- Trivial extensions (a new `if` branch, one new stat) can use commented-out
  code instead of stubs.

### Curriculum arc (10 sessions × 4 hours each)

| # | Day-start codebase has | Session adds |
|---|---|---|
| 1 | Boot + screen + 1 button + 1 stat + bar + sprite-display path | Git basics + Piskel pipeline → student's own sprite + name + decay timer |
| 2 | + decay timer + multi-stat scaffolding (stubbed) | Activate multiple stats + button menu + multiple interactions |
| 3 | + IMU init | Tilt → sprite moves on screen + play mini-game |
| 4 | + buzzer init | Sound feedback (melodies for critical alerts) |
| 5 | + NVS persistence stubs | Save/load wired up |
| 6 | + multi-screen + 1 mood sprite | Multi-screen + 4-mood sprite system + 2-frame animation |
| 7 | Full pet | **Open development** — extend in their direction |
| 8 | Full pet | **Open development** continued + early polishing |
| 9 | Full pet | Final project — debug, finish, presentation prep |
| 10 | — | Showcase & reflection |

### Recurring 4-hour session rhythm
| Block | Length |
|---|---|
| Opening / recap / pair check-in | 25' |
| Presentation (2–3 mini-blocks) | 50' |
| Pair exploration prompt | 20' |
| Break | 20' |
| Guided task | 75' |
| Open creative block | 25' |
| Share-out + wrap | 15' |

Total ~230 min ≈ 3h50. Pair students deliberately (mix Programming II grads
with Programming I / cold students).

### `#define ENABLE_*` flag plan (set in `src/main.cpp`)

| Flag | Activates in |
|---|---|
| `ENABLE_ACTION_MENU` | Session 2 |
| `ENABLE_IMU_PLAY` | Session 3 |
| `ENABLE_SOUND` | Session 4 |
| `ENABLE_PERSISTENCE` | Session 5 |
| `ENABLE_MULTISCREEN` | Session 6 |
| `ENABLE_MOOD_SPRITES` | Session 6 |

### New teaching repo
- **Name:** `virtual-pet-learning-lab`
- **Host:** GitHub (private until ready, then public)
- `main` = cleaned-up reference (the "finished pet" as of Phase 1 completion)
- `session-1-start` … `session-9-start` = branches off `main`
- Students learn 5 Git commands: `clone`, `status`, `add`, `commit`,
  `checkout <branch>`

---

## Phases

### Phase 1 — Clean up this repo

Bring `main` of `vitrual-pet` to a state that matches the curriculum spec.
Step-by-step:

| Step | Task # | Branch | Status |
|---|---|---|---|
| 0 | (branch hygiene) | n/a | ✅ done — `task/16-microphone-detect-react` deleted; on `docs/curriculum-realignment` |
| 1.0 | (docs surgery + create this file) | `docs/curriculum-realignment` | 🟡 in progress |
| 1.1 | Task 19 — Pre-Template Simplification | `task/19-pre-template-simplification` | ✅ done |
| 1.1b | Task 19b — Pet-Owns-Sound Refactor | `task/19b-pet-owns-sound` | ✅ done |
| 1.2 | Task 14c — Gameplay Balance Tuning | `task/14c-balance-tuning` | pending |
| 1.3 | Task 14d — Sprite Display Simplification | `task/14d-sprite-simplification` | pending |
| 1.4 | Task 13a — Sprite Animation | `task/13a-sprite-animation` | pending |
| 1.5 | Task 20 — Mood Sprite System (new) | `task/20-mood-sprites` | pending |
| 1.6 | Task 21 — Curriculum Scaffolding Refactor (new) | `task/21-scaffolding-refactor` | pending |
| 1.7 | Task 22 — Doc Sweep (new) | `task/22-doc-sweep` | pending |

### Phase 2 — Define Session 1 in this repo

Before standing up the new repo, write Session 1's lesson plan and verify
its day-start codebase compiles here.

1. Draft `LESSON_PLANS/SESSION_01.md` in this repo (temporary location) in the
   minute-budgeted lesson-plan format.
2. Build with Session 1's `#define` set, flash to device, verify behaviour
   matches the lesson plan.
3. Draft `PROMOTION_GUIDE.md` bullet notes for the new repo.

### Phase 3 — Stand up `virtual-pet-learning-lab`

1. Create the repo on GitHub.
2. Push cleaned-up `vitrual-pet` `main` as the new repo's `main`.
3. Cut `session-1-start` branch matching the Session 1 spec.
4. Add docs: `STUDENT_README.md`, `GIT_PRIMER.md`, `SPRITE_GUIDE.md`,
   `TEACHER_README.md`, `PROMOTION_GUIDE.md`, `LESSON_PLANS/SESSION_01.md`.

### Phase 4 — Write Sessions 2–10

For each of Sessions 2 through 10:
1. Write `LESSON_PLANS/SESSION_NN.md` in the minute-budgeted lesson-plan format using Session 1 as a template.
2. Specify which `ENABLE_*` flags are on for that session's day-start.
3. Cut `session-N-start` branch.
4. Verify the branch compiles and behaves as the lesson plan promises.

---

## Continuity protocol (across Claude Code sessions)

1. `CLAUDE.md`'s "Current Progress" section is the entry point — it points
   here and at the current Step.
2. Each completed Step gets a commit following the project's
   `<type>: <subject>` convention.
3. Update this file's Phase 1 status table as Steps complete.
4. To pick up state in a new session: open `CLAUDE.md`, then this file, then
   `git log --oneline -10`.

---

## Open questions (still need answers before later Steps run)

1. **Mood threshold rules** (needed for Step 1.5 / Task 20). Default proposal,
   priority order:
   - `UNWELL` when `sickness > 50` (highest priority)
   - `HUNGRY` when `hunger > 70`
   - `HAPPY` when `happiness > 70` (and not unwell or hungry)
   - `NEUTRAL` otherwise

2. **Other Task 19 cleanup candidates** beyond mic/wifi/evolution.
   Discovered during the Task 19 audit; capture findings here as they come up.

---

## Verification

**Phase 1 done when:**
- `main` builds and flashes cleanly on the M5StickC Plus 2.
- `DEV_ROADMAP.md`, `COURSE_CHECKLIST.md`, `IDEAS.md`, `CLAUDE.md` all describe
  the same trimmed-scope project with no contradictions.
- The 4-mood sprite system works on device.
- The 6 `ENABLE_*` flags can be toggled to produce 6 working intermediate
  builds matching each session's day-start state.

**Phase 2 done when:**
- `LESSON_PLANS/SESSION_01.md` exists in the minute-budgeted lesson-plan format.
- A different teacher could deliver Session 1 without needing the author's help.

**Phase 3 done when:**
- `virtual-pet-learning-lab` repo exists on GitHub.
- A fresh `git clone` + `git checkout session-1-start` produces code that
  compiles and flashes as Session 1's day-start pet.

**Phase 4 done when:**
- `LESSON_PLANS/SESSION_01.md` through `SESSION_10.md` all exist.
- `session-1-start` through `session-9-start` branches all compile and
  behave as their lesson plans promise.
