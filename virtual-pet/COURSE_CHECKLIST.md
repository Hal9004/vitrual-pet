# Tamagotchi Project: Kid-Friendly Course Checklist

## Phase 1: Foundations & UI
- [x] Hardware Initialization (M5.begin, LCD, Serial)
- [x] Asset Pipeline (Converting images/gifs to C++ arrays/sprites) — C++ piskel_converter tool, SPRITE_GUIDE.md student walkthrough, SPRITE_TEST flag in main.cpp. RGB565 values are pre-byte-swapped to match the M5StickC Plus 2 LCD's big-endian SPI byte order (transparent key: 0x1FF8)
- [x] Basic Sprite Rendering (Drawing the pet to the screen) — `DisplayManager::drawPetSprite()` renders bitmap sprites via `M5.Lcd.pushImage()` with transparent key `0x1FF8`. A single 80×80 sprite is used on the Main and Interact screens; the Stats screen is a pure data view with no sprite. Sprite headers live in `lib/Display/sprites/`; raw Piskel exports in `assets/sprites/raw/`
- [x] Screen Real Estate Management (Layout for stats vs. pet area)
- [x] Multi-Screen Framework — three screens (Main, Stats, Interact) with `ScreenState` enum and `NavigationManager`. B → Interact, C → Stats from the Main screen.
- [x] Stats Detail Screen — `SCREEN_STATS` shows all five stat bars + pet face + mood, identical to the original layout.
- [x] Pet Interaction Screens — `SCREEN_INTERACT` shows pet face + contextual stat bar (the stat the selected action affects) + action menu at the bottom.

## Phase 2: Core Logic & State Machine
- [x] State Machine Architecture (IDLE, EATING, SLEEPING, EVOLVING)
- [x] State Machine — Full Action Coverage (PLAYING, SICK, HEALING, BATHING)
- [x] Hunger Logic (Timer-based decrement)
- [x] Happiness Logic (Timer-based decrement)
- [x] Energy/Sleep Logic (Recovery vs. Depletion)
- [x] Death/Reset Condition (Handling 0 stats)
- [x] Cleanliness Decay Logic (Timer-based decrement)
- [x] Sickness Accumulation Logic (Rises when cleanliness is low)
- [ ] Sadness Logic — revisit when sprites are implemented. Idea: `sad` rises automatically when `happy` falls below a threshold, triggering a dedicated sad sprite. The stat and getter/setter already exist in `Pet` — needs a timer rule in `TimerManager` and a matching sprite in the asset pipeline.

## Phase 3: Interaction & Menu System
- [x] Navigation Logic (Buttons B & C to cycle; Button A to confirm)
- [x] Menu UI (Visual indicators for selected actions)
- [x] Motion Play (Using MPU6886 accelerometer for "Play" mode interactions)
- [x] Sound Feedback (Buzzer melodies for Level Up, Hunger, etc.)
- [ ] Microphone Input (Detect & React) — right-sized from the original "Voice Memos" during the Task 14b audit. Foundation: detect a loud noise (clap/voice/whistle), pet reacts with happiness +5 and a buzzer chirp. Full record/playback becomes Bonus Feature 6 (`DEV_ROADMAP.md` Appendix B).

## Phase 4: Environmental & Advanced Features
- 🔁 RTC (Real Time Clock) — moved to bonus during the Task 14b audit. Without overnight-decay logic, a clock widget does not integrate with any other module. Full design in `DEV_ROADMAP.md` Appendix B — Bonus Feature 1.
- [x] NVS Persistence via `Preferences` (Saving pet state on power off) — Note: the checklist originally said "EEPROM" but the ESP32 has no real EEPROM. We used NVS (Non-Volatile Storage) directly via the Arduino `Preferences` library, which is the correct ESP32-native approach.
- [ ] Evolution Logic (Growth stages based on care/time) — deferred, lower priority than EEPROM

## Phase 5: Connectivity & Polish
- [ ] Wireless Access Point Primitive — right-sized from the original "Wireless Communication" during the Task 14b audit. Foundation: device broadcasts a WiFi hotspot, displays SSID + IP on the LCD, phone can connect. Two-device comms (ESP-NOW), BLE, and an HTTP server all become bonus features (`DEV_ROADMAP.md` Appendix B).
- 🔁 Remote Dashboard — moved to bonus during the Task 14b audit. The web server pattern is its own slug of new concepts (routes, named callbacks, `String` HTML building) and is better as a follow-on bonus than a mandatory task. Full design in `DEV_ROADMAP.md` Appendix B — Bonus Feature 2.
- [ ] Final UI Polish (Clean code comments, descriptive variable names)

## Phase 6: Student Template Creation
- [ ] Review completed codebase against the lesson plan
- [ ] Break each module into a skeleton template — preserve function signatures, comments, and example implementations where helpful, but leave the core logic for students to write themselves
- [ ] Verify each template compiles and provides enough scaffolding for a student to complete it without being lost
- [ ] Produce one template per complexity level (Levels 1–7 from the roadmap) so teachers can assign tasks appropriate to each student's skill