# Session 6 — Screens & Mood

**Day-start flag state:** `ENABLE_ACTION_MENU` + `ENABLE_IMU_PLAY` + `ENABLE_SOUND` + `ENABLE_PERSISTENCE` ON (from Sessions 2–5)
**This session activates:** `ENABLE_MOOD_SPRITES` + `ENABLE_MULTISCREEN` — the final two
switches. After today **the whole pet is awake.**
**One concept (takeaway sentence):** *"An `if` / `else if` ladder checks conditions in order
and takes the first one that's true — so the order decides the winner."*
**Where we are on the map:** the **Display** teammate's last two jobs wake up — the pet's
**face changes with its mood**, and a third **Stats** screen appears. Today's workshop is
`lib/Config/scaffold_config.h` (the switches), `lib/Pet/pet.cpp` (`computeMood()` — the
ladder), `lib/Display/screen_layout.h` (the mood list), and `lib/Display/display_manager.cpp`
(which picture goes with each mood). This is the **last feature session** —
> it completes the pet and the fundamentals pathway.

---

## Recap from Session 5

*"A program can save its values so they survive a power-off."* Your pet now remembers itself.
Today it learns to **show how it feels** — and you'll meet the decision structure behind it: a
**ladder of `if`s**.

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

By the end of Session 6, every student can:
- Flip `ENABLE_MOOD_SPRITES` + `ENABLE_MULTISCREEN` and rebuild, then watch the **pet's face
  change** with its stats and find the new **Stats** screen.
- Explain an **`if` / `else if` ladder**: conditions checked in order, first match wins.
- Change a **mood threshold** and the **priority order**, and predict the result.

The confident also: **add a brand-new mood** (the famous "update three places"), bringing the
sprite pipeline from Session 1 full circle.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Recap save/load; "today the pet shows its feelings" | — |
| Presentation | 50' | Flip the flags (**Reveal**) → map update → intro the **`if`-ladder** | Reveal + Learn |
| Pair exploration | 20' | Change mood thresholds and the priority order | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Tune your pet's moods; (confident) add a new mood | Make it yours / Build |
| Open creative | 25' | Optional code challenges (★/★★/★★★) | **Build** |
| Share-out + wrap | 15' | Show every mood your pet can wear; "what I changed" | — |

---

## 1. Reveal — the pet shows how it feels *(in the presentation block)*

1. Open `lib/Config/scaffold_config.h` and **turn on `ENABLE_MOOD_SPRITES` and
   `ENABLE_MULTISCREEN`** (the last two switches).
2. **Rebuild + flash.**
3. **The reveal:** the pet's **face now changes** — happy when content, a hungry face when
   starving, an unwell face when sick. And a new **Stats** screen appears (cycle to it) showing
   all the bars plus the **mood word**. Every teammate is now awake — this is the finished pet.

> **Why two switches today?** Mood sprites are the star (and today's concept); the Stats screen
> is the companion that shows the matching mood word. The curriculum turns them on together so
> the pet is complete by the end of the feature sessions.

## Navigation tour — *teach this before any tweaking*

Update the **"meet the team" map** (same diagram, new "you are here"):
- **Now awake:** the rest of **Display** — the mood face + the **Stats** screen (the third screen).
- **The whole team is awake.** Nothing is asleep anymore — this is the full pet.

Keep the habits: **read the `.h` to use it**. Today the concept lives mainly in one short
function: `computeMood()`.

---

## 2. Play — parameters *(pair exploration, editing values only)*

Everyone does these — all inside `computeMood()` in `lib/Pet/pet.cpp`. Each is **one change,
an obvious result, a one-line undo.**

- **Dial 1 — look happy more easily.** Find `if (happy > 70)`. Change `70` to `40` → the pet
  wears its happy face far more of the time.
- **Dial 2 — look hungry sooner.** Find `if (fullness < 30)`. Change `30` to `60` → the hungry
  face shows up sooner, while there's still food left.
- **Dial 3 — change who wins (reorder the ladder).** Move the `if (happy > 70)` block **above**
  the `if (fullness < 30)` block. Now when the pet is *both* hungry and happy, it shows **happy**
  instead of hungry — because the first matching rule wins.

Predict-then-flash: nudge a stat to the edge of a rule and guess which face appears first.

---

## 3. Learn — the fundamental: the **`if` / `else if` ladder**

**Takeaway:** *"An `if`-ladder checks conditions in order and takes the first one that's true —
so the order decides the winner."*

**Taught by the teacher (short):** `computeMood()` is a ladder. It checks the worst problems
first: *if* sick is high → unwell (stop here); *else if* very hungry → hungry (stop); *else if*
very happy → happy (stop); otherwise → neutral. Only **one** face ever shows, because the first
rule that's true returns immediately and the rest never run. The **thresholds** (Dials 1–2)
decide *when* each rule is true; the **order** (Dial 3) decides who wins when two are true.

**Pointed to in the code (for the readers):** `computeMood()` in `lib/Pet/pet.cpp` — the stack
of `if (...) return MOOD_...;` lines. The comment above it even says how to add a new rung.

Connect the pathway: Session 3 was a single `if` (one decision). Session 6 is a **ladder** of
them — where order matters and the first match wins. *(This completes the floor pathway:
variables → functions → `if` → lists+loops → save/load → `if`-ladder.)*

---

## 4. Make it yours — design your pet's feelings *(guided task, everyone)*

Using the dials, **tune when each mood appears and which one wins** so your pet's personality
matches how you want it to behave — quick to cheer up, dramatic when hungry, whatever you like.
Everyone leaves with a pet whose face is theirs. Confident students start the "add a mood"
challenge here.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** Tuning thresholds and order is a complete session. These extend today's feature
(the mood ladder) — and ★★ is the signature challenge this whole system was designed around.

- **★ Compose a mood profile.** Pick thresholds + order that give your pet a clear personality,
  and write down the design before flashing. *(Reinforces: the ladder's conditions and order.)*
- **★★ Add a brand-new mood — `MOOD_SAD` *(update three places)*.** The pet currently shows no
  special face when its happiness drops toward zero. Fix that:
  1. add `MOOD_SAD` to the list in `lib/Display/screen_layout.h`,
  2. add a rung in `computeMood()` (e.g. `if (happy < 20) return MOOD_SAD;`),
  3. add a `case` for it in `spriteForMood()` in `lib/Display/display_manager.cpp`.
  Worked twin: every existing mood already appears in those same three places. *(This is the
  deliberate three-file capstone — the system was built with this exact gap to fill.)*
- **★★★ A new mood with your own art *(full loop back to Session 1)*.** Add `MOOD_TIRED` for low
  energy the same three-places way, then **draw its face in Piskel** and run it through the
  sprite pipeline from Session 1 so the new mood has its own picture. *(Combines today's ladder
  with the very first thing you learned.)*

---

## Navigation note (what this session adds to the map)

- **Newly awake:** the mood face + the **Stats** screen — the rest of Display. **The whole team
  is now awake** (the finished pet).
- **Your workshop (editable today):** `lib/Config/scaffold_config.h`, `lib/Pet/pet.cpp`
  (`computeMood()`), `lib/Display/screen_layout.h`, `lib/Display/display_manager.cpp` (`spriteForMood()`).
- **Asleep / not for today:** nothing — this is the complete pet. From here (Sessions 7+) you
  extend it in your own direction.

---

## Author verification — *to complete when `session-6-start` is cut (Phase 3)*

Run on device with the day-start build (S2–5 flags on, mood/multiscreen off), then turn both on:

- [ ] Day-start build compiles; the pet has one face and two screens (Main + Interact).
- [ ] Turning on both flags adds the changing face and the Stats screen.
- [ ] Dial 1 (`happy > 70`→`40`) shows the happy face more readily.
- [ ] Dial 2 (`fullness < 30`→`60`) shows the hungry face sooner.
- [ ] Dial 3 (reorder happy above hungry) flips which face wins when both are true.
- [ ] ★★ `MOOD_SAD` shows when happiness is low; ★★★ `MOOD_TIRED` shows with its own sprite.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] ONE concept (the **`if`-ladder**) — kept singular even though two flags flip (see Reveal note).
- [x] Concept builds on the single `if` from Session 3; completes the fundamentals pathway.
- [x] A dial a struggling student clears fast with a visible result (a mood threshold).
- [x] Challenges optional, ordered ★→★★★, all extending the mood ladder (tune → add mood → add mood + art).
- [x] Concept taught both by teacher (worst-first ladder) AND pointed to in code (`computeMood()`).
- [x] Navigation taught up front; map updated with "you are here"; workshop bounded to four files.
- [ ] Verified on device — pending the `session-6-start` branch (see above).

> **Author note (two flags):** per the curriculum arc, Session 6 activates `ENABLE_MOOD_SPRITES`
> and `ENABLE_MULTISCREEN` together. The concept is kept singular by making mood sprites the
> vehicle and the Stats screen the companion display. If a cohort needs a gentler pace, this
> session can be split — moods first, the Stats screen second — without changing the concept.
