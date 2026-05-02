#include "navigation_manager.h"
#include <Arduino.h>

NavigationManager::NavigationManager()
    : currentScreen(SCREEN_MAIN),
      mainNavIndex(0),
      confirmActionRequested(false) {
    // Start on the Main screen with the Stats tab highlighted by default.
}

void NavigationManager::update(const ButtonHandler& buttons, const ActionMenu& menu) {
    // Reset the confirm flag at the start of every frame so that it is only
    // true for the single loop iteration in which the player pressed A.
    // If we did not reset it here, confirmAction() would fire every frame
    // after the first press — feeding the pet repeatedly without extra presses.
    confirmActionRequested = false;

    switch (currentScreen) {
        case SCREEN_MAIN:
            handleMainScreenInput(buttons);
            break;
        case SCREEN_STATS:
            handleStatsScreenInput(buttons);
            break;
        case SCREEN_INTERACT:
            handleInteractScreenInput(buttons, menu);
            break;
    }
}

void NavigationManager::handleMainScreenInput(const ButtonHandler& buttons) {
    // B and C both toggle between the two tabs (Stats / Interact).
    // With only two options, pressing either direction flips the selection —
    // there is no meaningful difference between "next" and "previous" here.
    if (buttons.wasButtonBPressed() || buttons.wasButtonCPressed()) {
        mainNavIndex = (mainNavIndex == 0) ? 1 : 0;
    }

    // A enters whichever tab is currently highlighted.
    if (buttons.wasButtonAPressed()) {
        if (mainNavIndex == 0) {
            currentScreen = SCREEN_STATS;
        } else {
            currentScreen = SCREEN_INTERACT;
        }
    }
}

void NavigationManager::handleStatsScreenInput(const ButtonHandler& buttons) {
    // Stats is a read-only screen — there is nothing to select or confirm.
    // Any button press returns the player to the Main screen.
    if (buttons.wasButtonBPressed() || buttons.wasButtonCPressed() || buttons.wasButtonAPressed()) {
        currentScreen = SCREEN_MAIN;
    }
}

void NavigationManager::handleInteractScreenInput(const ButtonHandler& buttons, const ActionMenu& menu) {
    // B and C are NOT handled here — loop() calls menu.update(buttons) first
    // so they cycle through actions rather than triggering a screen switch.
    // This handler only cares about A: either go back to Main or confirm an action.
    if (buttons.wasButtonAPressed()) {
        if (menu.isBackSelected()) {
            currentScreen = SCREEN_MAIN;
        } else {
            confirmActionRequested = true;
        }
    }
}

ScreenState NavigationManager::getCurrentScreen() const {
    return currentScreen;
}

int NavigationManager::getMainNavIndex() const {
    return mainNavIndex;
}

bool NavigationManager::shouldConfirmAction() const {
    return confirmActionRequested;
}
