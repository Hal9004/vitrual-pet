# Virtual Pet — Developer Roadmap

> **Audience:** Students of varying skill levels.
> **Hardware:** M5StickC Plus 2 (ESP32-PICO-V3-02, MPU6886, LCD 135×240, Buzzer, Microphone)
> **Pedagogical rules:** No clever syntax. Full descriptive names. Every function gets a comment.

---

## Part 1 — Audit: Completed vs. Missing Items

Items are mapped directly against `COURSE_CHECKLIST.md`.

### Phase 1: Foundations & UI

| Checklist Item | Status | Where It Lives |
|---|---|---|
| Hardware Initialization (M5.begin, LCD, Serial) | ✅ Done | `src/main.cpp` → `setup()`, `lib/Display/display_manager.cpp` → `init()` |
| Asset Pipeline (images/gifs → C++ arrays) | ❌ Missing | `lib/Display/animation_manager.cpp` — file is empty |
| Basic Sprite Rendering (draw pet to screen) | ⚠️ Partial | Only primitive circles/lines in `display_manager.cpp:211`. No bitmap sprites yet |
| Screen Real Estate Management (stats zone vs. pet zone) | ⚠️ Partial | Layout is hardcoded with magic pixel numbers in `showPetStatus()` and `drawPetFace()`. No zone constants or layout manager |

### Phase 2: Core Logic & State Machine

| Checklist Item | Status | Where It Lives |
|---|---|---|
| State Machine Architecture (IDLE, EATING, SLEEPING, EVOLVING) | ✅ Done | `lib/Pet/pet.h` → `PetState` enum. `lib/Pet/pet.cpp` → `updateState()` / `setState()` / `getState()` |
| State Machine — Full Action Coverage (PLAYING, SICK, HEALING, BATHING) | ✅ Done | `lib/Pet/pet.h` → expanded `PetState` enum. `lib/Pet/pet.cpp` → new cases in `updateState()`, `setState()` wired into `play()`, `bathe()`, `heal()` |
| Hunger Logic (timer-based decrement) | ✅ Done | `lib/Timer/time_manager.cpp` → `applyHungerIncrease()` |
| Happiness Logic (timer-based decrement) | ✅ Done | `lib/Timer/time_manager.cpp` → `applyHappinessDecay()` |
| Energy/Sleep Logic (recovery vs. depletion) | ✅ Done | Auto-drain in `lib/Timer/time_manager.cpp` → `applyEnergyDrain()`. Manual recovery via `pet.cpp` → `sleep()` |
| Death/Reset Condition (handle 0 stats) | ✅ Done | `lib/Pet/pet.h` → `isDead()` / `reset()`. Death screen routed through `display_manager.cpp` → `renderDisplay()` |
| Cleanliness Decay Logic (timer-based decrement) | ✅ Done | `lib/Timer/time_manager.cpp` → `applyCleanlinessDecay()`. Drops by 1 every 10 seconds |
| Sickness Accumulation Logic (rises when cleanliness is low) | ✅ Done | `lib/Timer/time_manager.cpp` → `applySicknessAccumulation()`. Rises by 1 every 12 seconds when `cleanliness` is below 30 |
| Sadness Logic (rises when happiness is low) | ⏸ Deferred | `sad` stat, getter, setter, default constant already exist in `Pet`. Needs: a `TimerManager` rule to raise `sad` when `happy` falls below a threshold, and a sad sprite to display it. Revisit after the asset pipeline (Task 12) is complete |
| Cleanliness / Sickness Display | ✅ Done | Both bars shown in `display_manager.cpp` → `showPetStatus()`. Layout tightened to fit all five stats |

### Phase 3: Interaction & Menu System

| Checklist Item | Status | Where It Lives |
|---|---|---|
| Navigation Logic (B & C cycle, A confirms) | ✅ Done | `lib/Button/button_handler.cpp`, `lib/Actions/action_menu.cpp`, `src/main.cpp:33–49` |
| Menu UI (visual indicators for selected actions) | ✅ Done | `display_manager.cpp:96–108` → `drawMenuIndicator()` |
| Motion Play (MPU6886 accelerometer for Play mode) | ✅ Done | `lib/Imu/imu_manager.h/.cpp` → `ImuManager`. `wasShaken()` called in `src/main.cpp` → triggers `myPet.play()` |
| Sound Feedback (buzzer melodies) | ✅ Done | `lib/Speaker/speaker_manager.h/.cpp` — melodies for all 5 actions, death, reset, hunger alert, sickness alert |
| Voice Memos (microphone record/playback) | ❌ Missing | `lib/Microphone/microphone_manager.cpp` — empty |

### Phase 4: Environmental & Advanced Features

| Checklist Item | Status | Where It Lives |
|---|---|---|
| MPU6886 "Shake to Wake" (low-power wake) | 🚫 Removed | Hardware investigation confirmed the MPU6886 INT pin is not routed to an ESP32 GPIO on the M5StickCPlus2 — interrupt-driven wake is not possible on this board |
| RTC (Real Time Clock for overnight logic) | ❌ Missing | `lib/Timer/time_manager.cpp` exists (decay timers implemented) but RTC/BM8563 integration not started |
| EEPROM/Preferences (save pet on power-off) | ❌ Missing | `lib/Storage/storage_manager.cpp` — empty |
| Evolution Logic (growth stages based on care/time) | ❌ Missing | No growth stage tracking in `Pet` class |

### Phase 5: Connectivity & Polish

| Checklist Item | Status | Where It Lives |
|---|---|---|
| Wireless Communication (BLE or WiFi) | ❌ Missing | Not started |
| Remote Dashboard (Web/App stat checking) | ❌ Missing | Not started |
| Final UI Polish (comments, descriptive names) | ⚠️ Partial | Existing code is reasonably documented. Magic pixel constants have been replaced with named constants. Remaining: bitmap sprites and animation |
| SpeakerManager refactor — playNote() helper | ⏸ Deferred | Every sound method repeats the same tone/delay/stop pattern. A `playNote(frequency, duration)` helper could eliminate the repetition. Intentionally left verbose for now so students can read each melody top to bottom without following abstractions. Revisit during the final polish pass. |

---

## Part 2 — Complexity Queue

Tasks ordered from **easiest** to **hardest** so a student always has a clear next step that builds on what they already know.

```
LEVEL 1 — COPY THE PATTERN (no new concepts)
  1. Happiness auto-decay timer        ✅ Done
  2. Energy auto-drain timer           ✅ Done
  3. Death / Reset condition           ✅ Done
 3a. Cleanliness decay timer           ✅ Done
 3b. Sickness accumulation timer       ✅ Done

LEVEL 2 — SMALL NEW CONCEPT
  4. State Machine Architecture        ✅ Done
 4b. Expand State Machine              ✅ Done
  5. Screen Real Estate constants      ✅ Done

LEVEL 3 — NEW HARDWARE API (library already in project)
  6. MPU6886 Motion Play               ✅ Done
  7. MPU6886 Shake to Wake             🚫 Removed — INT pin not routed on M5StickCPlus2
  8. Buzzer Sound Feedback             ✅ Done

LEVEL 4 — DATA + PLANNING
  9. State Machine Cleanup             ✅ Done (STATE_DEAD + alert timers moved from main into updateState)
  9a. Evolution Logic                  (deferred — age counter + growth stages, requires task 9 refactor)
  9b. Sadness Logic                    (deferred — sad rises when happy is low, needs sad sprite from task 12)
 10. EEPROM / Preferences persistence  ← ★ NEXT TASK (new: Preferences library, key/value storage)

LEVEL 5 — ASSET PIPELINE
 11. Screen Real Estate layout zones   (refactor from task 5 into reusable structs)
 12. Asset Pipeline (image → C array)  (new: Python/online converter tools)
 13. Basic Sprite Rendering            (requires: asset pipeline from task 12)

LEVEL 6 — COMPLEX HARDWARE
 14. RTC overnight logic               (new: I2C, BM8563 library, Unix timestamp math)
 15. Microphone Voice Memos            (new: DMA audio buffers — see Hardware Gotchas)

LEVEL 7 — NETWORKING
 16. Wireless Communication (BLE/WiFi) (new: WiFi.h, ESP-NOW or BLE library)
 17. Remote Dashboard                  (requires: task 16 + simple HTTP server)

PHASE 6 — STUDENT TEMPLATE CREATION (after fully functioning Tamagotchi is complete)
 18. Codebase review against lesson plan
     — Walk every module against the course checklist and confirm the reference
       implementation is clean, documented, and complete.
 19. Create skeleton templates per module
     — Strip core logic from each .cpp file, leaving function signatures, comments,
       and example implementations where needed to give students enough scaffolding.
       Students write their own logic inside the provided structure.
 20. Validate templates compile and are teachable
     — Each skeleton should compile without errors and give a student of the target
       skill level enough context to complete it without being lost.
 21. Organise templates by complexity level
     — One template set per Level (1–7) so teachers can assign tasks matched to
       each student's experience. Beginner students get Level 1–2 skeletons;
       advanced students get Level 4–7.
```

---

## Part 3 — Hardware Gotchas

These are real pitfalls that will cause mysterious bugs or crashes on the ESP32-PICO-V3-02. Read this section before attempting Levels 5–7.

---

### Gotcha 1 — Audio Buffer Memory on the ESP32-PICO-V3-02

**The problem:** The ESP32-PICO-V3-02 has 520 KB of internal SRAM, but the M5StickC Plus 2 display framebuffer and stack already consume a large chunk. If you allocate a large audio recording buffer on the stack (as a local array inside a function), the device will crash with a stack overflow — often with no error message.

**The rule:** Always allocate audio buffers on the **heap** using `malloc()` (or Arduino's `new`), and always **free** them when done. Never declare a large buffer like `int16_t recordingBuffer[8000]` inside a function.

```cpp
// BAD — this lives on the stack and will crash the device
void recordSound() {
    int16_t recordingBuffer[8000]; // ~16 KB on the stack — CRASH
}

// GOOD — this lives on the heap
void recordSound() {
    // Ask the heap for memory. Check that it succeeded before using it.
    int16_t* recordingBuffer = (int16_t*) malloc(8000 * sizeof(int16_t));

    if (recordingBuffer == nullptr) {
        // malloc returned null — not enough free memory
        Serial.println("ERROR: Not enough memory to record audio.");
        return;
    }

    // ... use recordingBuffer here ...

    // ALWAYS release heap memory when you are done with it
    free(recordingBuffer);
    recordingBuffer = nullptr; // Good habit: nullify the pointer after freeing
}
```

**Tip:** Call `ESP.getFreeHeap()` and print it via Serial before and after your recording function to make sure memory is being released correctly.

---

### Gotcha 2 — Non-Blocking Logic with `millis()` (Never Use `delay()` in the Main Loop)

**The problem:** `delay(1000)` pauses the entire program for 1 second. During that pause, button presses are ignored, the pet's face freezes, and the device feels "dead". This is called **blocking** code.

**The rule:** Use `millis()` to track time without stopping the program. The pattern is always the same three lines:

```cpp
// Declare a variable to remember when the last event happened.
// The word 'static' means it keeps its value between loop() calls.
static unsigned long lastHappinessDecayTime = 0;

// Inside loop() — check if enough time has passed
if (millis() - lastHappinessDecayTime >= 5000) {  // 5000 ms = 5 seconds
    // Do the timed action here
    myPet.setHappy(myPet.getHappy() - 1);

    // Remember when this last happened so the timer resets
    lastHappinessDecayTime = millis();
}
```

**This is the SAME pattern** used in `lib/Timer/time_manager.cpp` for both the hunger and happiness timers. Every timed feature in this project should use this pattern.

**Warning:** Never use `delay()` inside `loop()`, `update()`, or any function called from `loop()`. It is fine to use `delay()` once inside `setup()` for a startup message.

---

### Gotcha 3 — Screen Flicker Prevention with M5Canvas (Double-Buffering)

**The problem:** Calling `M5.Lcd.clear()` followed by multiple draw calls causes visible flickering because the screen is blank for a fraction of a second between clears and redraws. This is already partially managed by the 5-second `STATUS_UPDATE_INTERVAL` in `display_manager.cpp:48–53`, but for fast-moving animations it will not be enough.

**The solution:** Use `M5Canvas` as an off-screen buffer. You draw everything onto the invisible canvas first, then "push" the finished image to the screen in one fast operation. The screen never shows a blank intermediate state.

```cpp
#include "M5StickCPlus2.h"

// Create a canvas the same size as the screen
M5Canvas canvas(&M5.Lcd);

void setup() {
    M5.begin();
    // Tell the canvas how big to be (matches the screen: 135 wide, 240 tall)
    canvas.createSprite(135, 240);
}

void drawFrame() {
    // Step 1: Draw everything onto the INVISIBLE canvas (never shows flicker)
    canvas.fillSprite(TFT_BLACK);         // Clear the canvas with black
    canvas.drawCircle(67, 120, 25, TFT_WHITE); // Draw pet face onto canvas

    // Step 2: Push the finished canvas to the REAL screen all at once
    // Arguments: destination X, destination Y on the real screen
    canvas.pushSprite(0, 0);
}
```

**Memory note:** A full 135×240 canvas at 16-bit colour uses `135 × 240 × 2 = 64,800 bytes` (~63 KB) of heap. This is safe on the ESP32-PICO-V3-02 as long as you are not also holding a large audio buffer at the same time. If you run low on heap, reduce the canvas to cover only the animated region of the screen rather than the full display.

---

## Part 4 — Next Best Foundational Tasks

---

### Task 3a — Cleanliness Decay Timer ✅ Next

**Why this task next?**

`cleanliness` can already be increased by `bathe()`, but it never decreases on its own. The pet can stay perfectly clean forever without any effort, which makes the bathe action pointless. This task adds the missing decay timer using the exact same `millis()` pattern students have already seen three times in Tasks 1–3. No new concepts — just practice.

**Exactly where to add the code:**

Step 1 — Add the decay function declaration to `lib/Timer/time_manager.h`, alongside the existing timer declarations:
```cpp
// Decreases cleanliness over time so the pet gets dirty without bathing
void applyCleanlinessDecay(Pet& pet);
```

Step 2 — Add the implementation to `lib/Timer/time_manager.cpp`:
```cpp
// applyCleanlinessDecay()
// Decreases cleanliness by 1 every 8 seconds. The pet gets dirty over time
// and will need bathing, making the bathe action meaningful.
void TimerManager::applyCleanlinessDecay(Pet& pet) {
    static unsigned long lastCleanlinessDecayTime = 0;
    unsigned long cleanlinessDecayInterval = 8000;

    if (millis() - lastCleanlinessDecayTime >= cleanlinessDecayInterval) {
        pet.setCleanliness(pet.getCleanliness() - 1);
        lastCleanlinessDecayTime = millis();
    }
}
```

Step 3 — Call it inside `TimerManager::update()` in `time_manager.cpp`:
```cpp
applyCleanlinessDecay(pet);
```

**Files touched:** `lib/Timer/time_manager.h` and `lib/Timer/time_manager.cpp`.

---

### Task 3b — Sickness Accumulation Timer

**Why this task next?**

`sick` can only be decreased by `heal()`, but it never increases — the pet can never actually get sick. This task makes sickness a real threat by slowly increasing `sick` when `cleanliness` is low. It uses the same `millis()` timer pattern but introduces a simple conditional: the timer only fires when a condition is met. A natural next step after 3a.

**Exactly where to add the code:**

Step 1 — Add the declaration to `lib/Timer/time_manager.h`:
```cpp
// Increases sickness over time when the pet is dirty
void applySicknessAccumulation(Pet& pet);
```

Step 2 — Add the implementation to `lib/Timer/time_manager.cpp`:
```cpp
// applySicknessAccumulation()
// Increases sick by 1 every 10 seconds when cleanliness is below 30.
// A dirty pet gradually becomes unwell — bathing prevents this.
void TimerManager::applySicknessAccumulation(Pet& pet) {
    static unsigned long lastSicknessAccumulationTime = 0;
    unsigned long sicknessAccumulationInterval = 10000;
    int cleanlinessDangerThreshold = 30;

    if (pet.getCleanliness() < cleanlinessDangerThreshold) {
        if (millis() - lastSicknessAccumulationTime >= sicknessAccumulationInterval) {
            pet.setSick(pet.getSick() + 1);
            lastSicknessAccumulationTime = millis();
        }
    }
}
```

Step 3 — Call it inside `TimerManager::update()`:
```cpp
applySicknessAccumulation(pet);
```

**Files touched:** `lib/Timer/time_manager.h` and `lib/Timer/time_manager.cpp`.

---

### Task 4b — Expand State Machine (PLAYING, SICK, HEALING, BATHING)

**Why this task next?**

Task 4 added the state machine foundation but only wired two actions into it — `feed()` and `sleep()`. The remaining three care actions (`play()`, `bathe()`, `heal()`) still set no state, which means the switch handler can never react to them. This task closes that gap. The student already knows the enum and switch pattern from Task 4 — this is purely practice: add four more states, four more cases, and four more `setState()` calls.

**Exactly where to add the code:**

Step 1 — Add the four new states to the `PetState` enum in `lib/Pet/pet.h`:
```cpp
enum PetState {
    STATE_IDLE,      // Default — pet is awake but doing nothing
    STATE_EATING,    // Triggered by feed() — pet is eating
    STATE_SLEEPING,  // Triggered by sleep() — pet is resting
    STATE_PLAYING,   // Triggered by play() — pet is exercising
    STATE_SICK,      // Entered automatically when sick stat is high — pet is unwell
    STATE_HEALING,   // Triggered by heal() — pet is receiving treatment
    STATE_BATHING,   // Triggered by bathe() — pet is being cleaned
    STATE_EVOLVING   // Reserved for future evolution logic (task 9)
};
```

Step 2 — Add the four new cases to the switch in `Pet::updateState()` in `pet.cpp`:
```cpp
case STATE_PLAYING:
    // Playing is handled instantly by play() — return to idle
    setState(STATE_IDLE);
    break;

case STATE_SICK:
    // Pet stays sick until heal() is called — no automatic return to idle
    break;

case STATE_HEALING:
    // Healing is handled instantly by heal() — return to idle
    setState(STATE_IDLE);
    break;

case STATE_BATHING:
    // Bathing is handled instantly by bathe() — return to idle
    setState(STATE_IDLE);
    break;
```

Step 3 — Wire `setState()` calls into the remaining actions in `pet.cpp`:
```cpp
void Pet::play() {
    setState(STATE_PLAYING);   // <-- add this line
    happy     = happy     + 25;
    tired     = tired     + 20;
    energised = energised - 20;
    hungry    = hungry    + 15;
    constrainValues();
}

void Pet::bathe() {
    setState(STATE_BATHING);   // <-- add this line
    cleanliness = cleanliness + 30;
    tired       = tired       + 10;
    energised   = energised   - 10;
    constrainValues();
}

void Pet::heal() {
    setState(STATE_HEALING);   // <-- add this line
    sick  = sick  - 50;
    tired = tired + 20;
    happy = happy - 5;
    constrainValues();
}
```

Step 4 — Enter `STATE_SICK` automatically when `sick` is high. Add this check inside the `STATE_IDLE` case in `Pet::updateState()`:
```cpp
case STATE_IDLE:
    // If the sick stat is dangerously high, transition to the sick state automatically
    if (sick >= 50) {
        setState(STATE_SICK);
    }
    break;
```

**Files touched:** `lib/Pet/pet.h` and `lib/Pet/pet.cpp`.

---

### Task 5 — Screen Real Estate Constants

**Why this task?**

`display_manager.cpp` currently uses raw pixel numbers like `5`, `36`, `125`, and `152` scattered across several functions. These are called **magic numbers** — numbers with no explanation of what they represent. If a student wants to move the pet face down by 10 pixels, they have to hunt through the file to find every related number and hope they don't miss one. Named constants fix this: change the value in one place, and it updates everywhere.

**New concept introduced:** `static const` class members in a header file. The `static` keyword means the constant belongs to the class itself, not to any single object — every `DisplayManager` instance shares the same value without wasting extra memory.

**Exactly where to add the code:**

Step 1 — Add the layout constants to the `private:` section of `DisplayManager` in `lib/Display/display_manager.h`, below the existing `SCREEN_WIDTH` and `SCREEN_HEIGHT` constants:

```cpp
// Shared left margin and width used by every stat bar and label
static const int STAT_LEFT_MARGIN = 5;
static const int STAT_BAR_WIDTH   = 125;

// Y position of each stat label, and the bar drawn 10 px below it
static const int HAPPY_LABEL_Y  = 26;
static const int HAPPY_BAR_Y    = 36;
static const int HUNGER_LABEL_Y = 48;
static const int HUNGER_BAR_Y   = 58;
static const int ENERGY_LABEL_Y = 70;
static const int ENERGY_BAR_Y   = 80;
static const int CLEAN_LABEL_Y  = 92;
static const int CLEAN_BAR_Y    = 102;
static const int SICK_LABEL_Y   = 114;
static const int SICK_BAR_Y     = 124;

// Pet face drawn below the five stat bars
static const int PET_FACE_Y      = 152;
static const int PET_FACE_RADIUS = 18;

// Mood text printed just below the pet face
static const int MOOD_TEXT_Y = 180;

// Menu indicator strip pinned to the bottom of the screen
static const int MENU_INDICATOR_X      = 5;
static const int MENU_INDICATOR_Y      = 220;
static const int MENU_INDICATOR_WIDTH  = 130;
static const int MENU_INDICATOR_HEIGHT = 20;
```

Step 2 — In `lib/Display/display_manager.cpp`, replace the raw numbers in `showPetStatus()` with the new constants:

```cpp
// Before (magic numbers)
M5.Lcd.setCursor(5, 26);
drawStatusBar(happiness, 100, 5, 36, 125, TFT_GREEN);

// After (named constants)
M5.Lcd.setCursor(STAT_LEFT_MARGIN, HAPPY_LABEL_Y);
drawStatusBar(happiness, 100, STAT_LEFT_MARGIN, HAPPY_BAR_Y, STAT_BAR_WIDTH, TFT_GREEN);
```

Repeat for all five stats (Hunger, Energy, Clean, Sick) using the matching `_LABEL_Y` and `_BAR_Y` constants.

Step 3 — Replace magic numbers in `showPetMood()`:

```cpp
// Before
printCenteredText(moodText, 180, moodColor, 2);

// After
printCenteredText(moodText, MOOD_TEXT_Y, moodColor, 2);
```

Step 4 — Replace magic numbers in `drawPetFace()`:

```cpp
// Before
int faceY = 152;
int faceRadius = 18;

// After
int faceY = PET_FACE_Y;
int faceRadius = PET_FACE_RADIUS;
```

Step 5 — Replace magic numbers in `drawMenuIndicator()`:

```cpp
// Before
fillRect(x, y, 130, 20, TFT_BLACK);
M5.Lcd.drawRect(x, y, 130, 20, TFT_CYAN);

// After
fillRect(x, y, MENU_INDICATOR_WIDTH, MENU_INDICATOR_HEIGHT, TFT_BLACK);
M5.Lcd.drawRect(x, y, MENU_INDICATOR_WIDTH, MENU_INDICATOR_HEIGHT, TFT_CYAN);
```

Step 6 — Replace the hardcoded call site in `renderDisplay()`:

```cpp
// Before
drawMenuIndicator(menu, 5, 220);

// After
drawMenuIndicator(menu, MENU_INDICATOR_X, MENU_INDICATOR_Y);
```

**Files touched:** `lib/Display/display_manager.h` and `lib/Display/display_manager.cpp`.

---

### Task 8 — Buzzer Sound Feedback ✅ Done

**Why this task?**

The pet could already show what it was doing on screen, but gave no audio cue. This task fills in `lib/Speaker/speaker_manager.h/.cpp` and wires it into the action menu and main loop so every care action, every alert, and the death/reset lifecycle each play a distinct melody. It introduces a new hardware API (`M5.Speaker.tone()`) and teaches the difference between blocking sound calls (acceptable inside `confirmAction()`) and non-blocking alert patterns (millis()-debounced in the main loop).

**Architecture — what was added:**

`SpeakerManager` follows the same single-responsibility pattern as every other `lib/` module. It has one job: play sounds. It knows nothing about pet stats or the display.

| Method | When it fires |
|---|---|
| `init()` | Once in `setup()` — sets speaker volume |
| `playFeedSound()` | Inside `confirmAction()` when FEED is selected |
| `playPlaySound()` | Inside `confirmAction()` when PLAY is selected |
| `playSleepSound()` | Inside `confirmAction()` when SLEEP is selected |
| `playBatheSound()` | Inside `confirmAction()` when BATHE is selected |
| `playHealSound()` | Inside `confirmAction()` when HEAL is selected |
| `playDeathSound()` | Once in `loop()` when `isDead()` becomes true (static flag prevents replaying) |
| `playResetSound()` | In `loop()` when Button A is pressed on the death screen |
| `playHungerAlertSound()` | In `loop()` via millis() timer — at most once every 15 s when `hungry >= 80` |
| `playSicknessAlertSound()` | In `loop()` via millis() timer — at most once every 15 s when `sick >= 80` |

**Step 1 — Fill in `lib/Speaker/speaker_manager.h`:**

Declare `SpeakerManager` with `init()` and one named method per event. Keep the names descriptive so a student reading the call site immediately understands what sound will play.

**Step 2 — Fill in `lib/Speaker/speaker_manager.cpp`:**

Implement each method using `M5.Speaker.tone(frequency, duration)`. Each method plays 2–4 notes chosen to match the mood of the event. See `USEFUL_NOTES.md` for a full explanation of how frequencies map to musical notes.

**Step 3 — Update `lib/Actions/action_menu.h`:**

Add `#include "../Speaker/speaker_manager.h"` and update the `confirmAction()` signature to accept a `SpeakerManager&` parameter.

**Step 4 — Update `lib/Actions/action_menu.cpp`:**

In `confirmAction()`, add a `switch` on `selectedAction.type` after `executePetAction()`. Call the matching sound method for each action type.

**Step 5 — Update `src/main.cpp`:**

- Add `#include "../lib/Speaker/speaker_manager.h"` and a `SpeakerManager speaker;` global.
- Call `speaker.init()` in `setup()` after `M5.begin()`.
- Add a `static bool deathSoundPlayed` flag inside the `isDead()` block to play the death melody once and the reset fanfare on revival.
- Add two millis()-debounced alert blocks after `timers.update()` — one for hunger, one for sickness.
- Update the `confirmAction()` call to pass `speaker` as the third argument.

**Files touched:** `lib/Speaker/speaker_manager.h`, `lib/Speaker/speaker_manager.cpp`, `lib/Actions/action_menu.h`, `lib/Actions/action_menu.cpp`, `src/main.cpp`.
