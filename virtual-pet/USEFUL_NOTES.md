# Virtual Pet — Teaching Notes

This file explains the "why" behind concepts that appear in the code but are not obvious from reading the code alone. These are not instructions for building features — those live in `DEV_ROADMAP.md`. This is for when you finish a task and find yourself asking *"but how does that actually work?"*

---

## How Musical Notes Work (`M5.Speaker.tone`)

### Sound is vibration

When the buzzer makes a sound, it is physically vibrating — a tiny component is moving back and forth very quickly. The speed of that vibration is called the **frequency**, and it is measured in **Hertz (Hz)**, which just means "times per second".

Your ear detects that vibration speed and your brain interprets it as a pitch:

- A slow vibration (low Hz) sounds like a low, deep note.
- A fast vibration (high Hz) sounds like a high, sharp note.

When you write `M5.Speaker.tone(440, 200)`, you are saying:
> "Vibrate at 440 times per second, for 200 milliseconds."

Your ear hears that as the musical note **A4** — the A above middle C, the note that orchestras tune to before a concert.

### The notes used in this project

| Frequency | Note name | Where it appears |
|---|---|---|
| 220 Hz | A3 | End of death melody (very low, very final) |
| 294 Hz | D4 | Sickness alert beeps |
| 349 Hz | F4 | Middle of sleep melody |
| 392 Hz | G4 | Start of heal melody |
| 440 Hz | A4 | Start of death melody, part of heal melody |
| 523 Hz | C5 | Feed melody, bathe melody, heal melody, play melody |
| 659 Hz | E5 | Feed melody, heal melody, reset fanfare |
| 784 Hz | G5 | Play melody, bathe melody, reset fanfare |
| 880 Hz | A5 | Hunger alert beeps (high, urgent) |
| 1047 Hz | C6 | End of play melody and reset fanfare (bright, triumphant) |

### The doubling rule — what an octave is

Here is the most important pattern in music: **doubling the frequency raises the pitch by exactly one octave**.

- A4 = 440 Hz
- A5 = 880 Hz (double 440 — one octave higher, same note but brighter)
- A6 = 1760 Hz (double again — two octaves above A4)

Your ear recognises the ratio between two frequencies, not the raw numbers. That is why 440 Hz and 880 Hz feel like "the same note" even though the numbers are different. This is also why the hunger alert (880 Hz) and the sickness alert (294 Hz) sound so different — they are nearly three octaves apart.

### How duration and gaps shape a melody

`M5.Speaker.tone(frequency, duration)` plays one note for `duration` milliseconds. The `delay()` call after it creates the gap before the next note starts.

```cpp
M5.Speaker.tone(523, 100);  // C5, very short — snappy
delay(120);
M5.Speaker.tone(659, 180);  // E5, slightly longer — feels like it "arrives"
delay(200);
```

Changing just the durations changes the whole character of a melody:
- Short notes (80–120 ms) feel quick and energetic.
- Longer notes (200–400 ms) feel calm, dramatic, or final.
- A longer final note gives a sense of resolution — the melody has "landed".

Try changing the numbers in `lib/Speaker/speaker_manager.cpp` and flashing the device to hear the difference. Experimenting is the fastest way to develop an intuition for this.

### Why the sound code uses `delay()` — and why that is acceptable here

You will notice that the functions in `speaker_manager.cpp` use `delay()` between each note. The next section explains why `delay()` is normally forbidden inside `loop()`, so this might look like a contradiction. It is not — and understanding the difference is important.

The sound functions only ever run inside `confirmAction()`, which is called when the user presses Button A to confirm a menu choice. At that moment, the device is **intentionally pausing** to show the user a feedback message on screen anyway — there is already a `delay(1000)` call there. Adding a short melody of a few hundred milliseconds inside that same deliberate pause does not make things worse. The user expects the game to briefly acknowledge their input.

The rule is more precisely stated as: **do not use `delay()` for background, automatic events that should happen silently without interrupting the user**. The stat decay timers are a perfect example — if hunger increased every 5 seconds and the screen froze for a moment each time, the device would feel broken. The melodies are different: they are a direct, intentional response to something the user just did.

The hunger and sickness alert beeps are a minor exception — they do call `delay()` from the main loop for about 300 ms each time they fire. That brief freeze is a deliberate trade-off: the beep needs two separate tones to be recognisable, and the simplicity of the code is worth the very short interruption that happens at most once every 15 seconds.

---

## How Non-Blocking Timers Work (`millis()`)

### The problem with `delay()`

`delay(1000)` pauses the **entire program** for one second. While it is paused, nothing runs — buttons are not checked, the display is not updated, no stat changes are calculated. To a user, the device just feels frozen.

For a one-off startup message this is fine. But inside `loop()`, which runs hundreds of times per second, using `delay()` means the pet's face freezes every time a timer fires. That is why the rule exists: **never use `delay()` inside `loop()`**.

### The solution: ask the clock instead of stopping the clock

`millis()` returns the number of milliseconds that have passed since the device was switched on. It never pauses anything — it just reads a counter that the hardware is always updating in the background.

The pattern for a non-blocking timer is always these same three ideas:

```cpp
// 1. Remember the last time this event fired.
//    'static' means this variable keeps its value between loop() calls.
static unsigned long lastHappinessDecayTime = 0;

// 2. Decide how long to wait between firings.
unsigned long happinessDecayInterval = 5000;  // 5000 ms = 5 seconds

// 3. Check if enough time has passed. If yes, act and reset the clock.
if (millis() - lastHappinessDecayTime >= happinessDecayInterval) {
    myPet.setHappy(myPet.getHappy() - 1);  // do the timed action
    lastHappinessDecayTime = millis();      // reset the timer
}
```

Every time `loop()` runs, this check takes almost zero time — it is just subtraction and comparison. If not enough time has passed, the `if` is false and the code skips straight past it. The program never stops.

### Where to find this pattern in the project

Every decay and accumulation timer in `lib/Timer/time_manager.cpp` uses exactly this pattern — `applyHungerIncrease()`, `applyHappinessDecay()`, `applyEnergyDrain()`, and so on. The hunger alert and sickness alert in `src/main.cpp` use it too. Once you recognise the three-part structure, you will see it everywhere in embedded code.

---

## How to Completely Reset an ESP32 Device

Reflashing the firmware does **not** wipe NVS or any other data partition — only the firmware image is overwritten. If you need a guaranteed clean slate, you have two options.

### Option 1 — Erase via PlatformIO (inside a project)

Run this command from inside any PlatformIO project directory:

```bash
pio run --target erase
```

This wipes the entire flash chip — firmware, NVS, filesystem, everything. The next upload will start completely fresh. This works in any PlatformIO project without any extra setup.

### Option 2 — Erase via esptool.py (outside a project)

`esptool.py` is the underlying tool PlatformIO uses, and it is installed automatically as part of the ESP32 platform. You can call it directly without a PlatformIO project:

```bash
esptool.py --port /dev/tty.usbserial-XXXX erase_flash
```

Replace `/dev/tty.usbserial-XXXX` with the port your device is connected to. On macOS you can find it by running:

```bash
ls /dev/tty.usb*
```

This is useful when sharing a reset procedure with someone who has the ESP32 toolchain installed but is not using PlatformIO, or when working on a device outside of any specific project.

### What each option erases

| What | `pio run --target erase` | `esptool.py erase_flash` |
|---|---|---|
| Firmware (`app0` / `app1`) | ✅ | ✅ |
| NVS (Preferences data) | ✅ | ✅ |
| Filesystem (SPIFFS / LittleFS) | ✅ | ✅ |
| Partition table | ✅ | ✅ |

Both commands erase everything. After either one, you must re-upload your firmware before the device will run again.

---

## What `const` Means at the End of a Function Declaration

You will notice that some functions in the class headers end with the word `const`:

```cpp
PetState getState() const;
int      getHungry() const;
bool     isDead()    const;
```

This `const` is a **promise to the compiler** that the function will not modify any member variables of the object. Functions marked this way are called **const member functions**.

### Why it matters — intent and enforcement

`getState()` only reads `currentState` and returns it. It changes nothing. Marking it `const` communicates that clearly to anyone reading the code, and the compiler enforces the promise. If you accidentally wrote `currentState = STATE_IDLE` inside a `const` function, the compiler would refuse to build and tell you exactly why.

Scanning a class header becomes much easier once you know this rule:
- Functions with `const` at the end are **read-only** — safe to call with no side effects.
- Functions without it **may change the object's state** — setters and action methods like `feed()` and `play()`.

### Why it matters — const references

When a function receives an object as `const Pet&`, it is promising not to modify that pet. The compiler then only allows `const` member functions to be called on it. `StorageManager::save()` is a real example of this in the project:

```cpp
// The const Pet& parameter means: "I promise not to modify this pet."
void StorageManager::save(const Pet& pet) {
    prefs.putInt("hungry", pet.getHungry());  // allowed — getHungry() is const
    pet.setHungry(50);                        // compiler error — setHungry() is not const
}
```

The `const` on the parameter and the `const` on each getter work together. The compiler can verify that `save()` truly does not modify the pet — every function it calls on `pet` has made the same promise.

### The general rule

Mark a member function `const` whenever it only **reads** from the object and never changes it. In practice this means all getters should be `const`, and action methods that change stats should not be.

---

## How the Scope Resolution Operator Works (`::`)

`::` is called the **scope resolution operator**. It means "look inside this class (or namespace) for this name."

### Static members vs instance members

When a member is declared `static` in a class, it belongs to the **class itself** — not to any individual object created from that class. There is only one copy of it, shared everywhere.

```cpp
class Pet {
public:
    static const int DEFAULT_HUNGRY = 30;  // belongs to Pet the class
    int getHungry() const;                 // belongs to each Pet object
};
```

The way you access each type of member reflects this difference:

```cpp
// Instance member — accessed via a dot on a specific object
pet.getHungry()        // pet is an object; getHungry() belongs to that object

// Static member — accessed via :: on the class name, no object needed
Pet::DEFAULT_HUNGRY    // no object required; it belongs to the class itself
```

### Why :: is needed outside the class

Inside `pet.cpp`, the compiler already knows it is working inside the `Pet` class context, so `DEFAULT_HUNGRY` on its own is enough. But in a different file like `storage_manager.cpp`, the compiler has no idea where `DEFAULT_HUNGRY` comes from unless you tell it:

```cpp
// Compiler does not know which class to look in — fails
prefs.getInt("hungry", DEFAULT_HUNGRY);

// Compiler knows to look inside Pet — works
prefs.getInt("hungry", Pet::DEFAULT_HUNGRY);
```

### You have already seen :: in this project

The same operator appears in other places you have already written:

```cpp
ActionType::FEED    // FEED is a value inside the ActionType enum class
STATE_IDLE          // plain enum values (no class keyword) do not need ::,
                    // but enum class values do — that is why ActionType needs it
```

Think of `::` as saying *"open this container and find the name inside it."* The container can be a class, an enum class, or a namespace — `::` works the same way in all three cases.

### Defining class functions outside the class body

In this project, every class is split across two files: a header (`.h`) that declares what the class has, and a source file (`.cpp`) that defines what each function actually does. When you write a function body in the `.cpp` file, you use `::` to tell the compiler which class that function belongs to.

```cpp
// In pet.h — the declaration (what exists)
class Pet {
public:
    void feed();
};

// In pet.cpp — the definition (what it does)
void Pet::feed() {
    hungry = hungry - 20;
    happy  = happy  + 15;
    constrainValues();
}
```

`Pet::feed()` means "this is the implementation of the `feed` function that belongs to the `Pet` class." Without the `Pet::` prefix, the compiler would treat it as a standalone free function with no connection to the class at all.

Once you are inside the function body — between the `{` and `}` — the compiler knows you are in `Pet` context. That is why you can write `hungry` and `constrainValues()` directly without any prefix. The `Pet::` on the function name is the door; everything inside is already in the room.

You can see this pattern used consistently throughout the project:

```cpp
void StorageManager::save(const Pet& pet) { ... }
void TimerManager::update(Pet& pet)       { ... }
void ActionMenu::confirmAction(...)       { ... }
```

Every one of those `ClassName::functionName` lines is the same thing: "here is the body for a function I declared inside this class."

---

## How the State Machine Works

### What is a "state"?

At any moment, the pet is doing exactly one thing — eating, sleeping, playing, being sick, or just waiting around. That "one thing it is currently doing" is called its **state**.

A state machine is a system that:
1. Keeps track of the current state.
2. Defines rules for when to switch from one state to another.
3. Can run different behaviour depending on which state is active.

### The three functions that split the responsibility

In `lib/Pet/pet.h` and `pet.cpp`, three functions handle all the state logic:

| Function | Job |
|---|---|
| `setState(newState)` | Changes the current state. Called by action methods like `feed()` and `play()`. |
| `getState()` | Returns the current state. Used by the display and any code that needs to know what the pet is doing. |
| `updateState()` | Called once per `loop()`. Checks the current state and decides if it is time to switch to a new one. |

### How a state change flows through the code

Here is what happens when the user presses Button A to feed the pet:

```
Button A pressed
  → menu.confirmAction() is called in main.cpp
    → executePetAction() calls pet.feed()
      → feed() calls setState(STATE_EATING)   ← state changes here
      → feed() adjusts hunger, happy, etc.
  → next loop() iteration
    → pet.updateState() runs
      → sees STATE_EATING
      → immediately calls setState(STATE_IDLE) ← returns to idle
```

Eating happens instantly in this project — `feed()` adjusts stats right away — so `updateState()` returns to `STATE_IDLE` on the very next frame. In a more advanced version, you could hold the pet in `STATE_EATING` for several seconds and show an eating animation before returning to idle.

### Why not just use a variable and an `if` statement?

You could write `if (isFeedingNow) { ... }`. For one action this works. But as soon as you have six states, you have six separate boolean variables that can accidentally all be `true` at once. The state machine using an `enum` guarantees exactly one state is active at a time — the compiler enforces it. It also makes the code much easier to read: `getState() == STATE_SLEEPING` is clearer than `isSleeping && !isEating && !isPlaying`.

The state list lives in `lib/Pet/pet.h` as the `PetState` enum. Reading through it gives you a complete picture of everything the pet can do.
