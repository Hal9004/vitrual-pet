# Session 3 — Motion

**Day-start flag state:** `ENABLE_ACTION_MENU` ON (from Session 2); all others OFF
**This session activates:** `ENABLE_IMU_PLAY` — the shake-to-play link. (It also introduces
the optional tilt-movement demo, a plain `true/false` switch in `main.cpp`.)
**One concept (takeaway sentence):** *"An `if` statement makes a decision — if something is
true, do something."*
**Where we are on the map:** you wake the **IMU** teammate (the motion sensor). Today's
workshop is `lib/Config/scaffold_config.h` (the switch), `lib/Imu/imu_manager.h` (how a
shake is detected), `src/main.cpp` (the `if` that reacts to a shake), and
`lib/Display/tilt_motion.h` (the tilt dials).

> Designed with the `lesson-design` skill. Session spine: **Reveal → Play → Learn → Build.**
> Everyone clears Reveal/Play/Learn; Build is optional.

---

## Recap from Session 2

*"A function is a named action you can call."* You called `play()` from the menu by pressing
A. Today: the pet plays when you **shake it** — and the `if` statement that *decides* when.

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

By the end of Session 3, every student can:
- Flip `ENABLE_IMU_PLAY` and rebuild, then **shake the device to make the pet play**.
- Explain what an **`if` statement** is, and point to `if (imu.wasShaken())` in `main.cpp`.
- Change how *easily* a shake triggers play by editing the shake sensitivity.
- Turn on the tilt demo and make the pet glide as they tilt the device.

The confident also: add a condition so the pet only plays when it has energy, or give the
shake an `if`/`else`.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Recap functions; "today we wake the motion sensor" | — |
| Presentation | 50' | Flip the flag (**Reveal**) → map update → intro the **`if` statement** | Reveal + Learn |
| Pair exploration | 20' | Tune shake sensitivity; turn on tilt-glide | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Make a shake feel right; play the "shake to cheer up" mini-game | Make it yours |
| Open creative | 25' | Optional code challenges (★/★★/★★★) | **Build** |
| Share-out + wrap | 15' | Demo your pet reacting to motion; "what I changed" | — |

---

## 1. Reveal — shake it to life *(in the presentation block)*

1. Open `lib/Config/scaffold_config.h` and **turn on `ENABLE_IMU_PLAY`**.
2. **Rebuild + flash.**
3. **The reveal:** **shake the device** — the pet plays and its happiness jumps. The motion
   sensor is now feeding into the game.

> The shake-to-play link is one line in `src/main.cpp`:
> `if (imu.wasShaken()) { myPet.play(); }` — that's today's whole concept in a nutshell.

## Navigation tour — *teach this before any tweaking*

Update the **"meet the team" map** (same diagram, new "you are here"):
- **Now awake:** **IMU** — the motion sensor that feels shakes and tilts.
- **Still asleep:** Speaker (sound), Storage (saving), the Stats screen, moods.

Keep the habits: **read the `.h` to use it**. Your **workshop** is the four files above.

---

## 2. Play — parameters *(pair exploration, editing values only)*

Everyone does these. Each is **one value, an obvious result, a one-line undo.**

- **Dial 1 — shake sensitivity.** In `lib/Imu/imu_manager.h`, find `SHAKE_THRESHOLD = 1.8f`.
  Lower it to `1.3f` → a gentle wiggle triggers play. Raise it to `2.5f` → you must shake
  hard. (A still device reads ~1.0; a shake spikes higher.)
- **Dial 2 — shake recovery.** In the same file, find `SHAKE_COOLDOWN_INTERVAL = 2000` (the
  wait between shakes, in ms). Lower it → you can shake-to-play again sooner.
- **Dial 3 — make it glide (the fun one).** In `src/main.cpp`, `TILT_MOVEMENT_ENABLED` turns
  the tilt demo on/off. With it `true`, tilt the device and the pet slides. Then in
  `lib/Display/tilt_motion.h`, change `TILT_SCALE = 60.0f` to `120.0f` → the pet slides
  **twice as far** for the same tilt. (Bonus: lower `SMOOTHING_FACTOR` for a calmer glide.)

Predict-then-flash: pairs guess how hard a shake will need to be before each change.

---

## 3. Learn — the fundamental: the **`if` statement**

**Takeaway:** *"An `if` statement makes a decision — if something is true, do something."*

**Taught by the teacher (short):** every moment, the program asks a yes/no question and acts
on the answer. In `main.cpp` the loop runs `if (imu.wasShaken()) { myPet.play(); }` — *"IF
the pet was shaken, THEN play."* `wasShaken()` hands back a yes or no (`true`/`false`); the
`if` only runs `play()` when the answer is yes. The sensitivity dial you just changed decides
*how easily* that answer becomes yes.

**Pointed to in the code (for the readers):**
- `if (imu.wasShaken()) { myPet.play(); }` in `src/main.cpp` (inside the `ENABLE_IMU_PLAY` block).
- `wasShaken()` in `lib/Imu/imu_manager.cpp` — it returns `true` or `false`, the yes/no the `if` checks.

Connect the pathway: Session 1 = values, Session 2 = functions (do a thing), Session 3 = an
`if` that **decides when** to call a function, based on whether something is true.

---

## 4. Make it yours — the shake-to-cheer mini-game *(guided task, everyone)*

Tune the shake until it feels right *to you* (sensitivity + cooldown), then play the loop:
let the pet's happiness fall, then shake it back up. Add the tilt-glide for personality.
Everyone leaves with a pet that reacts to motion the way they like.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** Tuning the shake and glide is a complete session. These extend today's feature
(the shake `if`) and step into real conditions.

- **★ Find your feel.** Settle on a `SHAKE_THRESHOLD` + `SHAKE_COOLDOWN_INTERVAL` pair that
  feels best, and write down *why* before flashing. *(Reinforces: the condition controls the if.)*
- **★★ Only play if there's energy *(combine two conditions)*.** In `main.cpp`, change the
  shake check to `if (imu.wasShaken() && myPet.getEnergised() > 10) { myPet.play(); }` so a
  shaking an exhausted pet does nothing. Worked twin: the existing `if (imu.wasShaken())`.
  *(First taste of combining conditions with `&&` and a comparison.)*
- **★★★ Add an `else` *(make the decision two-sided)*.** Give the shake an `if`/`else`: if the
  pet has energy it plays; **otherwise** it's too tired, so do something gentler instead (a
  tiny happiness bump, or nothing). Worked twin: the `if` you just edited. *(Introduces the
  `else` branch — the other half of a decision.)*

---

## Navigation note (what this session adds to the map)

- **Newly awake:** IMU (motion sensor).
- **Your workshop (editable today):** `lib/Config/scaffold_config.h`, `lib/Imu/imu_manager.h`,
  `src/main.cpp` (the shake `if` + the tilt switch), `lib/Display/tilt_motion.h`.
- **Still asleep / not for today:** Speaker, Storage, the Stats screen (`ENABLE_MULTISCREEN`),
  mood sprites.

---

## Author verification — *to complete when `session-3-start` is cut (Phase 3)*

Run on device with the day-start build (`ENABLE_ACTION_MENU` on, `ENABLE_IMU_PLAY` off), then
turn `ENABLE_IMU_PLAY` on:

- [ ] Day-start build compiles; shaking does nothing yet.
- [ ] Turning on `ENABLE_IMU_PLAY` and rebuilding makes a shake call `play()` (happiness jumps).
- [ ] Dial 1 (`SHAKE_THRESHOLD` 1.8→1.3) makes a gentle wiggle trigger play; 2.5 makes it hard.
- [ ] Dial 2 (`SHAKE_COOLDOWN_INTERVAL`) visibly changes how soon you can shake again.
- [ ] Dial 3: with `TILT_MOVEMENT_ENABLED` true the pet glides; `TILT_SCALE` 60→120 doubles the slide.
- [ ] ★★ energy condition stops play when exhausted; ★★★ `else` branch runs, inside the 25' block.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] Exactly ONE feature revealed (`ENABLE_IMU_PLAY`; tilt is a bonus on the same IMU teammate).
- [x] Exactly ONE fundamental concept (**`if` statement**), one sentence, building on functions.
- [x] A dial a struggling student clears fast with a visible result (shake sensitivity / tilt glide).
- [x] Challenges optional, ordered ★→★★★, all extending the shake `if` (condition → `&&` → `else`).
- [x] Concept taught both by teacher (yes/no decision) AND pointed to in code (`if (imu.wasShaken())`).
- [x] Navigation taught up front; map updated with "you are here"; workshop bounded to four files.
- [ ] Verified on device — pending the `session-3-start` branch (see above).
