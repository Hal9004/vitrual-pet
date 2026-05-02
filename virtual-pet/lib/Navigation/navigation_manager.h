#ifndef NAVIGATION_MANAGER_H
#define NAVIGATION_MANAGER_H

#include "../Display/screen_layout.h"
#include "../Button/button_handler.h"
#include "../Actions/action_menu.h"

// NavigationManager owns which screen the player is currently viewing and
// handles all screen-switching input. By moving this logic into its own
// module, loop() stays short — it just delegates here instead of containing
// a growing switch statement that would need to change for every new screen.
class NavigationManager {
private:
    ScreenState currentScreen;
    bool confirmActionRequested;  // True for one loop iteration when A is pressed on Interact

    // One input handler per screen.
    // Each method only reads the buttons that are meaningful on that screen,
    // which makes it easy to reason about: open the handler, see exactly what
    // the buttons do on that screen, nothing more.
    void handleMainScreenInput(const ButtonHandler& buttons);
    void handleStatsScreenInput(const ButtonHandler& buttons);
    void handleInteractScreenInput(const ButtonHandler& buttons, const ActionMenu& menu);

public:
    NavigationManager();

    // update() — call once per loop. Resets the confirm flag, then routes
    // button input to the handler for whichever screen is currently active.
    void update(const ButtonHandler& buttons, const ActionMenu& menu);

    // getCurrentScreen() — returns the screen the player is currently viewing.
    ScreenState getCurrentScreen() const;

    // shouldConfirmAction() — returns true if the player pressed A on the Interact
    // screen and the selected action is NOT Back. loop() reads this once and calls
    // menu.confirmAction() when it is true.
    bool shouldConfirmAction() const;
};

#endif
