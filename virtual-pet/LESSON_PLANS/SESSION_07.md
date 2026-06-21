# Session 7 — Open Development (Make It Yours)

**Day-start flag state:** **all `ENABLE_*` ON** — the complete pet from Session 6.
**This session activates:** *nothing new* — open development begins. There is no flag to
flip; **you** decide what the pet does next.
**One concept (takeaway sentence):** *"To extend real code: pick one small goal, find a
worked example already in the project, change one thing at a time, and test after every change."*
**Where we are on the map:** the whole team is awake. Today's skill isn't a new area — it's
learning to **navigate to wherever your chosen feature lives** and change it safely.

> Designed with the `lesson-design` skill — **adapted** for an open-development session.
> The spine inverts: there's no single Reveal/flag, so the shape is
> **Survey → Choose → Learn-the-process → Build.** The floor is "ship one small change of your
> own"; the ceiling is a multi-session feature.

---

## Recap from Sessions 1–6

You built the fundamentals: **variables → functions → `if` → lists + loops → save/load →
`if`-ladders.** The pet is finished and every teammate is awake. From here, the project is
yours to extend — and you already have every skill you need to do it.

---

## Learning goals

By the end of Session 7, every student can:
- **Choose** an extension that fits their confidence and the time available.
- **Find** where their chosen feature lives in the code (the navigation recipe below).
- Make **one small change at a time** and test each step on the device.
- Ship **at least one visible change of their own** (everyone clears this floor).

The confident also: begin a larger, multi-session track (continued in Session 8).

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Celebrate the finished pet; tour the **track menu** | Survey |
| Presentation | 50' | The **extend-real-code recipe** + the **find-where-it-lives** navigation method | Learn |
| Pair exploration | 20' | Pick a track; write down your *smallest first visible step* | Choose |
| Break | 20' | — | — |
| Guided task | 75' | Build your extension, one change at a time | **Build** |
| Open creative | 25' | Push further, or start a second track | Build |
| Share-out + wrap | 15' | Demo what you added; "what was your first step?" | — |

---

## 1. Survey — what could your pet do next? *(opening block)*

There's no switch today. Instead, look at the finished pet and pick something to add. The
**track menu** below is the starting point — but a student's own idea is just as welcome, as
long as it's scoped small enough to start today.

## The find-where-it-lives recipe *(teach this in the presentation)*

The one navigation skill that makes open development possible — go from *what you see* to
*where to edit*:
1. **Name what you see** (a sound, a face, an action, a bar).
2. **Find the teammate** responsible on the map (sounds → Speaker, face → Display, care →
   Pet/Actions, timing → Timer, saving → Storage).
3. **Open that teammate's `.h`** to see what it can already do.
4. **Open its `.cpp`** and find the **worked example** closest to your goal — copy it, then
   change one thing.
5. **Test after every single change.** If it breaks, you changed one thing, so you know what.

---

## 2. The extend-real-code recipe — the concept *(Learn)*

**Takeaway:** *"Pick one small goal, find a worked example, change one thing at a time, test
after every change."*

**Taught by the teacher (short):** professionals rarely write from a blank page — they find
something similar that already works and adapt it. Every track below names its **worked twin**
(the existing code to copy). The discipline that keeps you out of trouble: change *one* thing,
flash, check — then the next. This is the most real-world skill in the whole course, and it
uses everything you've already learned.

**Pointed to in the code:** every feature you'll extend has a twin already in the repo — the
five care actions, the mood ladder, the melodies, the save/load pairs. You've read them all.

---

## 3. The track menu *(Choose, then Build)*

Pick **one** to start. Each lists its goal, where it lives, and its worked twin. Everyone
should clear at least a ★; ★★★ tracks may span into Session 8.

### ★ Remix what exists (guaranteed first win)
- **New care action** (e.g. *cuddle*, *scold*). Files: `lib/Pet/pet.cpp` + `lib/Actions/action_menu.cpp`. Twin: `feed()` and the existing menu entries.
- **New mood face** (`MOOD_SAD` / `MOOD_TIRED`). Files: `screen_layout.h` + `pet.cpp` (`computeMood`) + `display_manager.cpp` (`spriteForMood`). Twin: any existing mood (it lives in all three).
- **Your own melodies.** File: `lib/Speaker/speaker_manager.cpp`. Twin: any `playXxxSound()`.
- **Rebalance the whole game.** Files: `lib/Timer/time_manager.cpp` (decay) + the action amounts in `pet.cpp`. Twin: the existing constants.

### ★★ Add something new
- **Idle animation** — a gentle `millis()`-driven bob. Files: `lib/Display/tilt_motion.*` (sum a sine offset) + `drawPetSprite()`. Twin: the existing tilt offset (the bob is the same idea on a timer). *(See `IDEAS.md`.)*
- **Animated mood sprites** — 2-frame faces. Files: `lib/Display/animation_manager.cpp` (frame count) + new Piskel frames. Twin: the existing single-frame moods.
- **A persisted counter / high score** (e.g. *times fed*, *longest survival*). Files: `pet` + `lib/Storage/storage_manager.cpp`. Twin: the seven existing `putInt`/`getInt` pairs.

### ★★★ Ambitious (often spans Session 8; uses the design notes in `DEV_ROADMAP.md` Appendix B)
- **Pet-to-pet greetings (ESP-NOW)** — shake to send a happiness gift to a friend's pet.
  **Needs two devices.** New `lib/Wireless/peer_link.*` from the Bonus 3 design. The biggest
  "wow," the most new ground (MAC addresses, sending data between devices).
- **Evolution / growth stages** — the pet's sprite changes as it ages well. (Bonus / design note.)
- **Web dashboard** — the pet broadcasts WiFi and serves a stats page. (Bonus 2.)

> **Floor guarantee:** a student who is unsure should take a ★ track — a worked twin exists,
> so it's a copy-and-change-one-thing task. Nobody faces a blank page.

---

## 4. Make it yours — build it *(guided + open creative)*

Pick your track, write your *smallest first visible step* on paper, then build it one change
at a time, testing each on the device. Teacher circulates, pointing students to worked twins
and helping scope ambitions down to a first step. Everyone ships at least one change of their
own by share-out.

---

## Navigation note

- **The whole team is awake** — no new area. The new skill is the **find-where-it-lives
  recipe**: there is no "off-limits" zone today, but the safe move is still to start from a
  worked twin and change one thing at a time.
- Larger ★★★ tracks (especially ESP-NOW) introduce a genuinely new area (`lib/Wireless/`) and
  continue into Session 8 — that's expected.

---

## Teacher notes

- **Scope ruthlessly to a first step.** "Add ESP-NOW" is a session; "make the screen show my
  device's ID" is a first step. Help every student name a step they can finish today.
- **Pair by ambition.** Two students on the same track can pair; a confident student on a ★★★
  track can mentor a neighbour on a ★ track during share-out.
- **The ★★★ design notes** live in `DEV_ROADMAP.md` Appendix B (ESP-NOW = Bonus 3, web =
  Bonus 2). These are *designs*, not finished code — set expectations accordingly.

---

## Author verification — *to complete when `session-7-start` is cut (Phase 3)*

`session-7-start` is the full pet (all `ENABLE_*` on) — the same as the frozen reference. Before
shipping the session, confirm on device:

- [ ] The full pet builds and runs (all features on).
- [ ] Each ★ track's worked twin is genuinely copy-and-adapt (do one end-to-end yourself).
- [ ] At least one ★★ track is completable within the guided block.
- [ ] The ESP-NOW ★★★ track's prerequisites (two devices, the Bonus 3 design) are noted for the teacher.

---

## Pre-ship checklist (adapted from the lesson-design skill)

- [x] No new flag — open development; the spine is correctly inverted (Survey → Choose → Learn → Build).
- [x] ONE concept (**extend real code incrementally**), one sentence, using every prior fundamental.
- [x] A guaranteed floor: every student ships a ★ change with a worked twin (no blank page).
- [x] Tracks ordered ★→★★★, each naming where it lives and its worked twin.
- [x] The concept (the recipe) is taught by the teacher AND grounded in the twins already in the code.
- [x] Navigation taught up front as the find-where-it-lives recipe (the whole map is now in play).
- [ ] Verified on device — pending the `session-7-start` branch (see above).
