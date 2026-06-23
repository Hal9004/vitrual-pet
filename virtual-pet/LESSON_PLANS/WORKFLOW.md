# Student Workflow — Google Drive + VS Code + PlatformIO

This is the routine students use **every session**. Sessions 1–10 reference it instead of
repeating the steps. There is **no Git** — students keep their work in their own Google
Drive folder and edit/build/upload with VS Code + PlatformIO.

---

## One-time setup (Session 1, with teacher help)
1. Install **VS Code** and the **PlatformIO** extension (or use the pre-set-up class machines).
2. Download the **starter project** from the class **Google Drive** folder and unzip it.
3. In VS Code: **File → Open Folder…** and choose the project folder. PlatformIO detects the
   `platformio.ini` and sets everything up automatically — no extra configuration.

---

## Start of every session — pick up where you left off
1. Open **your own** latest project folder from **your Google Drive** (Session 1: the starter;
   every session after: the version you saved last time).
2. **File → Open Folder…** in VS Code. Wait a few seconds for PlatformIO to finish loading.

## Build & upload — see your changes on the device
- **✓ Build** (PlatformIO toolbar, or the checkmark in the blue bar) — compiles the code.
- **→ Upload** (the right-arrow) — flashes it onto the M5StickC over USB.
- If Build shows red errors, read the first one — it usually names the file and line.

## End of every session — save your progress
- Save your **whole project folder back to your Google Drive** (drag it in, or re-upload the
  zipped folder). This is what lets you continue next session.
- **If you forget, you lose this session's work** — saving to Drive is the only copy.

---

## Sprites (Piskel) — create, save, reuse
1. Draw your sprite in **Piskel** (80×80).
2. **Export** it from Piskel and **save the file to your Google Drive** so your art is safe.
3. Run the **converter** to turn the image into a C++ array (see `SPRITE_GUIDE.md`).
4. Put the generated array file in `lib/Display/sprites/`, point the code at it, and **Upload**.
5. Keep the exported sprite in your Drive so you can reuse or tweak it in a later session.

---

## Quick reference
| You want to… | Do this |
|---|---|
| Get your project | Open your Google Drive folder → **File → Open Folder** in VS Code |
| Run your changes | PlatformIO **✓ Build** then **→ Upload** |
| Not lose your work | At session end, save the project folder **back to Google Drive** |
| Use your own sprite | Export from Piskel → save to Drive → convert → drop in `lib/Display/sprites/` |
