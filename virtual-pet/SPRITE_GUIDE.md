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
