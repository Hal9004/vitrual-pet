#include "action_menu.h"

ActionMenu::ActionMenu() : currentActionIndex(0) {
    // Initialize action array with all available pet actions
    actions[0] = {ActionType::FEED, "Feed", "Give food to pet"};
    actions[1] = {ActionType::PLAY, "Play", "Play with pet"};
    actions[2] = {ActionType::SLEEP, "Sleep", "Let pet rest"};
    actions[3] = {ActionType::BATHE, "Bathe", "Clean the pet"};
    actions[4] = {ActionType::HEAL, "Heal", "Treat pet illness"};
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

void ActionMenu::executePetAction(Pet& pet, ActionType actionType) {
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
    }
}

void ActionMenu::confirmAction(Pet& pet, DisplayManager& display, SpeakerManager& speaker) {
    Action selectedAction = getSelectedAction();

    // Execute the pet action — updates the pet's stats and sets the new state.
    executePetAction(pet, selectedAction.type);

    // Play the sound that matches the action so the player gets audio confirmation.
    switch (selectedAction.type) {
        case ActionType::FEED:
            speaker.playFeedSound();
            break;
        case ActionType::PLAY:
            speaker.playPlaySound();
            break;
        case ActionType::SLEEP:
            speaker.playSleepSound();
            break;
        case ActionType::BATHE:
            speaker.playBatheSound();
            break;
        case ActionType::HEAL:
            speaker.playHealSound();
            break;
    }

    // Show the action name on screen so the player can see what just happened.
    display.showActionFeedback(selectedAction.name);

    // Hold the feedback message long enough for the player to read it.
    delay(1000);
}

void ActionMenu::displayCurrentMenu(DisplayManager& display) const {
    Action selectedAction = getSelectedAction();
    
    // Display the selected action name
    // Format: "[ACTION_NAME]" centered on screen
    // NOTE: This method no longer clears the screen - screen should be cleared by caller if needed
    display.clearScreen();
    display.printCenteredText("Select Action:", 30);
    display.printCenteredText(selectedAction.name, 100, TFT_YELLOW, 3);
    display.printCenteredText(selectedAction.description, 180, TFT_WHITE, 1);
    
    // Display navigation hint
    display.printCenteredText("C: Previous  B: Next", 215, TFT_CYAN, 1);
}

// Draw a small menu indicator overlay on the status view
void ActionMenu::drawMenuIndicator(DisplayManager& display, int x, int y) const {
    Action selectedAction = getSelectedAction();
    
    // Clear only the indicator area before redrawing
    display.fillRect(x, y, 130, 20, TFT_BLACK);
    
    // Draw small box around the indication area
    M5.Lcd.drawRect(x, y, 130, 20, TFT_CYAN);
    
    // Show selected action name in small text
    display.printText("Action: ", x + 2, y + 4, TFT_CYAN, 1);
    display.printText(selectedAction.name, x + 50, y + 4, TFT_YELLOW, 1);
}
