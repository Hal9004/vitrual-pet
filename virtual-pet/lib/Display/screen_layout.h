#ifndef SCREEN_LAYOUT_H
#define SCREEN_LAYOUT_H

// A struct is a plain bundle of related values with named fields.
// Unlike a class, it has no methods and no private data — it is just data.
// Using a struct here means we can pass a whole zone around as one value,
// rather than passing four separate integers every time.

// Describes the position and size of one rectangular region of the screen.
// x and y are the top-left corner; width and height are in pixels.
struct ScreenZone {
    int x;
    int y;
    int width;
    int height;
};

// Groups the two Y positions that every stat bar needs: where the text label
// sits and where the progress bar sits directly below it.
struct StatBarZone {
    int labelY;
    int barY;
};

#endif
