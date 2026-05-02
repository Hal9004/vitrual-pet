#ifndef ACTION_MENU_H
#define ACTION_MENU_H

#include "../Pet/pet.h"
#include "../Display/display_manager.h"
#include "../Display/screen_layout.h"
#include "../Button/button_handler.h"
#include "../Speaker/speaker_manager.h"
#include "../Storage/storage_manager.h"

// ActionMenu manages the list of things the player can do with their pet.
// It tracks which action is highlighted and executes the action when confirmed.
//
// Button Mapping (when the Interact screen is active):
//   Button B: Next action
//   Button C: Previous action
//   Button A: Confirm selected action (handled in main.cpp)
//
// Available Actions: Feed, Play, Sleep, Bathe, Heal, Save, Back

// ActionType names every possible action the player can select.
// Using an enum class (scoped enum) means we must write ActionType::FEED
// rather than just FEED — this prevents name collisions with other enums
// or variables in the project.
enum class ActionType {
    FEED,
    PLAY,
    SLEEP,
    BATHE,
    HEAL,
    SAVE,
    BACK  // Returns the player to the Main screen without doing anything
};

// Action bundles everything DisplayManager needs to know about one menu entry:
// what to call it, what it does, and which stat it affects.
// Adding relevantStat here means the display layer never needs a separate
// switch statement to figure out which bar to highlight — it just reads the field.
struct Action {
    ActionType   type;
    const char*  name;
    const char*  description;
    RelevantStat relevantStat;  // Which pet stat this action changes (STAT_NONE if none)
};

class ActionMenu {
private:
    static const int NUM_ACTIONS = 7;  // Feed, Play, Sleep, Bathe, Heal, Save, Back
    Action actions[NUM_ACTIONS];
    int currentActionIndex;

    // Calls the correct Pet method for a given action type.
    // Centralising this avoids repeating the same switch in multiple places.
    void executePetAction(Pet& pet, ActionType actionType);

public:
    ActionMenu();

    // update() — call once per loop when the Interact screen is active.
    // Reads Button B (next) and Button C (previous) to cycle through actions.
    // Only call this when the Interact screen is visible — otherwise the
    // selection would change even while the player is on a different screen.
    void update(const ButtonHandler& buttons);

    // getSelectedAction() — returns the full Action struct for the highlighted entry.
    Action getSelectedAction() const;

    // getCurrentActionIndex() — returns the numeric position of the selected action (0–6).
    int getCurrentActionIndex() const;

    // isBackSelected() — returns true when the player has scrolled to "Back".
    // main.cpp uses this to switch screens instead of calling confirmAction().
    bool isBackSelected() const;

    // getRelevantStat() — returns which stat is affected by the selected action.
    // DisplayManager uses this to draw the correct contextual bar on the Interact screen.
    RelevantStat getRelevantStat() const;

    // confirmAction() — executes the selected action, plays the matching sound,
    // and shows a brief feedback message on screen.
    // Only call this when isBackSelected() is false — Back is handled separately.
    // StorageManager is needed because the Save action writes directly to NVS.
    void confirmAction(Pet& pet, DisplayManager& display, SpeakerManager& speaker, StorageManager& storage);

    // displayCurrentMenu() — full-screen action display (legacy, mostly unused now).
    void displayCurrentMenu(DisplayManager& display) const;

    // drawMenuIndicator() — draws the compact action name overlay at the bottom of the screen.
    // Used on the Interact screen where the pet face takes up the top portion.
    void drawMenuIndicator(DisplayManager& display, int x, int y) const;
};

#endif
