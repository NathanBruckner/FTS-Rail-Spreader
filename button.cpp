#include "button.h"
#include "debug.h"
#include <Arduino.h>

Button::Button(int pin)
    : pin_(pin), current_(false), last_(false), pressedEvent_(false),
      lastDebounce_(0) {}
Button::~Button() {};
void Button ::begin() { pinMode(pin_, INPUT_PULLUP); }

void Button::update() {
  bool reading = (digitalRead(pin_) == LOW);

  // debounce
  if (reading != last_) {
    lastDebounce_ = millis();
  }

  if (millis() - lastDebounce_ > 20) {
    // detect ANY state change (toggle switch flips)
    if (reading != current_) {
      current_ = reading;
      pressedEvent_ = true;   // <-- fire event ON BOTH edges
      DBG_PRINTLN("TOGGLE DETECTED");
    }
  }

  last_ = reading;
}
bool Button::isPressed() {
  if (pressedEvent_) 
  DBG_PRINTLN("BUTTON EVENT!");
  
  return current_;
}
bool Button::wasPressed() {
  
  if (pressedEvent_) {
    DBG_PRINT("waspressed");
    pressedEvent_ = false;
    return true;
  }
  return false;
}
