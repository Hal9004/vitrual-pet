# Virtual Pet — Teaching Notes

This file explains the "why" behind concepts that appear in the code but are not obvious from reading the code alone. These are not instructions for building features — those live in `DEV_ROADMAP.md`. This is for when you finish a task and find yourself asking *"but how does that actually work?"*

---

## Why the Code is Split Into Separate Modules

### The problem with putting everything in one file

Imagine the entire virtual pet — display, buttons, sounds, timers, pet logic, storage — written as one giant `main.cpp`. It would be thousands of lines long. To fix a bug in the buzzer, you would have to search through display code and timer code to find it. To add a new action, you would have to understand the whole file before touching any of it. Every change would risk breaking something unrelated.

This is a real problem that professional engineers hit constantly. The solution is **modularisation**: split the code into separate files, where each file has one clearly defined job and does not reach into other files' business.

### The Single Responsibility Principle

Each `lib/` module in this project follows a simple rule:

> **One module. One job. One reason to change.**

This is called the **Single Responsibility Principle** — one of the most important ideas in software design.

| Module | Its one job |
|---|---|
| `Pet` | Store and update pet stats — hunger, happiness, energy, and so on |
| `DisplayManager` | Draw things to the screen — nothing else |
| `ButtonHandler` | Detect when buttons are pressed — nothing else |
| `ActionMenu` | Track which action is selected and confirm it — nothing else |
| `TimerManager` | Apply automatic stat changes over time — nothing else |
| `ImuManager` | Detect shake gestures from the accelerometer — nothing else |
| `SpeakerManager` | Play sounds and melodies — nothing else |
| `StorageManager` | Save and load pet stats to flash memory — nothing else |
| `NavigationManager` | Track which screen is active and handle top-level navigation — nothing else |

When a module has one job, you know exactly where to look when something goes wrong. Buzzer broken? Open `speaker_manager.cpp`. Pet not saving? Open `storage_manager.cpp`. You never have to read unrelated code to fix a focused problem.

### How the modules link together

None of the modules talk to each other directly. They each do their job, and `src/main.cpp` is the **orchestrator** — it holds one instance of each module and passes them to each other as needed.

```
                        src/main.cpp
                       (orchestrator)
                            │
         ┌──────────────────┼──────────────────┐
         │                  │                  │
         ▼                  ▼                  ▼
   ButtonHandler      NavigationManager    TimerManager
   (reads buttons)    (tracks screen)     (decay timers)
         │                  │                  │
         │            ┌─────┘                  │
         │            ▼                        │
         │       ActionMenu              ┌─────┘
         │       (cycles actions)        │
         │            │                  ▼
         │            └──────────►      Pet
         │                        (stats + state)
         │                              │
         ▼                              ▼
   ImuManager                    DisplayManager
   (shake gesture)                (draws screen)
         │                              │
         └──────────────────────────────┘
                       │
                       ▼
                 SpeakerManager
                 (plays sounds)
                       │
                       ▼
                 StorageManager
                 (saves to flash)
```

When `main.cpp` needs `ButtonHandler` to tell `ActionMenu` about a button press, it passes the `ButtonHandler` object into `ActionMenu::update()` as a parameter. The modules never hold permanent references to each other — they only borrow what they need for the duration of one function call. This keeps the dependencies loose and easy to follow.

### What is Object-Oriented Programming?

Each module is a **class** — a blueprint that describes:
- What data it holds (its **member variables**)
- What it can do (its **member functions**, also called methods)

When `main.cpp` creates a `Pet` object, it is like using that blueprint to build a real pet. You can have multiple objects from the same class — if you wanted two pets, you would create two `Pet` objects from the same `Pet` class.

```cpp
// The class is the blueprint
class Pet {
    int hungry;       // data it holds
    void feed();      // things it can do
};

// The object is the real thing built from the blueprint
Pet myPet;           // one pet
Pet rivalPet;        // another pet — same blueprint, separate data
```

The key idea is **encapsulation**: the data inside a class is hidden from the outside world. Other code cannot reach in and change `Pet::hungry` directly — it has to use the methods the `Pet` class provides (`getHungry()`, `setHungry()`). This means the `Pet` class is always in control of its own data and can enforce rules like "hunger can never go below 0 or above 100."

### What is Inheritance — and why this project does not use it

Inheritance is a way to build a new class by extending an existing one. The new class **inherits** all the data and behaviour of the original, and can add or override things on top.

```cpp
// A general Animal class
class Animal {
public:
    int hungry;
    void eat() { hungry = hungry - 10; }
};

// Dog inherits everything from Animal, and adds its own behaviour
class Dog : public Animal {
public:
    void bark() { /* play bark sound */ }
};

// Dog objects can eat() (inherited) AND bark() (their own)
Dog myDog;
myDog.eat();   // inherited from Animal
myDog.bark();  // Dog's own method
```

Inheritance is useful when you have several things that are mostly the same but differ in specific ways — for example, `Cat`, `Dog`, and `Bird` all sharing an `Animal` base class.

**This project intentionally avoids inheritance.** All nine modules are completely independent — a `DisplayManager` is not a kind of `Pet`, and a `SpeakerManager` is not a kind of `TimerManager`. Forcing inheritance where there is no real "is-a" relationship makes code harder to read and harder to change. Using separate, unrelated classes and connecting them through `main.cpp` (called **composition**) is the right approach here, and is actually preferred in most professional embedded C++ code for exactly this reason.

A useful rule of thumb:
- Use **inheritance** when one thing genuinely *is a kind of* another thing (a `Dog` is an `Animal`).
- Use **composition** (separate classes connected through `main.cpp`) when things *work together* but are not the same kind of thing.

In this project, everything is composition. You will encounter inheritance in future projects — now you know what to look for and why it exists.

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

---

## Why `static const` Structs Need a Definition in the `.cpp` File

### The short version

`static const int` members are special — the compiler treats them as compile-time numbers and substitutes them directly into the machine code. `static const` members of any other type (like a `struct`) have to exist in memory as real objects, so they need a definition in a `.cpp` file or the linker will complain.

### What "declaration" vs "definition" means

Every piece of code in C++ exists in two parts:

- A **declaration** (in the `.h` file) is a *promise* — it tells the compiler "this thing exists and here is what it looks like."
- A **definition** (in the `.cpp` file) is where the thing is *actually created* — memory is allocated, values are assigned.

For most class members, the constructor handles this automatically. But `static` members belong to the class itself, not to any object, so the constructor never runs for them. You have to provide the definition manually.

### Why `static const int` gets away without a `.cpp` definition

When you write this in a header:

```cpp
static const int SCREEN_WIDTH = 135;
```

The compiler treats `135` as a compile-time constant — like a smarter `#define`. Wherever `SCREEN_WIDTH` appears in your code, the compiler substitutes `135` directly into the machine code at compile time. It never needs to find a memory address for it, because it never puts it in memory at all.

This is called **constant folding**, and it is why `static const int` is a special case that does not need a `.cpp` definition.

### Why `static const ScreenZone` is different

```cpp
static const ScreenZone ZONE_PET_FACE = { 0, 20, 135, 185 };
```

`ScreenZone` is a struct — it has four separate `int` fields. The compiler cannot substitute it as a single number. It has to exist somewhere in memory as a real object with an address, so that code can read `.x`, `.y`, `.width`, and `.height` from it.

If you only declare it in the header and never define it in a `.cpp` file, the compiler will not warn you — it trusts your promise. But when the linker tries to build the final program, it searches all the compiled files looking for "where is `DisplayManager::ZONE_PET_FACE` actually stored?" and finds nothing. That produces a **linker error** — a subtly different kind of failure from a normal compile error.

### The fix — one line in the `.cpp` file

```cpp
// In display_manager.cpp
const ScreenZone DisplayManager::ZONE_PET_FACE = { 0, 20, 135, 185 };
```

This is the definition. It tells the linker: "here is where `ZONE_PET_FACE` actually lives in memory, and here are its values." The `DisplayManager::` prefix (the scope resolution operator, explained in an earlier section) tells the compiler this belongs to the `DisplayManager` class — the same pattern used for every function body in the project.

### The analogy

Think of the header file as a restaurant menu and the `.cpp` file as the kitchen.

A `static const int` is like a price printed directly on the menu — it is just a number, right there on the page, no kitchen required.

A `static const ScreenZone` is like a dish listed on the menu — the menu describes it, but someone in the kitchen has to actually prepare it before a customer can have it. The `.cpp` definition is the kitchen preparing the dish.

### When you will see this pattern

Any time you add a `static const` class member that is not a plain integer or boolean, you will need a matching definition in the `.cpp` file. This applies to structs, strings, arrays, and any other non-trivial type. The pattern is always the same:

```cpp
// Header — the promise
static const YourType MEMBER_NAME;

// .cpp — the definition
const YourType ClassName::MEMBER_NAME = { ... };
```

### The shortcut — `constexpr`

There is a smarter way to write struct constants that skips the extra `.cpp` line entirely. Just swap `static const` for `static constexpr`:

```cpp
// This works on its own in the header — no .cpp line needed
static constexpr ScreenZone TITLE_ZONE = { 0, 5, 135, 19 };
```

`constexpr` is short for "constant expression". It is your way of telling the compiler: **"I promise this value will never change — ever."**

When the compiler sees that promise, it works out the value while it is building your program, before the program ever runs on the device. It then bakes the answer directly into the code.

After the compiler finishes, a second tool called the **linker** runs. The linker's job is to take all the separate compiled pieces and join them into one final program. Without `constexpr`, the linker would go searching for your struct in memory and fail — because you never put it in a `.cpp` file. With `constexpr`, the value was already baked in during the build step, so the linker never needs to search for it.

This is what the zone constants in `display_manager.h` use. Whenever you add a struct constant to a class, use `constexpr` and you will not need the extra `.cpp` line at all.

---

## Why Header Files Start With `#ifndef`

Open any `.h` file in this project and you will see the same three lines at the very top and bottom:

```cpp
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

// ... all the actual code ...

#endif
```

These three lines are called an **include guard**. Here is why they exist.

### The problem — including a file twice

When your program is built, the compiler reads each `.cpp` file one at a time. In `main.cpp` you write `#include "pet.h"`. That tells the compiler: "copy everything in `pet.h` into this file right now."

But what if two different files both include `pet.h`? What if `main.cpp` includes `pet.h` and also includes `action_menu.h`, and `action_menu.h` also includes `pet.h`? The compiler would end up seeing the `Pet` class definition twice in the same file. In C++, defining the same thing twice is an error — the compiler will refuse to build and print a confusing message.

### How include guards fix it

`#ifndef` means **"if not defined"**. The three lines together say:

1. `#ifndef DISPLAY_MANAGER_H` — "have I seen the name `DISPLAY_MANAGER_H` before?"
2. `#define DISPLAY_MANAGER_H` — "if not, remember this name now"
3. `#endif` — "skip everything between these lines if the name was already known"

The first time the compiler reads `display_manager.h`, it has never seen `DISPLAY_MANAGER_H`, so it reads all the code inside and remembers the name. If anything tries to include the same file a second time, the compiler sees the name is already defined and skips the whole file. The class only gets defined once, no matter how many times the file is included.

### The name in the guard

The name used in the guard — `DISPLAY_MANAGER_H` — is just a convention. It matches the filename in ALL CAPS with the `.` replaced by `_`. As long as the name is unique across your whole project, it works. That is why every header uses a different name:

```cpp
#ifndef PET_H            // pet.h
#ifndef DISPLAY_MANAGER_H  // display_manager.h
#ifndef ACTION_MENU_H    // action_menu.h
```

### The short version

Every `.h` file needs an include guard. Without one, including the same header twice causes a build error. The `#ifndef` / `#define` / `#endif` pattern is the standard way to prevent that. You will see it at the top and bottom of every header file you ever write.

---

## How Multi-Screen Apps Are Structured

### What is a "screen" in embedded code?

In a web app or phone app, a "screen" is usually a whole new page loaded from a server. On a tiny device like the M5StickC Plus 2, there is no server and no pages — there is just one 135×240 pixel display that you draw on directly.

A "screen" in embedded code is just a **set of decisions about what to draw**. The Stats screen draws five stat bars and a pet face. The Interact screen draws a pet face, one stat bar, and an action menu. The Main screen draws a pet face and a tab bar. The hardware never changes — only what gets painted on it does.

### The `ScreenState` enum

The simplest way to track which screen is active is with an enum:

```cpp
enum ScreenState {
    SCREEN_MAIN,
    SCREEN_STATS,
    SCREEN_INTERACT
};
```

An enum is just a named list of options. Using an enum instead of a plain integer (0, 1, 2) makes the code readable: `SCREEN_STATS` tells you exactly what it means, while `1` means nothing on its own.

`ScreenState` lives in `lib/Display/screen_layout.h` because it is a display concept — it describes what the screen is showing. Any file that needs to know the current screen can include this header.

### How the three screens are structured

| Screen | What it shows | How you leave it |
|---|---|---|
| `SCREEN_MAIN` | Pet face, mood, nav bar (Stats / Interact) | Press B → Interact, Press C → Stats |
| `SCREEN_STATS` | All five stat bars + pet face + mood | Press any button → back to Main |
| `SCREEN_INTERACT` | Pet face, mood, one contextual stat bar, action menu | Confirm "Back" action → Main |

---

## How Screen Layouts Are Designed

### Thinking in zones

Before writing a single line of drawing code, it helps to divide the screen into named **zones** — rectangular regions, each with one job. This is the same idea a graphic designer uses when laying out a page: title area, content area, navigation area.

For this project, the Stats screen zones look like this:

```
┌─────────────────┐  y: 0
│   TITLE_ZONE    │     5 – 24    pet name
│   STATS_ZONE    │    26 – 133   five stat bars
│  PET_FACE_ZONE  │   134 – 169   pet face
│   MOOD_ZONE     │   180 – 198   mood text
│   MENU_ZONE     │   220 – 240   action / hint
└─────────────────┘  y: 240
```

Each zone is stored as a `ScreenZone` struct — four numbers that describe its top-left corner and its size:

```cpp
static constexpr ScreenZone TITLE_ZONE = { 0, 5, 135, 19 };
//                                         x  y  w    h
```

Using `constexpr` (explained in the earlier section) means these values are baked in at compile time and do not need a `.cpp` definition.

### Why define layout as data, not code

Without zone structs, drawing code looks like this:

```cpp
M5.Lcd.setCursor(5, 26);   // where is 26? no idea without comments
M5.Lcd.setCursor(5, 48);   // and 48?
M5.Lcd.setCursor(5, 70);   // and 70?
```

These **magic numbers** have no meaning to a reader. Change the layout and you have to hunt for every hardcoded pixel value across the whole file.

With zone structs:

```cpp
M5.Lcd.setCursor(STATS_ZONE.x, HAPPY_BAR_ZONE.labelY);
M5.Lcd.setCursor(STATS_ZONE.x, HUNGER_BAR_ZONE.labelY);
M5.Lcd.setCursor(STATS_ZONE.x, ENERGY_BAR_ZONE.labelY);
```

Now the intent is visible. If you want to move all the stat bars down by 10 pixels, you change one constant — not six scattered lines.

### Each screen can have its own zones

The Stats screen and the Interact screen need the pet face in different positions — the Interact screen has no stat bars above the face, so the face can sit higher up and be drawn larger.

Rather than one shared `PET_FACE_ZONE`, each screen defines its own face centre and radius:

```cpp
// Stats screen — face sits below the stat bars
static constexpr ScreenZone PET_FACE_ZONE = { 0, 134, 135, 36 };

// Interact screen — face gets more vertical room
static constexpr int INTERACT_FACE_CENTER_Y = 90;
static constexpr int INTERACT_FACE_RADIUS   = 26;
```

This is not duplication — it is each screen owning its own layout. The Stats face and the Interact face are deliberately different sizes.

---

## Why DisplayManager Does Not Decide What to Draw

### The old approach

Before Task 11a, `DisplayManager` tracked its own internal `DisplayState` enum:

```cpp
enum class DisplayState {
    STATUS_VIEW,
    MENU_INDICATOR,
    DEAD
};
```

This meant `DisplayManager` was making two decisions at once: *what to draw* and *how to draw it*. That is two jobs — a violation of the Single Responsibility Principle.

Worse, `STATUS_VIEW` and `MENU_INDICATOR` were not really different screens — they were both the same stats layout, just with or without the action indicator drawn over the top. As the number of real screens grew, this approach would have needed a new `DisplayState` value for every combination of layout and overlay.

### The new approach — the caller decides, DisplayManager executes

Now `renderDisplay()` receives a `ScreenState` from the caller:

```cpp
void DisplayManager::renderDisplay(..., ScreenState screenState) {
    switch (screenState) {
        case SCREEN_MAIN:     renderMainScreen(...);     break;
        case SCREEN_STATS:    renderStatsScreen(...);    break;
        case SCREEN_INTERACT: renderInteractScreen(...); break;
    }
}
```

`DisplayManager` no longer guesses what the player is looking at. `NavigationManager` tracks that, and passes the answer in. `DisplayManager` just draws.

This separation — **one module decides, another executes** — is a pattern called **separation of concerns**. Each module concerns itself with exactly one question:

- *What screen is the player on?* → `NavigationManager`'s job
- *How do I draw that screen?* → `DisplayManager`'s job

---

## How NavigationManager Keeps `loop()` Clean

### The problem without it

Without `NavigationManager`, all the screen-switching logic would live inside `loop()` as a large `switch` statement. Every time a new screen was added, `loop()` would grow longer. After three or four screens, `loop()` would be difficult to read and easy to break — it would be doing too many jobs at once.

### The solution — delegate to a module

`NavigationManager` owns one piece of state: which screen the player is currently on. Its `update()` method reads button input and switches screens when needed. `loop()` becomes two lines:

```cpp
navManager.update(buttons, menu);

if (navManager.shouldConfirmAction()) {
    menu.confirmAction(myPet, display, speaker, storage);
}
```

The switch statement still exists — it just lives inside `NavigationManager::update()` where it belongs, rather than mixed into the main game loop.

### One handler per screen

Inside `NavigationManager`, each screen gets its own private handler method:

```cpp
void handleMainScreenInput(const ButtonHandler& buttons);
void handleStatsScreenInput(const ButtonHandler& buttons);
void handleInteractScreenInput(const ButtonHandler& buttons, const ActionMenu& menu);
```

Each handler only reads the buttons that are relevant on that screen. `handleStatsScreenInput()` has no idea what `handleInteractScreenInput()` does. If you need to understand what the buttons do on the Stats screen, you open that one method and read four lines. Nothing else to search through.

When a new screen is added in the future, you add one new handler method and one new `case` in `update()`. Everything else stays the same.

---

## The One-Shot Flag Pattern

### The problem

When the player presses Button A to feed the pet, `confirmAction()` should fire **once**. But `loop()` runs hundreds of times per second. If you stored "A was pressed" as a boolean and never cleared it, `confirmAction()` would fire on every single frame until the player released the button — feeding the pet hundreds of times in one press.

### The solution — reset at the top of every frame

`NavigationManager` tracks this with a `confirmActionRequested` flag. The key is where it gets reset:

```cpp
void NavigationManager::update(const ButtonHandler& buttons, const ActionMenu& menu) {
    confirmActionRequested = false;  // reset FIRST, before checking anything

    switch (currentScreen) {
        ...
    }
}
```

By resetting `confirmActionRequested` to `false` at the very start of `update()`, the flag can only ever be `true` for **one loop iteration** — the single frame where the player's button press was detected. On the next frame, `update()` resets it back to `false` before anything else runs.

This is called a **one-shot flag**: it fires once and immediately resets itself. You will see this pattern used in `ButtonHandler` (`wasButtonAPressed()` returns `true` only once per press), in `Pet` (`checkDeathAlert()` returns `true` only on the first frame of death), and now in `NavigationManager`. It is one of the most useful patterns in game loop programming.

---

## Partial Screen Redraws — Why Not Draw Everything Every Frame?

### The problem with redrawing everything

The M5StickC Plus 2 LCD is driven over SPI — a serial connection that sends pixel data one bit at a time. A full 135×240 screen clear takes a noticeable fraction of a second. If you called `clearScreen()` every frame, the display would flash black 60 times per second. This is called **screen flicker**, and it makes the device feel broken.

### The solution — only redraw what changed

`DisplayManager` uses two techniques to avoid this.

**1. Full redraw throttle**

A full redraw — clearing the screen and drawing everything from scratch — only happens when the screen changes or every 5 seconds:

```cpp
bool screenChanged   = (lastRenderedScreen != SCREEN_INTERACT);
bool intervalElapsed = (millis() - lastFullRedrawTime >= STATUS_UPDATE_INTERVAL);

if (screenChanged || intervalElapsed) {
    clearScreen();
    // ... draw everything ...
}
```

Between full redraws, the screen just holds whatever was drawn last. Stat values change slowly enough that a 5-second stale display is fine.

**2. Fast path for elements that change quickly**

Some things change immediately — like the action menu selection when the player presses B or C. For these, only the small area that changed is redrawn. On the Interact screen:

```cpp
if (menu.getCurrentActionIndex() != lastMenuActionIndex) {
    drawContextualStatBar(...);   // redraw only the stat bar strip
    drawMenuIndicator(...);       // redraw only the menu zone
    lastMenuActionIndex = menu.getCurrentActionIndex();
}
```

This erases and redraws only the bottom ~40 pixels of the screen. The pet face and mood text above are untouched and never flicker.

The general rule: **redraw the whole screen rarely; redraw small regions immediately**. Track what changed using `last...` variables, and only draw when the value differs from what is already on screen.

---

## How Sprite Images Are Stored and Drawn

Before reading this section, make sure you have read `SPRITE_GUIDE.md`. That guide
covers how to create and convert sprites step by step. This section explains the
*why* behind the technical choices — why images cannot just be loaded from a file,
what RGB565 and ARGB8888 actually mean, and why the `PROGMEM` keyword matters.

### Why not just load a PNG or JPEG?

On a laptop or phone, loading an image is simple: the OS reads the file, a decoder
library unpacks it into pixels, and the display driver draws them. The M5StickC Plus 2
has none of those layers. There is no OS, no filesystem by default, and no image decoder
in memory. The only storage available during a normal program run is the code itself —
sitting in flash memory — and a small amount of RAM.

The solution is to convert the image into a raw array of pixel values *before* the
program is compiled, and bake that array directly into the firmware. When the program
runs, the pixel data is already sitting in flash, ready to draw without any decoding step.
That is what the `piskel_converter` tool does.

### Two colour formats: ARGB8888 and RGB565

Every digital colour is made up of red, green, and blue channels mixed together.
The difference between formats is how many bits are used to describe each channel,
and whether an alpha (transparency) channel is included.

**ARGB8888 — what Piskel exports**

Piskel stores each pixel as a 32-bit number: 8 bits for alpha, 8 for red, 8 for green,
8 for blue. That gives 256 possible values per channel — fine colour detail, but 4 bytes
per pixel.

```
One pixel in ARGB8888 — 32 bits (4 bytes):

Bit: 31      24 23      16 15       8 7        0
     ┌─────────┬──────────┬──────────┬──────────┐
     │  Alpha  │   Red    │  Green   │   Blue   │
     │  8 bits │  8 bits  │  8 bits  │  8 bits  │
     └─────────┴──────────┴──────────┴──────────┘

Example: 0xFF5C996E
  Alpha = 0xFF = 255 → fully opaque
  Red   = 0x5C = 92
  Green = 0x99 = 153
  Blue  = 0x6E = 110
  Result: a sage green colour
```

**RGB565 — what the M5StickC Plus 2 LCD needs**

The LCD controller uses a 16-bit format: 5 bits for red, 6 bits for green (green gets
an extra bit because human eyes are most sensitive to it), and 5 bits for blue.
No alpha channel — the LCD has no concept of transparency in hardware.
That is 2 bytes per pixel instead of 4, which halves the memory cost.

```
One pixel in RGB565 — 16 bits (2 bytes):

Bit: 15    11 10     5 4      0
     ┌───────┬────────┬────────┐
     │  Red  │ Green  │  Blue  │
     │ 5 bits│ 6 bits │ 5 bits │
     └───────┴────────┴────────┘

Example: 0x5CCD
  Red   = 0b01011 = 11
  Green = 0b100110 = 38
  Blue  = 0b01101 = 13
  Result: the same sage green, using half the memory
```

### How the converter translates between the two formats

The conversion happens in three steps for each pixel:

```
Step 1 — Check alpha.
  If alpha == 0, the pixel is transparent.
  Write the colour key (0xF81F) and stop.

Step 2 — Extract the colour channels from the 32-bit ARGB value.
  Red   = (pixel >> 16) & 0xFF   → 8-bit value, 0–255
  Green = (pixel >>  8) & 0xFF   → 8-bit value, 0–255
  Blue  = (pixel >>  0) & 0xFF   → 8-bit value, 0–255

Step 3 — Shrink each channel to fit the RGB565 bit widths.
  Red5   = Red   >> 3   (keep top 5 bits, discard bottom 3)
  Green6 = Green >> 2   (keep top 6 bits, discard bottom 2)
  Blue5  = Blue  >> 3   (keep top 5 bits, discard bottom 3)

Step 4 — Pack the three channels into one 16-bit value.
  rgb565 = (Red5 << 11) | (Green6 << 5) | Blue5

Example with 0xFF5C996E:
  Red   = 0x5C = 92  → 92  >> 3 = 11  → 0b01011
  Green = 0x99 = 153 → 153 >> 2 = 38  → 0b100110
  Blue  = 0x6E = 110 → 110 >> 3 = 13  → 0b01101

  Pack: (11 << 11) | (38 << 5) | 13 = 0x5CCD ✓
```

You can see this exact logic in `tools/piskel_converter/main.cpp` inside the
`convertArgbToRgb565()` function.

### The transparent colour key

The LCD has no hardware transparency. When the drawing code calls `pushImage()`, it
draws every pixel it is given — there is no automatic "skip this pixel" behaviour.

To get transparency, the converter replaces every pixel with alpha == 0 with a special
**colour key** value: magenta (`0xF81F`). The drawing call then receives that value as
the "transparent" argument and skips any pixel that matches it.

```
Why magenta?

  Magenta (0xF81F) in RGB565:
    Red   = 0b11111 = 31  (maximum)
    Green = 0b000000 = 0  (none)
    Blue  = 0b11111 = 31  (maximum)

  It is the most visually distinct colour from anything likely to appear in a
  pet sprite. Using black (0x0000) as the key would make any genuinely black
  pixel in the sprite also transparent — undesirable. Magenta is the convention.
```

If a magenta pixel ever appears in your finished sprite where you did not intend
transparency, check your source artwork — you may have an unintentionally transparent
pixel that the converter has replaced with the key colour.

### PROGMEM — storing sprite data in flash, not RAM

The ESP32-PICO-V3-02 has two kinds of memory:

```
Flash memory:  4 MB   — large, read-only, stores your firmware and sprite arrays
RAM:         ~200 KB  — small, read-write, used by running code, the display
                        buffer, the stack, and any variables your program creates
```

A single 32×32 sprite at 2 bytes per pixel = 2,048 bytes. Seven states of sprites =
~14 KB. That is manageable in either location, but if you add animation frames later
(Task 13), the size grows quickly. Storing sprite arrays in flash by default is the
safe habit to build now.

The `PROGMEM` keyword (from `<pgmspace.h>`) marks an array for flash storage:

```cpp
// Without PROGMEM — may end up in RAM
static const uint16_t sprite_pet_idle[1][1024] = { ... };

// With PROGMEM — guaranteed to live in flash
static const uint16_t PROGMEM sprite_pet_idle[1][1024] = { ... };
```

On the ESP32, flash memory is memory-mapped — the CPU can read it using normal array
syntax, the same as RAM. You do not need any special function calls to access the data.
`PROGMEM` is included as a clear signal of intent: this data is read-only and should
not be copied into RAM.

---

## Button Guards — Only Respond When Visible

### The hidden side-effect problem

`menu.update(buttons)` reads Button B and Button C to cycle through the action list. But Button B and C are also used on the Main screen to switch between screens.

If `menu.update(buttons)` ran on every frame regardless of which screen was active, pressing B on the Main screen would do two things at once:
1. Switch to the Interact screen (handled by `NavigationManager`)
2. Silently advance the action selection in the menu (handled by `menu.update()`)

The player would arrive at the Interact screen with a different action already selected — and have no idea why.

### The fix — check the screen before passing input

In `loop()`, `menu.update()` is wrapped in a guard:

```cpp
if (navManager.getCurrentScreen() == SCREEN_INTERACT) {
    menu.update(buttons);
}
```

The action menu only reads buttons when it is actually visible. On any other screen, the menu ignores input entirely.

This is a general rule for multi-screen apps: **UI elements should only respond to input when they are on screen**. An invisible menu, dialog, or button that silently responds to input is a bug waiting to happen. Always guard input handling with a screen check.
