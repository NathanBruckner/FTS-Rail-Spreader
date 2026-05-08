#include "solenoid.h"
#include "debug.h"
#include <Arduino.h>

Solenoid::Solenoid(int pin) : pin_(pin), active_(false), startTime_(0) {};
Solenoid::~Solenoid() {};

void Solenoid::begin() {
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
}

void Solenoid::trigger() {
  if (!active_) {
    digitalWrite(pin_, HIGH);

    DBG_PRINTLN("Solenoid has triggered");
    active_ = true;
    startTime_ = millis();
  }
}

void Solenoid::update() {
  if (active_ && millis() - startTime_ >= 2000) {
    digitalWrite(pin_, LOW);
    active_ = false;
  }
}

bool Solenoid::isActive() { return active_; };
