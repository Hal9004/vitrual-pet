# Session 9 — Final Project (Debug, Finish, Prepare to Present)

**Day-start flag state:** **all `ENABLE_*` ON** — the complete pet, plus your own additions.
**This session activates:** *nothing new* — the **last code session**. You finish, fix, and
get ready to show your pet.
**One concept (takeaway sentence):** *"Debugging is detective work: reproduce the problem,
narrow down where it goes wrong, look at the actual values, fix one thing, and confirm."*
**Where we are on the map:** the whole pet. Today's new skill is seeing **inside** a running
program with the Serial monitor — and deciding what's ready to demo.

> **Final-project session (no flag/Reveal).** Shape: **Set the finish line → Learn-to-debug →
> Build (finish) → Rehearse.** The floor is "a working demo of the pet you have"; the ceiling
> is "your biggest feature, debugged and demo-ready."

---

## Recap from Sessions 7–8

You extended the pet and polished it. Today is about **crossing the finish line**: fix the
bugs, decide what's done, and prepare a short demo for the showcase (Session 10).

---

## Getting set up & saving your work *(every session)*

- **Start:** open your project from **your Google Drive** folder in **VS Code** — PlatformIO
  loads automatically and you pick up exactly where you left off last session.
- **Build & upload:** PlatformIO **✓ Build** → **→ Upload** to flash the device.
- **End:** save your whole project folder **back to your Google Drive** so next session
  continues from here.

*(Full steps: `LESSON_PLANS/WORKFLOW.md`.)*

---

## Learning goals

By the end of Session 9, every student can:
- Use a **debugging method** — reproduce → narrow down → inspect values → fix → confirm.
- Read **Serial output** to see a variable's real value while the pet runs.
- Decide a **demo path** — the exact sequence that shows their pet at its best.
- Walk away with **a working demo** (everyone clears this floor — the baseline pet always works).

The confident also: debug and land a ★★★ feature live, with a fallback if it fails.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | State your finish line: what *must* work for your demo | Set the finish line |
| Presentation | 50' | The **debugging method** + Serial monitor + finishing under a deadline | Learn |
| Pair exploration | 20' | **Dry-run your demo** for your partner; they note what breaks or confuses | Test |
| Break | 20' | — | — |
| Guided task | 75' | Debug and finish your must-work items; lock the demo path | **Build** |
| Open creative | 25' | Rehearse the presentation; one final flourish if time | Build |
| Share-out + wrap | 15' | Full dry-runs of a few demos | — |

---

## 1. Set the finish line *(opening block)*

Write down the **one or two things that must work** for your demo — not everything you wish
you'd built, just what you'll actually show. Everything else is optional from here. *(This is
finishing-under-a-deadline: choose the smallest set that makes a great demo.)*

---

## 2. The debugging method — the concept *(Learn)*

**Takeaway:** *"Debugging is detective work: reproduce it, narrow down where it goes wrong,
look at the actual values, fix one thing, and confirm."*

**Taught by the teacher (short):** when something's wrong, don't guess-and-poke. Instead:
1. **Reproduce** it reliably — what exact steps make it happen?
2. **Narrow down** — which teammate is involved? (Use the map.) Because you've changed one
   thing at a time, the suspect is usually your most recent change.
3. **Look at the actual values** — print them to the **Serial monitor** so you can *see* what
   the program really thinks, instead of imagining it.
4. **Fix one thing**, then **confirm** the bug is gone *and* nothing else broke.

**Pointed to in the code:** the project already debugs this way — `lib/Storage/storage_manager.cpp`
prints stat values inside `#ifdef DEBUG` blocks (`Serial.println(...)`), which is exactly how
you peek at a value. And the `FAST_TEST` switch in `lib/Timer/time_manager.cpp` lets you
**reproduce** a slow decay/death bug in seconds instead of minutes.

> The **Serial monitor** — your window into the running pet — isn't new: you met it in
> Session 5 to watch your pet save and reload. Today you use it to *debug*: add a
> `Serial.println(myValue);` where you're confused, and read what it actually prints.

---

## 3. Dry-run your demo *(pair exploration)*

Show your partner the demo you plan to give. They watch as an audience and note: what broke,
what was confusing, what they couldn't see. This surfaces both **bugs** and **demo problems**
(the thing you wanted to show didn't read clearly) while there's still time to fix them.

---

## 4. Build — finish and lock it down *(guided + open creative)*

Work your must-work list using the debugging method, then **lock your demo path** (the exact
buttons/shakes you'll do, in order).

- **★ Lock your demo.** Write the exact sequence that shows your pet at its best, and rehearse
  it until it's reliable. *(Everyone — the baseline pet guarantees you a demo even if a feature
  isn't done.)*
- **★★ Fix your top bug.** Pick the one bug that most hurts your demo and kill it with the
  method: reproduce → Serial-print the values → fix one thing → confirm.
- **★★★ Land a big feature live.** Get a ★★★ track (e.g. ESP-NOW pet-to-pet) working reliably
  enough to demo — and prepare a **fallback** (a screenshot, a backup step) in case the live
  attempt misbehaves on the day.

---

## Navigation note

- Still the whole pet — no new code area. The new "navigation" is the **Serial monitor**: a way
  to see *inside* the running program and read real values, instead of only what's on screen.

---

## Teacher notes

- **Everyone has a demo.** The baseline pet always works, so no student arrives at Session 10
  empty-handed — frame the finish line around "show the pet you have," then add their feature.
- **Triage bugs by demo impact.** Help students fix the bug that hurts the *demo* most, not the
  most interesting bug. Finishing is a skill.
- **Insist on a rehearsed demo path.** Live demos fail when improvised. A written, rehearsed
  button sequence (plus a fallback for ★★★ features) is the difference on showcase day.
- **Capture a demo plan.** Each student leaves with: their demo path, one thing they added, one
  thing that was hard, one thing they learned — the raw material for Session 10.

---

## Author verification — *to complete when `session-9-start` is cut (Phase 3)*

`session-9-start` is the full pet (same as Sessions 7–8). Before shipping the session:

- [ ] The full pet builds and runs.
- [ ] The Serial monitor shows `#ifdef DEBUG` output as described (confirm the `DEBUG` build path).
- [ ] `FAST_TEST` reliably reproduces a fast decay/death for debugging practice.
- [ ] A sample bug can be found end-to-end with the method (reproduce → Serial → fix → confirm) in the block.

---

## Pre-ship checklist (adapted from the lesson-design skill)

- [x] No new flag — final project; spine adapted (Set finish line → Learn → Build → Rehearse).
- [x] ONE concept (**systematic debugging**), one sentence, building on the change-one-thing habit (S7).
- [x] A guaranteed floor: every student has a working demo of the baseline pet (no empty hands).
- [x] Tracks ordered ★→★★★ (lock the demo → fix top bug → land a big feature live).
- [x] The concept is taught by the teacher AND grounded in the code (`#ifdef DEBUG` prints, `FAST_TEST`).
- [x] Navigation taught as the Serial monitor — seeing inside the running program.
- [ ] Verified on device — pending the `session-9-start` branch (see above).
