# Session 5 — Persistence

**Day-start flag state:** `ENABLE_ACTION_MENU` + `ENABLE_IMU_PLAY` + `ENABLE_SOUND` ON (from Sessions 2–4); others OFF
**This session activates:** `ENABLE_PERSISTENCE` *(requires `ENABLE_ACTION_MENU` — the Save action lives in the menu)*
**One concept (takeaway sentence):** *"A program can save its values so they survive being
switched off — and load them back next time, each value stored under a name (a key)."*
**Where we are on the map:** you wake the **Storage** teammate. Today's workshop is
`lib/Config/scaffold_config.h` (the switch) and `lib/Storage/storage_manager.cpp` (save & load).
---

## Recap from Session 4

*"A melody is a list of notes."* You gave the pet a voice. But so far, switching the device
off **forgets everything** — the pet starts fresh every boot. Today we fix that: the pet
**remembers**.

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

By the end of Session 5, every student can:
- Flip `ENABLE_PERSISTENCE` and rebuild, then **Save** the pet and see it survive a power-off.
- Explain what it means to **save and load state**, and that each value is stored under a **key** (a name).
- Show that **only what you save is remembered**, and change the first-boot defaults.

The confident also: break and fix a key, or add a brand-new value that persists.

---

## Time budget (4-hour rhythm)

| Block | Time | What happens | Beat |
|---|---|---|---|
| Opening / recap / pair check-in | 25' | Recap lists; "today the pet remembers" | — |
| Presentation | 50' | Flip the flag (**Reveal**) → map update → intro **save/load + keys** | Reveal + Learn |
| Pair exploration | 20' | Save → power-cycle → confirm; stop saving one stat | **Play** |
| Break | 20' | — | — |
| Guided task | 75' | Raise a pet, save it, power-cycle, prove it persists | Make it yours |
| Open creative | 25' | Optional code challenges (★/★★/★★★) | **Build** |
| Share-out + wrap | 15' | "My pet survived a reboot"; what I changed | — |

---

## 1. Reveal — the pet remembers *(in the presentation block)*

1. Open `lib/Config/scaffold_config.h` and **turn on `ENABLE_PERSISTENCE`**.
   *(It needs `ENABLE_ACTION_MENU`, already on since Session 2 — the Save action lives in the menu.)*
2. **Rebuild + flash.**
3. **The reveal:** a new **Save** item appears in the menu. Care for the pet, pick **Save**,
   then **power the device off and on** — the pet comes back exactly as you left it, instead
   of starting over.

## Navigation tour — *teach this before any tweaking*

Update the **"meet the team" map** (same diagram, new "you are here"):
- **Now awake:** **Storage** — the pet's long-term memory (survives power-off).
- **New menu item:** **Save**.
- **Still asleep:** the Stats screen, moods.

Keep the habits: **read the `.h` to use it**. Your **workshop is two files** today.

---

## 2. Play — parameters *(pair exploration)*

Everyone does these. Each is **a small, safe change with an obvious result.** Open
`lib/Storage/storage_manager.cpp`:

- **Dial 1 — prove it (no code).** Change some stats, pick **Save**, power-cycle, and watch
  them come back. Then change stats **without** saving, power-cycle → those changes are gone.
  *(This is the core "only saved values survive" experience.)*
- **Dial 2 — stop saving one stat.** In `save()`, comment out one line, e.g.
  `// prefs.putInt("happy", pet.getHappy());`. Make the pet very happy, Save, power-cycle →
  happiness is **not** remembered (it reverts). Un-comment to restore.
- **Dial 3 — the first-boot default.** In `load()`, the second value in
  `prefs.getInt("fullness", Pet::DEFAULT_FULLNESS)` is what a brand-new pet uses before any
  save exists. Change it (e.g. to `10`) → a fresh/cleared pet starts almost empty (starving).

Predict-then-test: pairs say what will survive a reboot before pulling the power.

---

## 3. Learn — the fundamental: **saving & loading state (keys)**

**Takeaway:** *"A program can save its values so they survive a power-off and load them back
next time — each value stored under a name (a key)."*

**Taught by the teacher (short):** the device's normal memory is wiped when it powers off.
**Saving** writes values to special long-term storage. When you press Save, `save()` writes
each stat under a **name** — `putInt("fullness", ...)`. On boot, `load()` reads each one back by
the **same name** — `getInt("fullness", ...)`. The name (the **key**) is how save and load find
the same value. If nothing was ever saved, `getInt` uses its **default** (the second argument)
— that's why a brand-new pet still boots with sensible stats.

**Pointed to in the code (for the readers):** in `lib/Storage/storage_manager.cpp` —
- `save()` — seven `prefs.putInt("<key>", value)` lines.
- `load()` — seven matching `prefs.setX(prefs.getInt("<key>", default))` lines.
- Notice the **keys match** between save and load — that pairing is the whole trick.

Connect the pathway: the things being saved are the **stat variables from Session 1**.
Persistence is just "remember the boxes' values for next time."

---

## 4. Make it yours — a pet that lasts *(guided task, everyone)*

Raise your pet to a state you're proud of, **Save** it, power-cycle, and prove it survived.
Try the "stop saving one stat" dial and predict what comes back. Everyone leaves having made
their pet permanent across a reboot.

---

## 5. Build — optional code challenges *(open creative, for the confident)*

**Optional.** Saving and power-cycling is a complete session. These extend today's feature
(save/load) and deepen the key idea.

- **★ Tune the fresh start.** Change several first-boot defaults in `load()` so a brand-new
  pet begins with a personality (sleepy, grumpy, hungry). *(Reinforces: the default path.)*
- **★★ Break and fix a key.** In `save()`, change one key string (e.g. `"happy"` → `"happiness"`)
  but leave `load()` unchanged. Save, power-cycle → that stat won't come back (the names no
  longer match, so `load()` uses the default). Then fix it. Worked twin: the matched pairs.
  *(Hands-on proof that the keys must match.)*
- **★★★ Persist a brand-new value *(add a matching key pair)*.** Add a "times fed" counter
  that goes up in `feed()`, then save it with `prefs.putInt("feedCount", ...)` and load it
  with `prefs.getInt("feedCount", 0)`. Worked twins: the existing seven put/get pairs.
  *(The full "add persisted state" exercise — touches `pet` and `storage`.)*

---

## Navigation note (what this session adds to the map)

- **Newly awake:** Storage (long-term memory); **new menu item:** Save.
- **Your workshop (editable today):** `lib/Config/scaffold_config.h`, `lib/Storage/storage_manager.cpp`
  (and `lib/Pet/pet.h` defaults if you tune the fresh start).
- **Still asleep / not for today:** the Stats screen (`ENABLE_MULTISCREEN`), mood sprites.

---

## Author verification — *to complete when `session-5-start` is cut (Phase 3)*

Run on device with the day-start build (menu + IMU + sound on, persistence off), then turn
`ENABLE_PERSISTENCE` on:

- [ ] Day-start build compiles; no Save item; pet starts fresh every boot.
- [ ] Turning on `ENABLE_PERSISTENCE` and rebuilding adds the Save action.
- [ ] Save → power off → power on restores the saved stats.
- [ ] Dial 2 (comment out one `putInt`) makes that stat not persist; un-commenting restores it.
- [ ] Dial 3 (default in `getInt`) changes a fresh/cleared pet's starting value.
- [ ] ★★ mismatched key fails to load; ★★★ new `feedCount` key persists, inside the 25' block.

---

## Pre-ship checklist (from the lesson-design skill)

- [x] Exactly ONE feature revealed (`ENABLE_PERSISTENCE`).
- [x] Exactly ONE fundamental concept (**save/load state + keys**), one sentence, building on variables.
- [x] A dial a struggling student clears fast with a visible result (Save → power-cycle → it's there).
- [x] Challenges optional, ordered ★→★★★, all extending save/load (defaults → keys → new key).
- [x] Concept taught both by teacher (keys pair save↔load) AND pointed to in code (`putInt`/`getInt`).
- [x] Navigation taught up front; map updated with "you are here"; workshop bounded to two files.
- [ ] Verified on device — pending the `session-5-start` branch (see above).
