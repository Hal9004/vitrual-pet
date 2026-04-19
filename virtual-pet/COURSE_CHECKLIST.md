# Tamagotchi Project: Kid-Friendly Course Checklist

## Phase 1: Foundations & UI
- [x] Hardware Initialization (M5.begin, LCD, Serial)
- [ ] Asset Pipeline (Converting images/gifs to C++ arrays/sprites)
- [ ] Basic Sprite Rendering (Drawing the pet to the screen)
- [x] Screen Real Estate Management (Layout for stats vs. pet area)

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
- [ ] Voice Memos (Microphone recording/playback for short sound bites)

## Phase 4: Environmental & Advanced Features
- [ ] RTC (Real Time Clock) for overnight logic
- [x] NVS Persistence via `Preferences` (Saving pet state on power off) — Note: the checklist originally said "EEPROM" but the ESP32 has no real EEPROM. We used NVS (Non-Volatile Storage) directly via the Arduino `Preferences` library, which is the correct ESP32-native approach.
- [ ] Evolution Logic (Growth stages based on care/time) — deferred, lower priority than EEPROM

## Phase 5: Connectivity & Polish
- [ ] Wireless Communication (BLE or WiFi for inter-device interaction)
- [ ] Remote Dashboard (Web/App stat checking via WiFi)
- [ ] Final UI Polish (Clean code comments, descriptive variable names)

## Phase 6: Student Template Creation
- [ ] Review completed codebase against the lesson plan
- [ ] Break each module into a skeleton template — preserve function signatures, comments, and example implementations where helpful, but leave the core logic for students to write themselves
- [ ] Verify each template compiles and provides enough scaffolding for a student to complete it without being lost
- [ ] Produce one template per complexity level (Levels 1–7 from the roadmap) so teachers can assign tasks appropriate to each student's skill