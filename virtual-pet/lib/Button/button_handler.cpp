#include "button_handler.h"

ButtonHandler::ButtonHandler() 
    : prevButtonA(false), prevButtonB(false), prevButtonC(false) {
}

void ButtonHandler::update() {
    // Store previous states for transition detection
    prevButtonA = M5.BtnA.isPressed();
    prevButtonB = M5.BtnB.isPressed();
    prevButtonC = M5.BtnC.isPressed();
}

bool ButtonHandler::wasButtonAPressed() const {
    // Button was pressed if it's currently pressed and wasn't before
    return M5.BtnA.wasPressed();
}

bool ButtonHandler::wasButtonBPressed() const {
    return M5.BtnB.wasPressed();
}

bool ButtonHandler::wasButtonCPressed() const {
    return M5.BtnC.wasPressed();
}

bool ButtonHandler::isButtonAHeld() const {
    return M5.BtnA.isPressed();
}

bool ButtonHandler::isButtonBHeld() const {
    return M5.BtnB.isPressed();
}

bool ButtonHandler::isButtonCHeld() const {
    return M5.BtnC.isPressed();
}
