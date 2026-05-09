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
   pet state for now — animation frames come in Task 13.

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
in Task 13, keep to a maximum of **8 frames per state**. Here is the maths:

```
7 states × 8 frames × 32 pixels wide × 32 pixels tall × 2 bytes per pixel
= 114,688 bytes ≈ 112 KB
```

The ESP32 has roughly 2.5 MB of flash available after the firmware. 112 KB is
comfortable. The budget grows quickly if you increase the frame count or sprite size,
so stay within the 8-frame limit for now.

### Rule 5 — One frame per state for Task 12

Do not draw animation sequences yet. One clear, well-centred drawing per state is the
deliverable for this task. Multi-frame animation is Task 13. Getting the single frames
right first makes the animation work much smoother when you get there.

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
