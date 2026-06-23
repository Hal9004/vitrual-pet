# Session 4 — Sound

**Day-start flag state:** `ENABLE_ACTION_MENU` + `ENABLE_IMU_PLAY` ON (from Sessions 2–3); others OFF
**This session activates:** `ENABLE_SOUND`
**One concept (takeaway sentence):** *"A list (array) lets you handle many items one at a
time — and a loop does the same thing to each one without repeating yourself."*
**Where we are on the map:** you wake the **Speaker** teammate. Today's workshop is
`lib/Config/scaffold_config.h` (the switch) and `lib/Speaker/speaker_manager.cpp` (the melodies).

> Designed with the `lesson-design` skill. Session spine: **Reveal → Play → Learn → Build.**
> Everyone clears Reveal/Play/Learn; Build is optional.

---

## Recap from Session 3

*"An `if` statement makes a decision."* You made the pet react to a shake. Today the pet
gets a **voice** — and you'll meet the idea behind every melody: a **list of notes**.

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

By the end of Session 4, every student can:
- Flip `ENABLE_SOUND` and rebuild, then **hear the pet** — each action plays a melody and
  hunger/sickness/death have their own alerts.
- Change the **volume**, a note's **pitch**, and a note's **length**.
- Explain what a **list (array)** is and why a **loop** saves you repeating yourself,
  pointing at the repeated `tone()` lines in a melody.

The confident also: add notes to a melody, or rewrite a melody using an array + a loop.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Recap `if`; "today the pet gets a voice" | — |
| Presentation | 50' | Flip the flag (**Reveal**) → map update → intro **lists + loops** | Reveal + Learn |
| Pair exploration | 20' | Change volume, pitch, note length | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Compose your pet's signature feed tune | Make it yours |
| Open creative | 25' | Optional code challenges (★/★★/★★★) | **Build** |
| Share-out + wrap | 15' | Play your pet's tune for the room; "what I changed" | — |

---

## 1. Reveal — give the pet a voice *(in the presentation block)*

1. Open `lib/Config/scaffold_config.h` and **turn on `ENABLE_SOUND`**.
2. **Rebuild + flash.**
3. **The reveal:** now every action sings — feed plays a happy two-note rise, play is bouncy,
   sleep is a lullaby — and when the pet gets too hungry or sick it **beeps a warning**.

## Navigation tour — *teach this before any tweaking*

Update the **"meet the team" map** (same diagram, new "you are here"):
- **Now awake:** **Speaker** — the pet's voice (the built-in buzzer).
- **Still asleep:** Storage (saving), the Stats screen, moods.

Keep the habits: **read the `.h` to use it**. Your **workshop is two files** today.

---

## 2. Play — parameters *(pair exploration, editing values only)*

Everyone does these. Each is **one value, an obvious result, a one-line undo.** Open
`lib/Speaker/speaker_manager.cpp`:

- **Dial 1 — volume.** In `init()`, find `M5.Speaker.setVolume(128);`. Try `50` (quiet) or
  `200` (loud). Reflash → the whole pet gets quieter/louder.
- **Dial 2 — a note's pitch.** In `playFeedSound()`, find `M5.Speaker.tone(523, 120);` (that
  `523` is the note C5). Change `523` to `880` → feeding now beeps high and bright. Bigger
  number = higher note.
- **Dial 3 — a note's length.** In the same line, `120` is how long the note lasts (ms).
  Make it `400` for a long held note, or `60` for a quick blip.

Predict-then-flash: pairs hum what they think the change will sound like first.

---

## 3. Learn — the fundamental: **lists (arrays) + loops**

**Takeaway:** *"A list (array) lets you handle many items one at a time — and a loop does the
same thing to each one without repeating yourself."*

**Taught by the teacher (short):** look closely at a melody like `playPlaySound()` — it's the
**same two lines repeated for every note**: `tone(...)`, then `delay(...)`, over and over. A
melody is really just a **list of notes** written out the long way. The lesson's big idea:
that list could live in one place (an **array**), and a **loop** could play each note in turn
— so you'd never copy-paste those two lines again. *(The code today shows the long way on
purpose; the short way is the challenge below.)*

**Pointed to in the code (for the readers):** in `lib/Speaker/speaker_manager.cpp`, every
`playXxxSound()` is the repeated `tone(frequency, duration)` / `delay(...)` pattern. Count the
repeats — that repetition is exactly what a loop removes.

Connect the pathway: values (S1) → functions (S2) → decisions with `if` (S3) → and now a
**list** of values plus a **loop** to walk through them.

---

## 4. Make it yours — your pet's signature tune *(guided task, everyone)*

Using only the dials, **compose your pet's feed melody**: pick the notes (pitches) and their
lengths in `playFeedSound()` until it's a little tune you like. Everyone leaves with a pet
that sounds uniquely theirs — no new code required.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** Composing a tune with the dials is a complete session. These extend today's
feature (the melodies) and lead straight into arrays + loops.

- **★ Remix a melody.** Rewrite all the notes in one `playXxxSound()` into your own short
  tune. *(Reinforces: a melody is just a list of notes.)*
- **★★ Add notes to a melody *(extend the sequence)*.** Copy the `tone(...)` / `delay(...)`
  pair to add a 4th and 5th note. Worked twin: the existing notes in the same function. *(You
  will feel the copy-paste repetition first-hand — that's the problem loops solve.)*
- **★★★ Rewrite a melody with an array + a loop *(write code)*.** Put the note frequencies in
  an array and the durations in another, then use a `for` loop to play each note in turn.
  Worked twin: the verbose melody you just edited (it's the same notes, looped). *(This is the
  real payoff — the short way that replaces all that repetition.)*

---

## Navigation note (what this session adds to the map)

- **Newly awake:** Speaker (the pet's voice).
- **Your workshop (editable today):** `lib/Config/scaffold_config.h`, `lib/Speaker/speaker_manager.cpp`.
- **Still asleep / not for today:** Storage, the Stats screen (`ENABLE_MULTISCREEN`), mood sprites.

---

## Author verification — *to complete when `session-4-start` is cut (Phase 3)*

Run on device with the day-start build (`ENABLE_ACTION_MENU` + `ENABLE_IMU_PLAY` on,
`ENABLE_SOUND` off), then turn `ENABLE_SOUND` on:

- [ ] Day-start build compiles; the pet is silent.
- [ ] Turning on `ENABLE_SOUND` and rebuilding makes actions play melodies and alerts beep.
- [ ] Dial 1 (`setVolume` 128→50/200) audibly changes volume.
- [ ] Dial 2 (`523`→`880` in `playFeedSound`) raises the feed note's pitch.
- [ ] Dial 3 (note duration) changes how long a note holds.
- [ ] ★★ extra notes play; ★★★ array + `for` loop reproduces the melody, inside the 25' block.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] Exactly ONE feature revealed (`ENABLE_SOUND`).
- [x] Exactly ONE fundamental concept (**lists + loops**), one sentence, building on `if`.
- [x] A dial a struggling student clears fast with a visible/audible result (volume / pitch).
- [x] Challenges optional, ordered ★→★★★, all extending the melodies (remix → add notes → loop).
- [x] Concept taught both by teacher (the repeated `tone()` lines) AND pointed to in code.
- [x] Navigation taught up front; map updated with "you are here"; workshop bounded to two files.
- [ ] Verified on device — pending the `session-4-start` branch (see above).

> **Author note (honesty):** the melodies are intentionally long-hand in the reference code,
> so this session *teaches* arrays + loops via the repetition rather than pointing at an
> existing loop. The ★★★ challenge is the project's deferred "melody refactor" exercise.
