#include "action_menu.h"

ActionMenu::ActionMenu() : currentActionIndex(0) {
    // Each entry carries the action type, display name, description, and
    // which pet stat it affects. The relevantStat field lets the Interact
    // screen highlight the correct bar without needing a separate lookup.
    actions[0] = {ActionType::FEED,  "Feed",  "Give food to pet",    STAT_HUNGER};
    actions[1] = {ActionType::PLAY,  "Play",  "Play with pet",        STAT_HAPPINESS};
    actions[2] = {ActionType::SLEEP, "Sleep", "Let pet rest",         STAT_ENERGY};
    actions[3] = {ActionType::BATHE, "Bathe", "Clean the pet",        STAT_CLEANLINESS};
    actions[4] = {ActionType::HEAL,  "Heal",  "Treat pet illness",    STAT_SICKNESS};
    actions[5] = {ActionType::SAVE,  "Save",  "Save pet progress",    STAT_NONE};
    actions[6] = {ActionType::BACK,  "Back",  "Return to main screen",STAT_NONE};
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
    // Returns true when the player has scrolled to the Back entry.
    // main.cpp checks this before calling confirmAction() so the navigation
    // manager can switch screens without executing a pet action.
    return actions[currentActionIndex].type == ActionType::BACK;
}

RelevantStat ActionMenu::getRelevantStat() const {
    // Returns which stat the currently selected action affects.
    // DisplayManager calls this to know which bar to highlight on the Interact screen.
    return actions[currentActionIndex].relevantStat;
}

void ActionMenu::executePetAction(Pet& pet, ActionType actionType) {
    // Each case calls the matching method on the Pet object.
    // SAVE and BACK do not appear here because they are handled separately
    // in confirmAction() — Save writes to storage, Back switches screens.
    switch (actionType) {
        case ActionType::FEED:
            pet.feed();
            break;
        case ActionType::PLAY:
            pet.play();
            break;
        case ActionType::SLEEP:
            pet.sleep();
            break;
        case ActionType::BATHE:
            pet.bathe();
            break;
        case ActionType::HEAL:
            pet.heal();
            break;
        default:
            break;
    }
}

void ActionMenu::confirmAction(Pet& pet, DisplayManager& display, SpeakerManager& speaker, StorageManager& storage) {
    Action selectedAction = getSelectedAction();

    // Each case executes the action and plays its matching sound.
    // Save is included here alongside the care actions so all action handling
    // lives in one place and every case follows the same visible pattern.
    switch (selectedAction.type) {
        case ActionType::FEED:
            executePetAction(pet, selectedAction.type);
            speaker.playFeedSound();
            break;
        case ActionType::PLAY:
            executePetAction(pet, selectedAction.type);
            speaker.playPlaySound();
            break;
        case ActionType::SLEEP:
            executePetAction(pet, selectedAction.type);
            speaker.playSleepSound();
            break;
        case ActionType::BATHE:
            executePetAction(pet, selectedAction.type);
            speaker.playBatheSound();
            break;
        case ActionType::HEAL:
            executePetAction(pet, selectedAction.type);
            speaker.playHealSound();
            break;
        case ActionType::SAVE:
            storage.save(pet);
            speaker.playSaveSound();
            break;
        case ActionType::BACK:
            // Back is handled by the NavigationManager before confirmAction() is called.
            // If we somehow reach here, do nothing — there is no pet action to run.
            return;
    }

    // Show the action name on screen so the player can see what just happened.
    display.showActionFeedback(selectedAction.name);

    // Hold the feedback message long enough for the player to read it.
    delay(1000);
}
