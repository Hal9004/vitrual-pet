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
| State Machine Architecture (IDLE, EATING, SLEEPING, EVOLVING) | ❌ Missing | No `PetState` enum exists. The `Pet` class only tracks numeric stats, not behavioural states |
| Hunger Logic (timer-based decrement) | ✅ Done | `lib/Timer/time_manager.cpp` → `applyHungerIncrease()` |
| Happiness Logic (timer-based decrement) | ✅ Done | `lib/Timer/time_manager.cpp` → `applyHappinessDecay()` |
| Energy/Sleep Logic (recovery vs. depletion) | ✅ Done | Auto-drain in `lib/Timer/time_manager.cpp` → `applyEnergyDrain()`. Manual recovery via `pet.cpp` → `sleep()` |
| Death/Reset Condition (handle 0 stats) | ✅ Done | `lib/Pet/pet.h` → `isDead()` / `reset()`. Death screen routed through `display_manager.cpp` → `renderDisplay()` |

### Phase 3: Interaction & Menu System

| Checklist Item | Status | Where It Lives |
|---|---|---|
| Navigation Logic (B & C cycle, A confirms) | ✅ Done | `lib/Button/button_handler.cpp`, `lib/Actions/action_menu.cpp`, `src/main.cpp:33–49` |
| Menu UI (visual indicators for selected actions) | ✅ Done | `display_manager.cpp:96–108` → `drawMenuIndicator()` |
| Motion Play (MPU6886 accelerometer for Play mode) | ❌ Missing | `lib/Speaker/` — empty. M5.Imu not called anywhere |
| Sound Feedback (buzzer melodies) | ❌ Missing | `lib/Speaker/speaker_manager.cpp` — empty |
| Voice Memos (microphone record/playback) | ❌ Missing | `lib/Microphone/microphone_manager.cpp` — empty |

### Phase 4: Environmental & Advanced Features

| Checklist Item | Status | Where It Lives |
|---|---|---|
| MPU6886 "Shake to Wake" (low-power wake) | ❌ Missing | No accelerometer code exists at all |
| RTC (Real Time Clock for overnight logic) | ❌ Missing | `lib/Timer/time_manager.cpp` exists (decay timers implemented) but RTC/BM8563 integration not started |
| EEPROM/Preferences (save pet on power-off) | ❌ Missing | `lib/Storage/storage_manager.cpp` — empty |
| Evolution Logic (growth stages based on care/time) | ❌ Missing | No growth stage tracking in `Pet` class |

### Phase 5: Connectivity & Polish

| Checklist Item | Status | Where It Lives |
|---|---|---|
| Wireless Communication (BLE or WiFi) | ❌ Missing | Not started |
| Remote Dashboard (Web/App stat checking) | ❌ Missing | Not started |
| Final UI Polish (comments, descriptive names) | ⚠️ Partial | Existing code is reasonably documented. Magic pixel constants and the `lambda` in `constrainValues()` should be simplified for students |

---

## Part 2 — Complexity Queue

Tasks ordered from **easiest** to **hardest** so a student always has a clear next step that builds on what they already know.

```
LEVEL 1 — COPY THE PATTERN (no new concepts)
  1. Happiness auto-decay timer        ✅ Done
  2. Energy auto-drain timer           ✅ Done
  3. Death / Reset condition           ✅ Done

LEVEL 2 — SMALL NEW CONCEPT
  4. State Machine Architecture        ← ★ NEXT TASK (new: enum + switch statement)
  5. Screen Real Estate constants      (new: named layout constants, no more magic numbers)

LEVEL 3 — NEW HARDWARE API (library already in project)
  6. MPU6886 Motion Play               (new: M5.Imu.getAccel, threshold detection)
  7. MPU6886 Shake to Wake             (builds directly on task 6)
  8. Buzzer Sound Feedback             (new: M5.Speaker.tone() for simple melodies)

LEVEL 4 — DATA + PLANNING
  9. Evolution Logic                   (requires: state machine from task 4 + age counter)
 10. EEPROM / Preferences persistence  (new: Preferences library, key/value storage)

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

## Part 4 — Next Best Foundational Task

**Task: State Machine Architecture (Task 4)**

**Why this task next?**

The pet currently tracks numbers but has no concept of what it is *doing*. A state machine fixes that:

1. **It introduces one new concept at a time.** The student learns `enum` (a named list of states) and `switch` (a clean way to branch on it) — nothing else.
2. **It unlocks everything above it.** Evolution (task 9) needs a state to transition through. Animations need a state to key off. Without it, every future feature adds more `if` chains.
3. **The pattern is universally reusable.** A switch-based state machine appears in almost every embedded project. Learning it here pays off far beyond this pet.

**Exactly where to add the code:**

Step 1 — Add the enum to `lib/Pet/pet.h` before the class declaration:
```cpp
// PetState — the list of behaviours the pet can currently be doing.
// Only one state is active at a time.
enum PetState {
    STATE_IDLE,      // Default — pet is awake but doing nothing
    STATE_EATING,    // Triggered by feed() — pet is eating
    STATE_SLEEPING,  // Triggered by sleep() — pet is resting
    STATE_EVOLVING   // Reserved for future evolution logic (task 9)
};
```

Step 2 — Add a `currentState` member and interface to the `Pet` class in `pet.h`:
```cpp
private:
    PetState currentState;   // Which behaviour the pet is currently in

public:
    PetState getState() const;         // Returns the current state
    void setState(PetState newState);  // Changes the current state
    void updateState();                // Runs the switch handler each loop
```

Step 3 — Initialise it in the constructor in `pet.cpp`:
```cpp
Pet::Pet() : hungry(20), tired(20), happy(80), sick(0), sad(0),
             cleanliness(80), energised(80), currentState(STATE_IDLE) {
}
```

Step 4 — Add the switch handler in `pet.cpp`:
```cpp
// updateState()
// Runs once per loop. Checks the current state and applies any
// behaviour that belongs to it. Add new states here as the game grows.
void Pet::updateState() {
    switch (currentState) {
        case STATE_IDLE:
            // Nothing special happens while idle
            break;

        case STATE_EATING:
            // Eating is handled instantly by feed() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_SLEEPING:
            // Sleeping is handled instantly by sleep() — return to idle
            setState(STATE_IDLE);
            break;

        case STATE_EVOLVING:
            // Placeholder — evolution logic added in task 9
            setState(STATE_IDLE);
            break;
    }
}
```

Step 5 — Wire state transitions into existing actions in `pet.cpp`:
```cpp
void Pet::feed() {
    setState(STATE_EATING);   // <-- add this line
    hungry = hungry - 20;
    happy  = happy  + 5;
    constrainValues();
}

void Pet::sleep() {
    setState(STATE_SLEEPING); // <-- add this line
    tired     = tired     - 30;
    energised = energised + 30;
    constrainValues();
}
```

Step 6 — Call `updateState()` from `main.cpp`'s `loop()`:
```cpp
myPet.updateState();
```

**Files touched:** `lib/Pet/pet.h` and `lib/Pet/pet.cpp`. One line added to `src/main.cpp`.
