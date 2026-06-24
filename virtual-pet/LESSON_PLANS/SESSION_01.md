# Session 1 — First Pet on Screen

**Day-start flag state:** all `ENABLE_*` OFF (the cleaned baseline)
**This session activates:** *none* — Session 1 **is** the baseline. The feature-flag
model is introduced and previewed today, then first used in Session 2.
**One concept (takeaway sentence):** *"A variable is a named box that holds a value —
change the value, change how the pet behaves."*
**Where we are on the map:** you boot the project for the very first time, meet the
team that runs the pet, and work in **two files only** — `lib/Pet/pet.cpp` (the pet's
starting values + its name) and `lib/Timer/time_manager.cpp` (how fast a stat changes).
---

## Learning goals

By the end of Session 1, every student can:
- Download the starter project from **Google Drive**, open it in **VS Code**, and build + upload it to the device with **PlatformIO**.
- Point to where their pet's **name** and **starting stats** live, and change them.
- Explain in one sentence what a **variable** is, using the pet as the example.
- Draw their own sprite in Piskel and see *their* pet alive on screen.

The confident also: change *how fast* a stat decays, and (stretch) add a brand-new
automatic stat change by copying an existing one.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Welcome, the 10-session big picture, pair up, hand out devices | — |
| Presentation | 50' | Get the project (Drive → VS Code) + first build/upload (**Reveal**) → navigation tour → intro **variables** | Reveal + Learn |
| Pair exploration | 20' | Tweak the dials: name, starting stats, decay speed | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Draw your own sprite in Piskel → see *your* pet; set its name | Make it yours |
| Open creative | 25' | Optional code challenges (★/★★/★★★) — or keep polishing | **Build** |
| Share-out + wrap | 15' | Show your pet + name; "what I changed" | — |

---

## 1. Reveal — your pet appears *(in the presentation block)*

There is no switch to flip yet — today you meet the pet in its simplest form, and learn
the tools you'll use every session.

1. **Get the project.** Download the starter project from the class **Google Drive**
   folder and open the folder in **VS Code**. The **PlatformIO** extension loads
   automatically — that's the tool that builds the code and talks to the device.
   *(Full step-by-step: `LESSON_PLANS/WORKFLOW.md`.)*
2. **Build + upload.** Press PlatformIO's **✓ Build**, then **→ Upload**, to flash it
   onto the M5StickC.
3. **The reveal:** an empty device → *your pet, alive on screen,* with its fullness bar
   slowly draining. That build-and-watch-it-appear moment is the hook.

> **Preview the flag model (sets up Session 2):** "Most of the team that runs this pet
> is asleep right now. In future sessions you'll flip a switch — like `ENABLE_SOUND` —
> rebuild, and a new ability wakes up. Today you meet the sleeping baseline."

## Navigation tour — *teach this before any tweaking*

Show the **"meet the team" map** (one diagram, reused every session, with **"you are here"**):

- **Pet** — the creature itself; holds how it feels (its stats).
- **Timer** — the clock that makes it get hungry over time.
- **Display** — the artist that draws it.
- *(asleep today: Button, IMU, Speaker, Storage, Actions — we'll meet them later.)*
- **main.cpp** — *you, the boss*, telling each teammate when to do its job.

Two habits to install now:
- **"Read the `.h` to *use* something; you don't need the `.cpp`."** The `.h` is the short
  list of what a teammate can do.
- **Read-only engine vs. your workshop.** Today your workshop is exactly two files:
  `lib/Pet/pet.cpp` and `lib/Timer/time_manager.cpp`. Everything else is "not for today" —
  you can look, but you can't break it.

---

## 2. Play — parameters *(pair exploration, no code-writing)*

Everyone does these. Each is **one value, an obvious result, and a one-line undo.**

- **Dial 1 — your pet's name.** In `lib/Pet/pet.cpp`, find `petName = "Pixel";` and change
  `"Pixel"` to your own name. Reflash → the title shows your name.
- **Dial 2 — how it starts.** In `lib/Pet/pet.h`, the `DEFAULT_*` values are the pet's
  starting stats. Change `DEFAULT_FULLNESS` from `80` to `10`. Reflash → your pet starts
  *almost starving* (bar nearly empty). Try `100` → it starts completely full.
- **Dial 3 — how fast it gets hungry.** In `lib/Timer/time_manager.cpp`, find
  `FULLNESS_DECAY_INTERVAL` (it's `6000` — meaning "−1 fullness every 6000 ms"). Change it
  to `2000` → fullness drops much faster.
  - *Teacher tip:* the real pet takes ~8 minutes to starve — too slow to watch.
    Uncomment `#define FAST_TEST` at the top of `time_manager.cpp` so changes are visible
    in seconds, and re-comment it before committing.

Predict-then-flash: have pairs say what they *think* will happen before reflashing.

---

## 3. Learn — the fundamental: **variables**

**Takeaway:** *"A variable is a named box that holds a value — change the value, change
how the pet behaves."*

**Taught by the teacher (short):** the box metaphor. `petName` is a box holding text;
`DEFAULT_FULLNESS` is a box holding a number; `FULLNESS_DECAY_INTERVAL` is a box holding
how-many-milliseconds. You just changed what's in the boxes and the pet changed with it —
*that's all a variable is.*

**Pointed to in the code (for the readers):**
- `petName = "Pixel";` in `lib/Pet/pet.cpp` — a text variable.
- the `DEFAULT_*` constants in `lib/Pet/pet.h` — number variables (starting stats).
- `FULLNESS_DECAY_INTERVAL` in `lib/Timer/time_manager.cpp` — a number variable that
  controls timing.

Tie it back: every dial in the Play block was just "change the value in a box."

---

## 4. Make it yours — your own sprite *(guided task, everyone)*

The creative centrepiece. Follow **`SPRITE_GUIDE.md`** (the asset pipeline):
1. Draw an 80×80 sprite in Piskel.
2. **Export it from Piskel and save the file to your Google Drive** so your art is never lost.
3. Run the converter to turn the image into a C++ array.
4. Add the array file into `lib/Display/sprites/`, point the code at it, and **Upload** → *your* pet is on screen.
5. Set its name (Dial 1) to match.

This is the "make it yours" moment — everyone leaves with a pet that is theirs.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** If you only tweaked dials and drew a sprite, you've had a full session.
These extend today's feature (the stat/decay system) and stay in your two workshop files.

- **★ Predict the bars.** Change three different `DEFAULT_*` stats at once and, *before*
  flashing, write down how full each bar will be. Flash and check. *(Reinforces:
  many independent variables.)*
- **★★ Change the size of the drop, not just the speed.** In `time_manager.cpp`, find
  `FULLNESS_DECAY_AMOUNT` (it's `1`). Make it `5` → fullness now drops 5 at a time. Reason
  about the difference between *how much* (amount) and *how often* (interval).
- **★★★ Add a brand-new automatic change *(write code)*.** `time_manager.cpp` already
  contains a worked twin: `applyFullnessDecay()`, and a comment that says *"to add a new
  automatic stat change, add a method and call it in `update()`."* Copy that method,
  rename it, point it at a different stat, and call it from `update()`. Reflash and watch
  your new rule run. *(This is a first taste of writing a function — Session 2's concept.)*

---

## Save your work — every session ends here

Before you leave, **save your whole project folder back to your Google Drive**, along
with your sprite file. Next session you'll re-open it from Drive in VS Code and carry on
exactly where you stopped. *(This is the routine for every session from here on; full
steps in `LESSON_PLANS/WORKFLOW.md`.)*

---

## Navigation note (what this session adds to the map)

- **Introduced and "awake":** Pet, Timer, Display, main.cpp.
- **Your workshop (editable today):** `lib/Pet/pet.cpp`, `lib/Pet/pet.h`,
  `lib/Timer/time_manager.cpp`, plus your new sprite file.
- **Asleep / not for today:** Button, IMU, Speaker, Storage, Actions, Navigation, the
  rest of Display. They exist on the map but are explicitly off-limits this session.

---

## Author verification — *to complete when `session-1-start` is cut (Phase 3)*

This plan is drafted ahead of the `session-1-start` branch. Before the session ships,
run these on the device with the all-`ENABLE_*`-OFF build:

- [ ] All flags OFF compiles and flashes; the pet appears on screen with its fullness bar.
- [ ] Dial 1 (`petName`) changes the on-screen title.
- [ ] Dial 2 (`DEFAULT_FULLNESS` 80→10) makes the pet start near-starving — obvious on flash.
- [ ] Dial 3 (`FULLNESS_DECAY_INTERVAL`) visibly changes decay speed (use `FAST_TEST`).
- [ ] The Piskel → converter → flash pipeline produces a student sprite on screen.
- [ ] The ★★★ challenge (copy `applyFullnessDecay`) compiles and runs inside the 25' block.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] Exactly ONE feature (the baseline) — no flag flipped; flag model previewed for S2.
- [x] Exactly ONE fundamental concept (**variables**), statable in one sentence.
- [x] A dial a struggling student clears fast with a visible result (the name / starting stat).
- [x] Challenges optional, ordered ★→★★★, all extending the stat/decay feature.
- [x] Concept taught both by teacher (box metaphor) AND pointed to in code (`DEFAULT_*`, `petName`).
- [x] Navigation taught up front; map shows "you are here"; workshop bounded to two files.
- [ ] Verified on device — pending the `session-1-start` branch (see above).
