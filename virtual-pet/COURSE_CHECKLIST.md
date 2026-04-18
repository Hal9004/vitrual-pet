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

## Phase 3: Interaction & Menu System
- [x] Navigation Logic (Buttons B & C to cycle; Button A to confirm)
- [x] Menu UI (Visual indicators for selected actions)
- [x] Motion Play (Using MPU6886 accelerometer for "Play" mode interactions)
- [ ] Sound Feedback (Buzzer melodies for Level Up, Hunger, etc.)
- [ ] Voice Memos (Microphone recording/playback for short sound bites)

## Phase 4: Environmental & Advanced Features
- [ ] RTC (Real Time Clock) for overnight logic
- [ ] EEPROM/Preferences (Saving pet state on power off)
- [ ] Evolution Logic (Growth stages based on care/time)

## Phase 5: Connectivity & Polish
- [ ] Wireless Communication (BLE or WiFi for inter-device interaction)
- [ ] Remote Dashboard (Web/App stat checking via WiFi)
- [ ] Final UI Polish (Clean code comments, descriptive variable names)