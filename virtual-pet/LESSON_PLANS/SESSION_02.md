# Session 2 — Stats, Menu & Interactions

**Day-start flag state:** all `ENABLE_*` OFF (carried from Session 1)
**This session activates:** `ENABLE_ACTION_MENU` — the **first real flag flip**.
**One concept (takeaway sentence):** *"A function is a named action you can call — and
you can write your own."*
**Where we are on the map:** you wake up two sleeping teammates — **Button** (reads your
presses) and **Actions** (the care menu) — and a new **Interact** screen appears. Today's
workshop is `lib/Config/scaffold_config.h` (the switch), `lib/Pet/pet.cpp` (the care
actions), and `lib/Actions/action_menu.cpp` (the menu).

---

## Recap from Session 1

*"A variable is a named box that holds a value."* You changed your pet's name and starting
stats. Today: the **actions that change those boxes for you** — and what we call them.

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

By the end of Session 2, every student can:
- Flip a feature flag (`ENABLE_ACTION_MENU`) and rebuild to **wake a feature up**.
- Use the menu — **B = next, C = previous, A = confirm** — to feed, play, sleep, bathe, heal.
- Explain what a **function** is, and point to `feed()`, `play()`, etc. in the code.
- Change *how much* an action helps by editing the values inside a function.

The confident also: add an extra effect to an action, or write a brand-new action and wire
it into the menu.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Recap variables; "today we wake a teammate" | — |
| Presentation | 50' | Flip the flag (**Reveal**) → map update → intro **functions** | Reveal + Learn |
| Pair exploration | 20' | Tweak how much each action helps | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Use the menu to keep the pet alive; rebalance the actions | Make it yours |
| Open creative | 25' | Optional code challenges (★/★★/★★★) | **Build** |
| Share-out + wrap | 15' | Show your rebalanced pet; "what I changed" | — |

---

## 1. Reveal — wake up the menu *(in the presentation block)*

This is the moment the flag model becomes real. Remember from Session 1: most of the team
was asleep. Today you wake one up.

1. Open `lib/Config/scaffold_config.h` and **turn on `ENABLE_ACTION_MENU`** (uncomment its
   `#define`).
2. **Rebuild + flash.**
3. **The reveal:** a small **Interact tab** now appears at the bottom-right of the Main
   screen. Press **B** to open the **Interact screen** — it shows the pet, a **stat bar for
   the action you currently have selected**, and that action's name along the bottom. **B / C**
   cycle through the actions (Feed / Play / Sleep / Bathe / Heal); **A** performs the selected
   one, and you watch its stat bar jump. (Each action highlights the one stat it changes.)

> **What else you'll notice:** now that you can care for the pet, you can also *neglect* it.
> If a stat hits a fatal level the pet dies and shows a "press A to reset" screen. That's the
> same decay from Session 1 — the menu is just your way to fight back against it.

> **Heads-up — Sickness is the odd stat out:** it starts at **0** and you *Heal it down*
> (low = healthy), while the other four stats start high and you keep them up. So when you
> cycle to **Heal**, its bar looks empty at first — that's normal, not a missing stat.

## Navigation tour — *teach this before any tweaking*

Update the **"meet the team" map** (same diagram, new "you are here"):

- **Now awake:** **Button** (reads A/B/C) and **Actions** (the care menu).
- **New screen:** **Interact** — where the menu lives.
- **Still asleep:** IMU (shake), Speaker (sound), Storage (saving), the Stats screen, moods.

Keep the habits: **read the `.h` to use it**, and your **workshop is three files** today —
`scaffold_config.h`, `pet.cpp`, `action_menu.cpp`. Everything else is "not for today."

---

## 2. Play — parameters *(pair exploration, editing values only)*

Everyone does these. Each is **one value inside an action, an obvious result, one-line undo.**
Open `lib/Pet/pet.cpp` and find the care actions:

- **Dial 1 — how much feeding helps.** In `feed()`, find `setFullness(fullness + 30);`. Change
  `30` to `60`. Reflash → feeding fills the pet far more in one press.
- **Dial 2 — how much playing cheers it up.** In `play()`, find `setHappy(happy + 25);`.
  Change `25` to `5` (barely helps) or `50` (huge boost) and feel the difference.
- **Dial 3 — how much sleep restores.** In `sleep()`, find `setEnergised(energised + 30);`.
  Change `30` and watch the energy bar jump by a different amount.

Predict-then-flash: have pairs predict which bar moves and by how much before reflashing.

---

## 3. Learn — the fundamental: **functions**

**Takeaway:** *"A function is a named action you can call — and you can write your own."*

**Taught by the teacher (short):** `feed()` is a function. Its **name** is `feed`, and the
lines inside it are the **steps that happen when you feed**. Pressing **A** on the Feed menu
item **calls** `feed()`, which runs those steps. The dials you just changed were the steps
inside the function — so a function is just *a named bundle of steps you can run by name.*

**You already met functions in Session 1** — every value you changed lived *inside* a
function (the decay rule `applyFullnessDecay()`, the care-action bodies). You were editing
function bodies without naming them. Session 2 names the idea and goes deeper: a function has
a **name**, a **body** of steps, and only runs when something **calls** it. (And from Session 1:
variables are the boxes; functions are the named actions that change them.)

**Pointed to in the code (for the readers):** in `lib/Pet/pet.cpp` —
- `feed()`, `play()`, `sleep()`, `bathe()`, `heal()` — five functions, each a named action.
- Inside each: a `setState(...)` line plus a few setter lines — *the body* (what it does).
- In `lib/Actions/action_menu.cpp`: the menu is what **calls** these functions when you press A.

Name the distinction once: **defining** a function (writing its steps) vs **calling** it
(running it by name). The menu calls; `pet.cpp` defines.

---

## 4. Make it yours — keep your pet thriving *(guided task, everyone)*

Use the menu to keep your pet alive and happy through a few minutes of real decay, then
**rebalance the actions to your taste** using the dials — make a forgiving "easy mode" pet
(actions restore a lot) or a demanding one. This is the everyone-succeeds creative task:
every student leaves with a pet tuned the way they like.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** Tweaking the dials and balancing your pet is a complete session. These extend
today's feature (the actions) and start with the smallest possible step into writing code.

- **★ Design a difficulty.** Change several action amounts at once to make a coherent "easy"
  or "hard" pet, and write down your design before flashing. *(Reinforces: a function's body
  is yours to shape.)*
- **★★ Add an effect to an action *(add one line)*.** In `feed()`, add a line so feeding also
  makes a small mess: `setCleanliness(cleanliness - 5);`. Worked twin: the lines already
  inside `feed()`. *(First real code-writing — add a line to a function.)*
- **★★★ Write a brand-new action *(write a function + wire it in)*.** Add a new function in
  `pet.cpp` — e.g. `cuddle()` — following `feed()` as the twin (`setState(...)` + a few
  setter lines). Then add it to the menu in `action_menu.cpp` (copy an existing menu entry).
  Worked twins: `feed()` and the existing menu entries. *(This touches two files in
  agreement — the first deliberate multi-file change.)*
- **★★★ Invent a whole new stat *(stretch — the big one)*.** Give the pet a stat it doesn't
  have yet — e.g. *boredom* or *thirst*. The pieces, each with a worked twin already in the
  code: add the field + getter/setter + a `DEFAULT_` in `pet.h`; add a decay rule in
  `time_manager.cpp`; write a care action that restores it (`feed()` is the twin) and add it to
  the menu; and show its bar. Worked twins: **every existing stat is built from exactly these
  pieces** — copy one end to end. *(Combines everything from Sessions 1–2; expect it to span
  into open-development later.)*

---

## Navigation note (what this session adds to the map)

- **Newly awake:** Button, Actions; **new screen:** Interact.
- **Your workshop (editable today):** `lib/Config/scaffold_config.h`, `lib/Pet/pet.cpp`,
  `lib/Actions/action_menu.cpp`.
- **Still asleep / not for today:** IMU, Speaker, Storage, the Stats screen (`ENABLE_MULTISCREEN`),
  mood sprites. They're on the map but off-limits this session.

---

## Author verification — *to complete when `session-2-start` is cut (Phase 3)*

Run on device with the day-start (all-OFF) build, then with `ENABLE_ACTION_MENU` on:

- [ ] Day-start build (flag OFF) compiles and flashes; no menu present.
- [ ] Turning on `ENABLE_ACTION_MENU` and rebuilding makes the Interact screen + menu appear.
- [ ] B / C cycle the menu items; A confirms and visibly changes the right stat bars.
- [ ] Dial 1 (`feed()` 30→60) makes feeding visibly fill more in one press.
- [ ] Dials 2 and 3 visibly change play/sleep effects.
- [ ] ★★ extra line (`setCleanliness(cleanliness - 5)` in `feed()`) compiles and works.
- [ ] ★★★ new action appears in the menu and runs, inside the 25' block.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] Exactly ONE feature revealed (`ENABLE_ACTION_MENU`).
- [x] Exactly ONE fundamental concept (**functions**), one sentence, building on variables.
- [x] A dial a struggling student clears fast with a visible result (action amounts).
- [x] Challenges optional, ordered ★→★★★, all extending the actions feature.
- [x] Concept taught both by teacher (named bundle of steps) AND pointed to in code (`feed()` …).
- [x] Navigation taught up front; map updated with "you are here"; workshop bounded to three files.
- [ ] Verified on device — pending the `session-2-start` branch (see above).
