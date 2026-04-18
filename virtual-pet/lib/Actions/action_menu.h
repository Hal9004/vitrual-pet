#ifndef ACTION_MENU_H
#define ACTION_MENU_H

#include "../Pet/pet.h"
#include "../Display/display_manager.h"
#include "../Button/button_handler.h"
#include "../Speaker/speaker_manager.h"

/**
 * ActionMenu
 * 
 * Manages pet interaction selection via buttons using a menu-driven interface.
 * Allows user to cycle through available actions and confirm selection.
 * 
 * Button Mapping:
 *   - Button A: Previous action
 *   - Button B: Next action
 *   - Button C: Confirm/Execute selected action
 * 
 * Available Actions: Feed, Play, Sleep, Bathe, Heal
 * 
 * Usage:
 *   ActionMenu menu;
 *   
 *   // In your loop:
 *   menu.update(buttons);  // Handle button input for cycling
 *   menu.displayCurrentMenu(display);  // Show selected action
 *   if (someConfirmationTrigger) {
 *       menu.confirmAction(myPet, display);  // Execute action
 *   }
 */

enum class ActionType {
    FEED,
    PLAY,
    SLEEP,
    BATHE,
    HEAL
};

struct Action {
    ActionType type;
    const char* name;
    const char* description;
};

class ActionMenu {
private:
    static const int NUM_ACTIONS = 5;
    Action actions[NUM_ACTIONS];
    int currentActionIndex;

    // Helper function to execute pet action based on type
    void executePetAction(Pet& pet, ActionType actionType);

public:
    // Constructor
    ActionMenu();

    /**
     * update(ButtonHandler&)
     * Call this once per loop iteration.
     * Detects Button A (previous) and Button B (next) presses to cycle through actions.
     */
    void update(const ButtonHandler& buttons);

    /**
     * getSelectedAction()
     * Returns the currently selected action struct.
     */
    Action getSelectedAction() const;

    /**
     * getCurrentActionIndex()
     * Returns the index of the currently selected action (0-4).
     */
    int getCurrentActionIndex() const;

    // confirmAction(Pet&, DisplayManager&, SpeakerManager&)
    // Executes the currently selected pet action, plays the matching sound,
    // and shows feedback on the display. Call this when the user presses Button A.
    void confirmAction(Pet& pet, DisplayManager& display, SpeakerManager& speaker);

    /**
     * displayCurrentMenu(DisplayManager&)
     * Displays the currently selected action to the screen.
     * Shows action name and description.
     */
    void displayCurrentMenu(DisplayManager& display) const;

    /**
     * drawMenuIndicator(DisplayManager&, int x, int y)
     * Draws a small menu indicator showing selected action without clearing screen.
     * Used as overlay on pet status display.
     */
    void drawMenuIndicator(DisplayManager& display, int x, int y) const;
};

#endif
