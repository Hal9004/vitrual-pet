#include "action_menu.h"

ActionMenu::ActionMenu() : currentActionIndex(0) {
    // Each entry carries the action type, display name, description, and
    // which pet stat it affects. The relevantStat field lets the Interact
    // screen highlight the correct bar without needing a separate lookup.
    actions[0] = {ACTION_FEED,  "Feed",  "Give food to pet",    STAT_HUNGER};
    actions[1] = {ACTION_PLAY,  "Play",  "Play with pet",        STAT_HAPPINESS};
    actions[2] = {ACTION_SLEEP, "Sleep", "Let pet rest",         STAT_ENERGY};
    actions[3] = {ACTION_BATHE, "Bathe", "Clean the pet",        STAT_CLEANLINESS};
    actions[4] = {ACTION_HEAL,  "Heal",  "Treat pet illness",    STAT_SICKNESS};
    actions[5] = {ACTION_SAVE,  "Save",  "Save pet progress",    STAT_NONE};
    actions[6] = {ACTION_BACK,  "Back",  "Return to main screen",STAT_NONE};
}

void ActionMenu::update(const ButtonHandler& buttons) {
    // Button C: Previous action
    if (buttons.wasButtonCPressed()) {
        currentActionIndex--;
        if (currentActionIndex < 0) {
            currentActionIndex = NUM_ACTIONS - 1;  // Wrap around to last
        }
    }

    // Button B: Next action
    if (buttons.wasButtonBPressed()) {
        currentActionIndex++;
        if (currentActionIndex >= NUM_ACTIONS) {
            currentActionIndex = 0;  // Wrap around to first
        }
    }
}

Action ActionMenu::getSelectedAction() const {
    return actions[currentActionIndex];
}

int ActionMenu::getCurrentActionIndex() const {
    return currentActionIndex;
}

bool ActionMenu::isBackSelected() const {
    // Returns true when the user has scrolled to the Back entry.
    // main.cpp checks this before calling confirmAction() so the navigation
    // manager can switch screens without executing a pet action.
    return actions[currentActionIndex].type == ACTION_BACK;
}

RelevantStat ActionMenu::getRelevantStat() const {
    // Returns which stat the currently selected action affects.
    // DisplayManager calls this to know which bar to highlight on the Interact screen.
    return actions[currentActionIndex].relevantStat;
}

void ActionMenu::confirmAction(Pet& pet, DisplayManager& display, SpeakerManager& speaker, StorageManager& storage) {
    Action selectedAction = getSelectedAction();

    // Each case calls the matching Pet method directly and plays its sound.
    // Save and Back are special cases: Save writes to storage instead of changing
    // a pet stat, and Back is normally handled by NavigationManager before we get here.
    switch (selectedAction.type) {
        case ACTION_FEED:
            pet.feed();
            speaker.playFeedSound();
            break;
        case ACTION_PLAY:
            pet.play();
            speaker.playPlaySound();
            break;
        case ACTION_SLEEP:
            pet.sleep();
            speaker.playSleepSound();
            break;
        case ACTION_BATHE:
            pet.bathe();
            speaker.playBatheSound();
            break;
        case ACTION_HEAL:
            pet.heal();
            speaker.playHealSound();
            break;
        case ACTION_SAVE:
            storage.save(pet);
            speaker.playSaveSound();
            break;
        case ACTION_BACK:
            // Back is handled by the NavigationManager before confirmAction() is called.
            // If we somehow reach here, do nothing — there is no pet action to run.
            return;
    }
}
