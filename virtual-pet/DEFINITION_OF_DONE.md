# Definition of Done

> **Status:** Living document. Updated as tasks land or scope decisions change.
> **Deadline:** Teaching the learning lab starts **2026-06-27**. Every "should we add this?" question gets weighed against that date. If a thing is not on this page, it does not ship in Phase 1.

This file is the single place to check two questions while working:

- **Is task X done?** — find its block under the relevant phase and tick the bullets.
- **Should I add feature Y?** — check the *Out of scope* list. If Y is there, the answer is no.

Design rationale and per-task implementation notes live in `DEV_ROADMAP.md`; this file is the pass/fail checklist that points at it.

---

## The finished product (Phase 1 — this repo)

When `main` of this repo is frozen and copied to `virtual-pet-learning-lab`, the pet must:

- Be visible on screen as a **sprite** (not text or shapes).
- Have **5 stats** decay independently: `fullness`, `happiness`, `energy`, `cleanliness`, `sickness`.
- React to the user's five care actions: `feed`, `play`, `sleep`, `bathe`, `heal`.
- Show **4 mood sprite states** — `NEUTRAL` / `HAPPY` / `UNWELL` / `HUNGRY` — picked by `Pet::computeMood()`.
- Play **buzzer melodies** for action feedback, low-fullness / sickness alerts, and death.
- Detect a **shake gesture** via IMU → triggers `play()` from any screen.
- **Save and restore** stats via NVS so power-cycling does not wipe progress.
- **Die** when `fullness`, `energy`, or `happiness` reaches 0 → "press A to reset" screen.
- Carry a **hardcoded pet name** (each student edits this line in their own code).
- Render three screens — Main, Stats, Interact — navigable with Button B / C.
- Compile + flash cleanly under each of six `ENABLE_*` flag combinations matching the 10-session day-start states.

Source: the Basic Tamagotchi spec in `DEV_ROADMAP.md`.

---

## Out of scope — DO NOT BUILD on the critical path

If any of these come up between now and 2026-06-27, the answer is "bonus / stretch, not now". Designs are preserved in the linked locations so they can be picked up later.

| Feature | Why it's out | Where the design lives |
|---|---|---|
| Microphone (detect / react / record) | Right-sized to bonus in Task 14b — three modules' worth of new concepts in one task | `DEV_ROADMAP.md` § Task 16 + Appendix B Bonus 6 |
| WiFi Access Point / wireless comms | Right-sized to bonus in Task 14b — not in the curriculum arc | `DEV_ROADMAP.md` § Task 17 + Appendix B Bonus 3–5 |
| Remote Dashboard (web UI) | Bonus — its own slug of new concepts (routes, callbacks, HTML) | `DEV_ROADMAP.md` Appendix B Bonus 2 + 4 |
| Evolution / growth stages | Out of curriculum spec — "no evolution" is an explicit decision | `DEV_ROADMAP.md` Appendix B |
| RTC / day-night cycle | Out of curriculum spec — "no RTC, no day-night cycle" | `DEV_ROADMAP.md` Appendix B Bonus 1 |
| Sadness logic / sad sprite | Deferred indefinitely — a natural "add your own mood" student exercise | `COURSE_CHECKLIST.md` Bonus section |
| Student template extraction | Replaced by the new repo arc — templates *are* the `session-N-start` branches | `DEV_ROADMAP.md` |

If asked "should I add X" and X is on this list, the answer is **no**.

---

## Phase 1 freeze — pre-handoff device QA

Run all of these on hardware **before** copying `main` to `virtual-pet-learning-lab`. This is the gate between Phase 1 and Phase 2.

- [ ] Cold boot: `storage.load()` returns saved stats *or* healthy defaults — pet always boots alive.
- [ ] All five care actions (`feed` / `play` / `sleep` / `bathe` / `heal`) execute and visibly update their stats (feeding refills `fullness`).
- [ ] Background decay observably ticks: fullness, happiness, energy, cleanliness, sickness all change without input over a multi-minute run.
- [ ] Main screen shows a **fullness bar** that visibly drains over time and refills on `feed`.
- [ ] DEBUG Serial stat readout prints the live stat values for verification during a run.
- [ ] Shake gesture triggers `play()` from every screen.
- [ ] Low-fullness alert sound plays at threshold; sickness alert sound plays at threshold.
- [ ] Death sound plays exactly once on the first frame of death.
- [ ] Pet dies when `fullness`, `energy`, or `happiness` reaches 0 → "press A to reset" screen.
- [ ] Button A on the death screen resets the pet and plays the reset fanfare; next boot starts fresh.
- [ ] Power-cycle preserves stats: save → power off → power on → stats match the pre-power-off state.
- [ ] All four mood sprites (`NEUTRAL` / `HAPPY` / `UNWELL` / `HUNGRY`) render correctly when stats are nudged to each mood's trigger range (the hungry face shows when `fullness` is low).
- [ ] All six `ENABLE_*` build configurations compile and flash cleanly.

---

## Phase 2 DoD — Session 1 lesson plan

- [ ] `LESSON_PLANS/SESSION_01.md` exists in the minute-budgeted format.
- [ ] The lesson plan documents Session 1's `ENABLE_*` flag set (what is on, what is off).
- [ ] Building with Session 1's flag set compiles and flashes cleanly.
- [ ] On-device behaviour matches what the lesson plan promises students will see.
- [ ] A different teacher (not the author) could deliver Session 1 from the plan alone.
- [ ] Cross-ref: `DEV_ROADMAP.md`.

---

## Phase 3 DoD — `virtual-pet-learning-lab` stand-up

- [ ] Repo `virtual-pet-learning-lab` exists on GitHub (private initially, public once ready).
- [ ] `main` of the new repo equals the frozen `main` of this repo at Phase 1 completion.
- [ ] Branch `session-1-start` cut from `main` and matches the Session 1 day-start `ENABLE_*` configuration.
- [ ] All student / teacher docs present at root: `STUDENT_README.md`, `GIT_PRIMER.md`, `SPRITE_GUIDE.md`, `TEACHER_README.md`, `PROMOTION_GUIDE.md`, plus `LESSON_PLANS/SESSION_01.md`.
- [ ] On a fresh machine: `git clone` + `git checkout session-1-start` + `pio run --target upload` produces a working Session 1 day-start pet on hardware.
- [ ] Cross-ref: `DEV_ROADMAP.md`.

---

## Phase 4 DoD — Sessions 2–10 (per-session template)

Apply this checklist to **each** of Sessions 2 through 10. The session is done when every bullet passes.

- [ ] `LESSON_PLANS/SESSION_NN.md` exists in the minute-budgeted lesson-plan format (use SESSION_01 as the template).
- [ ] The plan documents the day-start `ENABLE_*` flag set and the new flags / features the session activates.
- [ ] Branch `session-N-start` is cut and compiles cleanly (Sessions 2–9 only; Session 10 is showcase + reflection, no code branch).
- [ ] On-device behaviour matches what the lesson plan promises students will see at session start.
- [ ] Cross-ref: `DEV_ROADMAP.md`.

Phase 4 in aggregate is done when Sessions 01 through 10 lesson plans all exist and `session-1-start` through `session-9-start` branches all compile and behave as their plans promise.

---

## Scope-creep guardrails

When tempted to add a feature, refactor, or polish pass that is not already on a task's DoD, run these three checks in order:

1. **Is the thing on the *Out of scope* list above?** → No, do not build.
2. **Is the thing on the current task's per-task DoD?** → No, file it as a separate task in `DEV_ROADMAP.md` Appendix B (Bonus) or as a future-work note. Do not work on it now.
3. **Will it move Phase 1 completion later than 2026-06-27?** → No, defer.

The 2026-06-27 deadline is the constraint that resolves every "while I'm in here, I could also…" thought. Cut, don't expand.

---

## See also

- `DEV_ROADMAP.md` — per-task design rationale and checklists
- `COURSE_CHECKLIST.md` — feature-status snapshot across the 10-session arc
- `LESSON_PLANS/SESSION_01.md` … `SESSION_10.md` — the minute-budgeted lesson plans
- `CLAUDE.md` — architecture map + next-task pointer
- `FLOW.md` — runtime control flow and pet state machine
