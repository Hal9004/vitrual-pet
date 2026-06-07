# Sprite Creation Guide

This guide walks you through the full process of creating a sprite for the virtual pet —
from drawing it in Piskel to having it ready to use in the firmware.

For the technical explanation of why images are stored the way they are (RGB565, PROGMEM,
transparent colour keys), see the **"How Sprite Images Are Stored and Drawn"** section
in `USEFUL_NOTES.md`.

---

## The Workflow at a Glance

```
1. Draw your sprite in Piskel       (piskelapp.com, 32×32 canvas)
            |
2. Export as a C file from Piskel   (Export -> C file -> Download)
            |
3. Rename and move the file         (-> assets/sprites/raw/pet_idle.c)
            |
4. Run the converter tool           (tools/piskel_converter/piskel_converter)
            |
5. The .h file appears              (-> lib/Display/sprites/pet_idle.h)
            |
6. #include it in the display code  (Task 13)
```

---

## Part 1 — Drawing Your Sprite in Piskel

### Setting up the canvas

1. Go to [piskelapp.com](https://www.piskelapp.com/p/create/sprite) and click **Create Sprite**.
2. In the right-hand panel, click the **Resize** icon (a square with arrows in the corners).
3. Set **Width: 32** and **Height: 32**, then click **Resize**.
4. Set the **Frames** count at the bottom to **1**. We are drawing one static image per
   pet state for now — animation frames come in Task 13a.

The canvas is now 32×32 pixels. Each small square you paint is one pixel on the device screen.

---

### The anchor point rule

**The pet's body must occupy roughly the same region of the canvas in every sprite.**

This does not mean every sprite must look the same. Different poses, angles, and expressions
are fine — and expected. What you must keep consistent is *where the body sits within the canvas*.

Here is why. When the pet changes state — say from IDLE to EATING — the rendering code
replaces one sprite with another at the exact same screen position. If the body has shifted
within the canvas, the pet appears to jump on screen even though the code is correct.

#### Good example — different poses, consistent anchor

The pet body occupies roughly the same region of the canvas (columns 3–6, rows 1–4)
in every state. The details change freely: eyes, mouth, arms, sleeping pose.

```
     IDLE                EATING              PLAYING
. . . . . . . .      . . . . . . . .      . . . . . . . .
. . . # # # . .      . . . # # # . .      . . # . . # . .   <-- arms raised
. . . # o # . .      . . . # - # . .      . . . # # # . .   <-- body same region
. . . # # # . .      . . . # # \ . .      . . . # o # . .
. . . . | . . .      . . . . | . . .      . . . # # # . .
. . . . . . . .      . . . . . . . .      . . . . | . . .
. . . . . . . .      . . . . . . . .      . . . . . . . .
```

On screen, the transitions look like this. The pet stays in place:

```
         IDLE        EATING      PLAYING

Screen:  [###]  ->   [###]  ->   [# #]
         [#o#]       [#-#]       [###]
         [###]       [##\]       [#o#]
          | |         | |        [###]
                                   |
         No jump. The pet stays in the same screen position every time.
```

#### Bad example — body shifts between states

The EATING sprite was drawn with the pet body shifted to the right.

```
     IDLE                EATING (body shifted — wrong)
. . . . . . . .      . . . . . . . .
. . . # # # . .      . . . . . # # #   <-- same pet, wrong canvas position
. . . # o # . .      . . . . . # - #
. . . # # # . .      . . . . . # # \
. . . . | . . .      . . . . . . | . .
. . . . . . . .      . . . . . . . . .
. . . . . . . .      . . . . . . . . .
```

On screen, the transition looks like this. The pet snaps sideways:

```
         IDLE        EATING

Screen:  [###]           [###]
         [#o#]   ->           [#-#]   <-- jumped right! the code is fine, the art is wrong.
         [###]           [##\]
          |                   |
```

#### Completely different orientations are fine — as long as they are centred

A sleeping pet lies on its side — a totally different orientation. That is fine.
The body still fills roughly the same central region of the canvas.

```
     IDLE                SLEEPING (rotated, but centred)
. . . . . . . .      . . . . . . . .
. . . # # # . .      . . . . . . . .
. . . # o # . .      . . # # # # # .   <-- rotated body, same central region
. . . # # # . .      . . # - . . # .
. . . . | . . .      . . # # # # # .
. . . . . . . .      . . . z . . . .
. . . . . . . .      . . . . . . . .
```

The rule in plain terms: **you can draw anything you want, as long as you do not slide
the whole body across the canvas between states**. Moving limbs, opening mouths, rotating,
adding accessories — all fine. Accidentally placing the pet at a different part of the
canvas in one state — that is what causes the jump.

---

### Transparency — what the background will look like

Empty (unpainted) pixels in Piskel export as fully transparent. The converter replaces
them with a magenta colour key (`0xF81F`). When the rendering code draws the sprite,
it skips any pixel that matches the key — letting the screen background show through.

**The screens in this project use a black background.**

This matters when you design your sprite:

```
What you draw in Piskel:      What appears on the black screen:

. . . . . . . .               # # # # # # # #   <-- black (transparent = background)
. . # # # # . .               # # G G G G # #   <-- green body
. . # . . # . .               # # G # # G # #   <-- black hole (transparent inside)
. . # . . # . .               # # G # # G # #   <-- black hole (transparent inside)
. . # # # # . .               # # G G G G # #   <-- green body
. . . . . . . .               # # # # # # # #   <-- black (transparent = background)

  . = transparent               # = black (background showing through)
  # = painted green             G = green pixel
```

Transparent pixels *inside* the pet body will appear as black holes. Decide deliberately
whether each area should be painted or left transparent. If you want the inside of an eye
to be white, paint it white — do not leave it empty.

---

### Choosing your colours

- **Keep the pet body colour the same across all states.** The player should recognise
  the pet as the same creature whether it is eating, sleeping, or sick.
- **3–4 colours per sprite is usually enough.** At 32×32, subtle colour differences are
  invisible. Bold, distinct colours read far more clearly than many similar shades.
- **Use your darkest colour for outlines.** A clear outline helps the pet stand out
  against the black background.

---

### Which states need a sprite

You need one sprite file for each value in the `PetState` enum in `lib/Pet/pet.h`.

| State constant | When the pet enters this state | Suggested look |
|---|---|---|
| `STATE_IDLE` | Default — pet is waiting | Facing forward, neutral or happy expression |
| `STATE_EATING` | After Feed action is confirmed | Mouth open, food nearby |
| `STATE_PLAYING` | While the device is being shaken | Arms raised, energetic pose |
| `STATE_SLEEPING` | After Sleep action is confirmed | Eyes closed, lying down, Zzz |
| `STATE_BATHING` | After Bathe action is confirmed | Bubbles or water drops around the pet |
| `STATE_HEALING` | After Heal action is confirmed | Small cross or sparkles |
| `STATE_SICK` | When sickness stat is high | Drooping posture, unwell expression |

**Start with `STATE_IDLE`.** Confirm the converter produces a valid `.h` file, then draw
the remaining states one at a time.

---

## Part 2 — Exporting from Piskel

When your sprite is ready:

1. Click the **Export** icon in the left-hand panel (arrow pointing right, out of a box).
2. Select the **C file** tab.
3. Leave the settings at their defaults — **4 bytes per pixel**. This is the ARGB8888
   format the converter expects. Do not change it.
4. Click **Download**.
5. Rename the downloaded file to match the state it represents. Use lowercase with underscores:

| State | Rename the file to |
|---|---|
| `STATE_IDLE` | `pet_idle.c` |
| `STATE_EATING` | `pet_eating.c` |
| `STATE_PLAYING` | `pet_playing.c` |
| `STATE_SLEEPING` | `pet_sleeping.c` |
| `STATE_BATHING` | `pet_bathing.c` |
| `STATE_HEALING` | `pet_healing.c` |
| `STATE_SICK` | `pet_sick.c` |

6. Move the renamed file into `assets/sprites/raw/`.

---

## Part 3 — Running the Converter

The converter is a small C++ program that runs on your Mac — not on the ESP32.
Full instructions are in `tools/piskel_converter/README.md`.

**Compile the tool once** (only needed the first time, or if `main.cpp` changes):

```bash
g++ -std=c++17 -o tools/piskel_converter/piskel_converter tools/piskel_converter/main.cpp
```

**Convert one sprite** (run this from the project root):

```bash
tools/piskel_converter/piskel_converter assets/sprites/raw/pet_idle.c lib/Display/sprites/pet_idle.h
```

When it works, you will see:

```
Converted 1 frame(s), 32x32 pixels each.
Output: lib/Display/sprites/pet_idle.h
```

**Convert all sprites at once** using a shell loop:

```bash
for file in assets/sprites/raw/*.c; do
    name=$(basename "$file" .c)
    tools/piskel_converter/piskel_converter "$file" "lib/Display/sprites/${name}.h"
done
```

---

## Part 4 — Rules That Affect Rendering and Animation Later

These rules apply now, at sprite creation time. Breaking them causes visual bugs that
are difficult to diagnose once the rendering code is in place.

### Rule 1 — All sprites must be the same size

Every state sprite must be **32×32 pixels**. If IDLE is 32×32 and EATING is 48×48,
the rendering code needs a special case for every mismatched state. Pick one size and
use it for every sprite in the project.

The converter handles any size — it reads the dimensions from the Piskel file. But the
rendering code written in Task 13 will expect one consistent size for all states.

### Rule 2 — Anchor point must be consistent (see Part 1)

Already covered above. The single most common sprite bug. Check every sprite before
exporting: does the pet body sit in the same region of the canvas as your IDLE sprite?

### Rule 3 — Sprite data is stored in flash, not RAM

The `.h` files the converter produces include the `PROGMEM` keyword. This keeps the
pixel arrays in the ESP32's flash memory (4 MB) rather than RAM (only ~200 KB available
after the display buffer). See `USEFUL_NOTES.md` — **"How Sprite Images Are Stored and
Drawn"** — for the full explanation of why this matters and how it works.

### Rule 4 — Flash memory budget

For Task 12, each state is one static frame — well within budget. When you add animation
in Task 13a, keep to a maximum of **8 frames per state**. Here is the maths:

```
7 states × 8 frames × 32 pixels wide × 32 pixels tall × 2 bytes per pixel
= 114,688 bytes ≈ 112 KB
```

The ESP32 has roughly 2.5 MB of flash available after the firmware. 112 KB is
comfortable. The budget grows quickly if you increase the frame count or sprite size,
so stay within the 8-frame limit for now.

### Rule 5 — One frame per state for Task 12

Do not draw animation sequences yet. One clear, well-centred drawing per state is the
deliverable for this task. Multi-frame animation is Task 13a. Getting the single frames
right first makes the animation work much smoother when you get there.

> **Picking up animation later?** Once you reach Task 13a, see [Part 6 — Animating Your
> Sprite](#part-6--animating-your-sprite-task-13a) for the multi-frame drawing rules and a
> full walkthrough of the frame-cycling code.

---

## Part 5 — How Your Sprite Appears on Each Screen

The pet appears on three different screens. Each screen reserves a different amount of
space for the pet face, and the rendering code (Task 13) will scale the sprite to fill
that space. Understanding the boundaries now helps you design a sprite that reads clearly
on every screen.

**Your sprite canvas is always 32x32 pixels.** The rendering code scales it up when it
draws to the screen — you never change the sprite size, only the draw call.

The scale factors shown below are the planned values for Task 13. They are based on the
face centre coordinates already defined in `lib/Display/display_manager.h`.

---

### Screen 1 — MAIN (the home screen)

The main screen shows a large version of the pet in the centre of the 135x240 display.
There are no stat bars on this screen — it is intentionally uncluttered.

```
|<--------- 135 px -------->|
+---------------------------+  y = 0
|                           |
|    Pet name (centred)     |  TITLE_ZONE: y = 5, height = 19
|                           |
+---------------------------+  y = 24
|                           |
|                           |
|       +-----------+       |
|       |           |       |
|       | 80 x 80px |       |  planned Task 13 sprite: 32x32 at 2.5x scale
|       |           |       |  face centre: y = 110  (MAIN_FACE_CENTER_Y)
|       |  2.5x     |       |  top edge: y = 70   (110 - 40)
|       |           |       |  bottom edge: y = 150  (110 + 40)
|       +-----------+       |
|                           |
+---------------------------+  y = 155
|   Mood text (centred)     |  MAIN_MOOD_Y = 155
+---------------------------+  y ~171
|                           |
|       (free space)        |
|                           |
+===========================+  y = 213
|   [Stats]  |  [Interact]  |  MAIN_NAV_ZONE: y = 213, height = 22
+===========================+  y = 235
|                           |  (5 px unused)
+---------------------------+  y = 240
```

**Key constraints on MAIN:**
- Pet name title occupies y = 5 to y = 24 — pet must not overlap it
- Mood text appears at y = 155 — the bottom edge of the sprite (y = 150) clears it by 5 px
- Nav bar starts at y = 213 — well below the pet

---

### Screen 2 — STATS (the detailed stats screen)

The stats screen fills the top half with five stat bars, then shows a small pet face
below them. The pet zone here is only 36 pixels tall — the tightest space of any screen.

```
|<--------- 135 px -------->|
+---------------------------+  y = 0
|                           |
|    Pet name (centred)     |  TITLE_ZONE: y = 5, height = 19
|                           |
+---------------------------+  y = 24
|  Happy: 80                |  label at y = 26   (HAPPY_BAR_ZONE.labelY)
|  [====================]   |  bar   at y = 36   (HAPPY_BAR_ZONE.barY), height = 10
|  Hunger: 60               |  label at y = 48   (HUNGER_BAR_ZONE.labelY)
|  [===============      ]  |  bar   at y = 58   (HUNGER_BAR_ZONE.barY), height = 10
|  Energy: 75               |  label at y = 70   (ENERGY_BAR_ZONE.labelY)
|  [==================   ]  |  bar   at y = 80   (ENERGY_BAR_ZONE.barY), height = 10
|  Clean: 90                |  label at y = 92   (CLEAN_BAR_ZONE.labelY)
|  [=====================]  |  bar   at y = 102  (CLEAN_BAR_ZONE.barY), height = 10
|  Sick: 20                 |  label at y = 114  (SICK_BAR_ZONE.labelY)
|  [====                 ]  |  bar   at y = 124  (SICK_BAR_ZONE.barY), height = 10
+===========================+  y = 134  (last bar ends here -- PET_FACE_ZONE begins)
|                           |  PET_FACE_ZONE: y = 134, height = 36, width = 135
|    face / sprite only     |  sprite drawn at 1x scale (no scaling)
|    (centred, 32 x 32)     |  face centre: y = 152  (134 + 36 / 2)
|                           |  top edge: y = 136, bottom edge: y = 168
+===========================+  y = 170  (134 + 36)
|                           |
|    Mood text (centred)    |  MOOD_ZONE: y = 180, height = 18
|                           |
+---------------------------+  y = 198
|                           |
|                           |
+===========================+  y = 220
|   [B/C: Back]             |  MENU_ZONE: y = 220, height = 20
+===========================+  y = 240
```

**Key constraints on STATS:**
- The pet zone (PET_FACE_ZONE) is only 36 px tall — a 32x32 sprite fits with 4 px spare
- Only the face / sprite is drawn in this zone — no name or state text
- Drawing at anything larger than 1x will overflow the zone boundary
- The last stat bar (Sick) ends at y = 134, exactly where the pet zone begins
- Mood text (y = 180) and back button (y = 220) sit below the pet zone

---

### Screen 3 — INTERACT (the action menu screen)

The interact screen shows a medium-sized pet at the top, with a contextual stat bar and
the action menu indicator below it.

```
|<--------- 135 px -------->|
+---------------------------+  y = 0
|                           |
|    Pet name (centred)     |  TITLE_ZONE: y = 5, height = 19
|                           |
+---------------------------+  y = 24
|                           |
|       +-----------+       |
|       |           |       |
|       | 64 x 64px |       |  planned Task 13 sprite: 32x32 at 2x scale
|       |           |       |  face centre: y = 90  (INTERACT_FACE_CENTER_Y)
|       |  2x       |       |  top edge: y = 58   (90 - 32)
|       |           |       |  bottom edge: y = 122  (90 + 32)
|       +-----------+       |
|                           |
+---------------------------+  y = 128
|   Mood text (centred)     |  INTERACT_MOOD_Y = 128
+---------------------------+  y ~144
|                           |
+===========================+  y = 153
|  Hungry: 72               |  INTERACT_STAT_ZONE: x = 5, y = 153, width = 125, height = 28
|  [================    ]   |  label at y = 156, bar at y = 168
+===========================+  y = 181  (153 + 28)
|                           |
|                           |
|                           |
+===========================+  y = 220
|  Action: [Feed]           |  MENU_ZONE: y = 220, height = 20
+===========================+  y = 240
```

**Key constraints on INTERACT:**
- Mood text is at y = 128 — the bottom edge of the sprite (y = 122) clears it by only 6 px
- The contextual stat bar (y = 153) shows the stat affected by the selected action
- The action menu indicator (y = 220) is at the very bottom — separate from the stat bar

---

### Summary table

All values are taken directly from the constants in `lib/Display/display_manager.h`.
The scale factor column is the planned value for Task 13 — it is not yet in the code.

| Screen   | Pet size on screen | Scale | Face centre (y) | Zone boundaries          |
|----------|--------------------|-------|-----------------|--------------------------|
| MAIN     | 80 x 80 px         | 2.5x  | y = 110         | title above y=24, mood at y=155 |
| STATS    | 32 x 32 px         | 1x    | y = 152         | zone y=134 to y=170 (36 px tall) |
| INTERACT | 64 x 64 px         | 2x    | y = 90          | title above y=24, mood at y=128 |

**The key takeaway:** your sprite canvas is always 32x32. The same drawing appears at
three different sizes depending on which screen the player is on. Bold shapes and outlines
that read clearly at 32x32 will look good at every scale. Fine detail drawn at 80x80 may
disappear entirely when the same sprite is shown at 32x32 on the Stats screen.

---

### Sprite size and flash memory cost

Every pixel in a sprite is stored as a 16-bit (2-byte) RGB565 value in flash memory.
The total cost of a complete spritesheet is:

```
total bytes = width × height × frames_per_state × number_of_states × 2
```

The table below shows flash cost at each base canvas size from 32×32 up to 135×135
(the maximum square that fits the 135 px screen width). The columns show how cost
scales as you add more animation frames and pet states.

| Base size | Per frame | 7 states × 1 frame | 7 states × 4 frames | 7 states × 8 frames | Budget |
|-----------|-----------|--------------------|---------------------|---------------------|--------|
| 32 × 32   |    2 KB   |    14 KB           |    56 KB            |   112 KB            | ✓      |
| 48 × 48   |    4.5 KB |    31 KB           |   126 KB            |   252 KB            | ✓      |
| 64 × 64   |    8 KB   |    56 KB           |   224 KB            |   448 KB            | ✓      |
| 80 × 80   |   12.5 KB |    88 KB           |   350 KB            |   700 KB            | ⚠      |
| 96 × 96   |   18 KB   |   126 KB           |   504 KB            | 1,008 KB            | ✗      |
| 112 × 112 |   24.5 KB |   172 KB           |   686 KB            | 1,372 KB            | ✗      |
| 128 × 128 |   32 KB   |   224 KB           |   896 KB            | 1,792 KB            | ✗      |
| 135 × 135 |   35.6 KB |   249 KB           |   997 KB            | 1,993 KB            | ✗      |

**Budget key:**
- ✓ safe — 7 states × 8 frames fits well within the ~812 KB of free flash
- ⚠ tight — 7 states × 8 frames uses ~700 KB; reduce frame count or state count to stay safe
- ✗ over budget for 8 frames — keep to 4 frames per state max (96×96 at 4 frames = 504 KB ✓)

**Flash budget explained:** The firmware partition on the M5StickC Plus 2 is ~1,260 KB.
The compiled firmware binary currently uses ~448 KB, leaving approximately 812 KB for sprite data.
These numbers will shift slightly as more code is added, so treat 700 KB of sprite data as
the practical ceiling.

**Formula reminder:** pixel count grows with the square of the side length — doubling the
sprite from 32×32 to 64×64 quadruples the pixel count (1,024 → 4,096) and quadruples
the flash cost. A jump from 64×64 to 128×128 costs 16× as much as the original 32×32.

---

### How sprite size interacts with each screen zone

Not every size can be displayed at 1:1 (one pixel in the sprite = one pixel on screen) on
every screen. The zones in `display_manager.h` set a hard physical limit per screen.

| Base size | STATS (36 px zone) | INTERACT (76 px max) | MAIN (90 px max) |
|-----------|--------------------|----------------------|------------------|
| 32 × 32   | fits with 4 px spare | fits with 44 px spare | fits with 58 px spare |
| 48 × 48   | clips (12 px over)   | fits with 28 px spare | fits with 42 px spare |
| 64 × 64   | clips (28 px over)   | fits with 12 px spare | fits with 26 px spare |
| 80 × 80   | clips (44 px over)   | clips (4 px over)    | fits with 10 px spare |
| 96 × 96   | clips (60 px over)   | clips (20 px over)   | clips (6 px over)    |
| 112 × 112 | clips (76 px over)   | clips (36 px over)   | clips (22 px over)   |
| 128 × 128 | clips (92 px over)   | clips (52 px over)   | clips (38 px over)   |
| 135 × 135 | clips (99 px over)   | clips (59 px over)   | clips (45 px over)   |

"Clips" means the sprite is taller or wider than the available zone space. The pixels outside
the zone boundary will either be drawn on top of adjacent UI elements, or the rendering code
must intentionally restrict the draw area. Neither is ideal — oversized sprites need either
a scaling step at render time (see `pushImageAffine()` in the M5GFX library) or a separate
cropped version for that screen.

**The STATS screen is the binding constraint.** Its pet zone is only 36 px tall regardless of
how large the sprite is on other screens. Any base canvas larger than 36×36 will overflow
the STATS zone at 1:1. The practical options are:
1. Keep the base canvas at 32×32 (fits everywhere at 1:1, but is small on MAIN)
2. Use a larger canvas and accept the clip on STATS — the centre of the sprite (the face) will
   still be visible; only the edges are cut off
3. Scale the sprite down at render time for the STATS screen — more complex code but the
   full image is visible at reduced size

---

## Part 6 — Animating Your Sprite (Task 13a)

Everything above produces a **still** picture: one frame per state, drawn once and held.
Task 13a brings the pet to life by showing several frames in turn — frame 0, then frame 1,
then back again — so it bounces, blinks, or breathes. This part covers both halves of that
work: **drawing** a multi-frame sprite, and the small piece of **code** that decides which
frame to show at each moment.

### 6.1 — Drawing a multi-frame sprite

A multi-frame sprite is just a normal Piskel drawing with **more than one frame** on the
timeline (use the **Frames** dropdown at the bottom of the Piskel canvas to add frames).
The converter already understands this — it reads however many frames you export and emits a
`sprite_name[FRAME_COUNT][W*H]` array. Nothing about the export or converter step changes.

Three art rules make an animation read well on a small 135×240 screen:

**Rule A — Anchor the silhouette across every frame.**
This is the [anchor point rule](#the-anchor-point-rule) from Part 1, and it matters even more
for animation. If the pet's body sits in a different part of the canvas from one frame to the
next, it will appear to *teleport* instead of move. Keep the body in the same region; move only
the part that should animate (eyes blinking, a small bob, ears twitching).

**Rule B — Move at least two pixels.**
At our starting speed of 5 frames per second, a one-pixel change is almost invisible. If you
want motion to read, move the animated part by **two pixels or more** between frames. Our
placeholder bounce, for example, shifts the whole body down by two pixels on the second frame.

**Rule C — Loop cleanly.**
The animation plays forever: after the last frame it jumps straight back to frame 0. Design the
frames so that jump looks smooth. Two reliable patterns:
- **Simple loop:** frame 0 → 1 → 2 → 0 → 1 → 2 … Best when the last frame flows naturally back
  into the first (e.g. a breathing cycle).
- **Ping-pong:** draw frames that go out and back — 0 → 1 → 2 → 1 → 0 — by exporting the
  middle frames twice. Best for a back-and-forth motion like a bounce or a wag, because there is
  no sudden jump at the loop point.

**Frame budget.** Keep to the **8-frames-per-state** ceiling from [Rule 4](#rule-4--flash-memory-budget).
More frames means smoother motion but more flash used; the maths in Rule 4 shows how quickly it
adds up.

### 6.2 — How frames cycle at runtime: the `AnimationManager`

Drawing the frames is only half the job. At runtime, *something* has to decide **which frame is
on screen right now** and change it at a steady speed. That is the entire job of
`lib/Display/animation_manager.{h,cpp}`. It does **not** draw anything — `DisplayManager` asks it
"which frame?" and draws that one. Splitting it this way keeps each class doing exactly one job
(the most important design rule in this codebase).

#### The trap to avoid: `delay()`

The tempting beginner approach is:

```cpp
drawFrame(0);
delay(200);
drawFrame(1);
delay(200);
```

**Never do this.** `delay(200)` freezes the *whole* program for 200 ms — buttons stop responding,
sound stops, the stat timers stop. On a microcontroller running one big `loop()`, `delay()` inside
the loop is almost always a bug (see Hardware Gotcha #2 in `CLAUDE.md`).

#### The pattern to use: "check the clock"

Instead of *waiting*, we *check the clock* every time around the loop and ask: **"has enough time
passed since I last changed the frame?"** `millis()` is the clock — it returns the number of
milliseconds since the device powered on, counting up forever. This is the exact same
non-blocking pattern the students already met in `TimerManager` (hunger rising over time); here it
is reused to advance an animation frame.

#### What the class remembers (its member variables)

```cpp
int frameCount;                     // how many frames the sprite has (e.g. 2)
unsigned long frameDurationMs;      // how long to show each frame (200 ms = 5 fps)
int currentFrame;                   // which frame is showing now (0 .. frameCount-1)
unsigned long lastFrameAdvanceTime; // the millis() value when we last switched frame
```

- `unsigned long` is used for every time value because `millis()` returns a large, always-positive
  number — the same type `TimerManager` uses.
- `currentFrame` is the one piece of state the rest of the program reads; everything else exists
  to keep it correct.

#### The constructor — setting up

```cpp
AnimationManager::AnimationManager(int frameCount, unsigned long frameDurationMs)
    : frameCount(frameCount),
      frameDurationMs(frameDurationMs),
      currentFrame(0),
      lastFrameAdvanceTime(0) {
}
```

It is told **how many frames** and **how fast** (with `FRAME_DURATION_MS = 200` as the default, so
`AnimationManager petAnimation(2);` gives 5 fps automatically). We start on frame 0, and start
`lastFrameAdvanceTime` at **0** on purpose: on the very first loop `millis()` is already larger
than 0, so "has 200 ms passed since time 0?" is true immediately and the animation starts right
away instead of pausing first. `TimerManager` uses the identical trick.

> The `: frameCount(frameCount), …` syntax is a C++ **member initializer list** — the standard way
> to set member variables when an object is created. The member and the parameter share a name, but
> the part in parentheses is always the parameter, so there is no ambiguity.

#### `update()` — the heart of it

Called **once every loop**:

```cpp
void AnimationManager::update() {
    if (frameCount <= 1) {
        return;                                    // (A)
    }

    unsigned long currentTime = millis();          // (B)

    if (currentTime - lastFrameAdvanceTime >= frameDurationMs) {   // (C)
        currentFrame = (currentFrame + 1) % frameCount;            // (D)
        lastFrameAdvanceTime = currentTime;                        // (E)
    }
}
```

- **(A) The guard.** A single-frame sprite (a still image) has nothing to cycle through, so we
  leave immediately. This also prevents a crash on line (D): you can never do `% 0`.
- **(B) Read the clock once** into a clearly named variable.
- **(C) The question.** "Current time − the time I last switched ≥ how long a frame should last?"
  If not, the `if` is false, the body is skipped, and `update()` returns instantly — no waiting.
- **(D) Advance the frame, with wrap-around.** This is the line to slow down on:

  ```cpp
  currentFrame = (currentFrame + 1) % frameCount;
  ```

  `currentFrame + 1` moves to the next frame; `% frameCount` (**modulo** — the remainder after
  division) wraps it back to 0 after the last frame. With 2 frames: `(0+1)%2 = 1`, then
  `(1+1)%2 = 0`. Modulo is the clean, idiomatic way to make a counter loop. A good 2-minute
  exercise: "what does `% 3` give for 0,1,2,3,4,5?" → 0,1,2,0,1,2.
- **(E) Remember when** we switched, so the next interval is measured from now.

#### The two helpers

```cpp
int AnimationManager::getCurrentFrame() const {   // which frame should I draw?
    return currentFrame;
}

void AnimationManager::reset() {                   // start the animation over
    currentFrame = 0;
    lastFrameAdvanceTime = millis();
}
```

`getCurrentFrame()` is what `DisplayManager` calls to ask which frame to draw. The `const` promises
it only *reads* the object, never changes it — a good habit to teach for "getter" functions.
`reset()` jumps back to frame 0 and restarts the timer; we call it when switching screens so the
pet does not appear to resume mid-bounce.

#### A worked timeline (good for the whiteboard)

Frame duration = 200 ms, 2 frames. The loop runs hundreds of times per second:

| `millis()` | `update()` asks: passed ≥ 200? | `currentFrame` after |
|-----------:|-------------------------------|:--------------------:|
| 5          | 5 − 0 = 5 → no                | 0 |
| 150        | 150 − 0 = 150 → no            | 0 |
| 205        | 205 − 0 = 205 → **yes**       | 1  (last = 205) |
| 300        | 300 − 205 = 95 → no           | 1 |
| 410        | 410 − 205 = 205 → **yes**     | 0  (last = 410) |

The frame only changes on the rare loops where enough time has passed; every other loop `update()`
does almost nothing and returns instantly. That is *why* buttons and sound stay responsive while
the pet animates.

#### The architectural lesson worth emphasising

`AnimationManager` knows about **frame numbers and time** — nothing else. It never touches the
screen, M5, or pixels; drawing stays in `DisplayManager`. That clean split — "each module has
exactly one job" — is the single most important design idea in this codebase, and this small class
is a tidy example to point students to.

---

## Part 7 — Sliding Your Sprite With Tilt (optional demo)

Part 6 made the pet change **which frame** is on screen over time (it bounces). This optional extra
makes it change **where on the screen** it is drawn, based on how you physically **tilt the
device** — the pet slides toward the low side, like a marble on a tray. It is a self-contained
demo you can switch on to play with the accelerometer; turning it off returns the pet to dead
centre exactly as before.

The key idea to take away: **animation (which frame) and motion (where on screen) are two separate
jobs.** `AnimationManager` owns the frame timing; a second small helper, `TiltMotion`, owns the
position. They compose without knowing about each other — the pet bounces *and* glides at once.

### 7.1 — Where the tilt signal comes from

The M5StickC has an accelerometer (the MPU6886) that already reports gravity as three numbers —
`accelX`, `accelY`, `accelZ` — in units of **g** (1 g = the pull of gravity). Held flat, gravity
points straight down through the Z axis, so X and Y read ≈ 0. **Tilt** the board and some of that
gravity "leans" into the X and Y axes. That lean is exactly the signal we want: tilt right and
`accelX` grows; tilt away from you and `accelY` grows. `ImuManager` already reads these every loop
for shake detection, and exposes `imu.getAccelX()` / `getAccelY()` — until now those getters were
unused.

### 7.2 — Two problems with raw tilt, and the two fixes

If we drew the pet straight at the raw reading, two things would go wrong. Each fix is one idea in
`lib/Display/tilt_motion.{h,cpp}`:

1. **Raw data is noisy and jumpy** → the pet would jitter. Fix: a **low-pass filter** (smoothing).
   Instead of snapping to each new reading, ease toward it a little every frame.
2. **A hard tilt would push the pet off the screen** → Fix: a **clamp**, capping the offset to a
   maximum number of pixels.

So `TiltMotion::update()` runs a four-step pipeline every loop: **scale → clamp → smooth → round.**

```cpp
void TiltMotion::update(float accelX, float accelY) {
    // 1 & 2 — scale tilt (g) to pixels, then clamp so the pet stays on screen.
    float targetX = constrain(accelX * TILT_SCALE, -MAX_OFFSET_X, MAX_OFFSET_X);
    float targetY = constrain(accelY * TILT_SCALE, -MAX_OFFSET_Y, MAX_OFFSET_Y);

    // 3 — ease the stored offset a fraction of the way toward the target.
    smoothedX = smoothedX + (targetX - smoothedX) * SMOOTHING_FACTOR;
    smoothedY = smoothedY + (targetY - smoothedY) * SMOOTHING_FACTOR;
}
```

- **`TILT_SCALE`** turns g into pixels — the main *sensitivity* knob (bigger = pet moves further).
- **`constrain(value, low, high)`** is an Arduino helper that returns `value`, or the nearest limit
  if it is out of range. We clamp the **target**, so even a violent shake can only ask for a slide
  of `MAX_OFFSET` pixels.
- **`MAX_OFFSET_X / MAX_OFFSET_Y`** are the limits. The sprite is 80 px wide on a 135 px screen,
  leaving `(135 − 80) / 2 = 27` px of slack each side, so 25 keeps it just inside the edge.

### 7.3 — The smoothing line, slowly

```cpp
smoothedX = smoothedX + (targetX - smoothedX) * SMOOTHING_FACTOR;
```

Read it as: *"move from where I am a fraction of the way toward where I want to be."*
`(targetX − smoothedX)` is the gap left to close; `* SMOOTHING_FACTOR` (0.2) closes 20% of it this
frame, then 20% of the new smaller gap next frame, and so on — fast at first, gently slowing as it
arrives. That is a **low-pass filter**: sudden jitter in the raw reading is averaged away, so the
pet glides instead of twitching.

It is the *same* "a little closer each tick" idea as the `millis()` timers in Part 6 and in
`TimerManager` — there it was applied to **time**, here to **position**. `SMOOTHING_FACTOR` is the
feel knob: 0 would freeze the pet, 1 would snap instantly (all jitter, no smoothing), 0.2 is calm.

#### What "low-pass filter" means

A **low-pass filter** lets *slow* changes through but blocks *fast* ones. A real, sustained tilt
lasts many frames, so it eases all the way through; a one-frame noise spike is gone before the pet
can move 20% toward it, so it gets smoothed away. That is exactly why the pet follows your tilt but
ignores the accelerometer's constant tiny jitter.

#### A worked example

Start the pet at `0` and tilt so `targetX = 100`, with `SMOOTHING_FACTOR = 0.2`. Each frame it
closes 20% of the gap that is *left*:

| Frame | Gap (`target − smoothed`) | Step (gap × 0.2) | New `smoothedX` |
|------:|--------------------------:|-----------------:|----------------:|
| 1 | 100  | 20   | 20   |
| 2 | 80   | 16   | 36   |
| 3 | 64   | 12.8 | 48.8 |
| 4 | 51.2 | 10.2 | 59.0 |

The steps shrink every frame, so the pet *glides in* — fast at first, easing as it arrives — and
never snaps. (It approaches 100 without quite landing on it exactly, which is why we keep
`smoothedX` as a `float`; see 7.4.)

> **Why it recenters by itself.** Hold the device flat and tilt ≈ 0, so `targetX` ≈ 0. The line
> above then keeps easing `smoothedX` toward 0 — the pet drifts back to centre on its own. There is
> no special "go home" code; it falls straight out of the maths.

### 7.4 — Handing out whole pixels

The offsets are kept as `float`s so the easing keeps its precision frame to frame. The screen only
deals in whole pixels, so we round **only when handing the value out**:

```cpp
int TiltMotion::getOffsetX() const { return (int) round(smoothedX); }
int TiltMotion::getOffsetY() const { return (int) round(smoothedY); }
```

### 7.5 — How the offset reaches the screen

`TiltMotion` does pure maths and touches no pixels — just like `AnimationManager`. The offset
travels from `main.cpp` (which owns the IMU) down to the one place that positions the sprite:

```
main.cpp  →  renderDisplay()  →  renderMainScreen()/renderInteractScreen()  →  drawPetSprite()
```

`drawPetSprite()` simply adds the offset to the centre it already computes:

```cpp
int spriteX = (SCREEN_WIDTH - spriteWidth) / 2 + spriteOffsetX;
int spriteY = faceCenterY - (spriteHeight / 2) + spriteOffsetY;
```

When the offset is `0, 0`, this *is* the old centred position — so every screen looks unchanged.
(The Stats screen draws no pet sprite, so it is left out of this entirely.)

### 7.6 — The on/off switch

One flag near the top of `main.cpp` controls the whole effect:

```cpp
static const bool TILT_MOVEMENT_ENABLED = true;
```

When `true`, `main.cpp` feeds the helper live tilt each loop and passes its offset on; when
`false`, it passes `0, 0` and the pet is drawn dead centre exactly as before. Flip it and rebuild —
a safe, self-contained thing for students to experiment with.

### 7.7 — Calibrating on the device (a good exercise)

Which way the pet moves depends on how the accelerometer is physically mounted in *this* board, so
the first build is an educated guess you confirm by looking. All the knobs are named constants in
`tilt_motion.{h,cpp}`, so tuning is a one-file edit:

- **Moves on the wrong axis** (you tilt left/right but it goes up/down): swap which reading feeds
  which target in `update()`.
- **Moves the opposite way** (tilt right, pet goes left): negate that axis's reading —
  `accelX` → `-accelX` (negate the *reading*, not `TILT_SCALE`, which is shared by both axes and
  would flip both).
- **Too sluggish or too twitchy**: raise/lower `TILT_SCALE` (distance) or `SMOOTHING_FACTOR`
  (responsiveness).

### The architectural lesson (again)

`TiltMotion` knows about **tilt and pixels** — nothing else. It never reads the IMU directly (it is
*handed* the numbers) and never draws (it *hands back* an offset). Pair it with `AnimationManager`
from Part 6 and you have two tiny single-job classes — one for *when*, one for *where* — composing
into richer behaviour than either could alone. That is the whole game: small pieces, each with one
job, combined.
