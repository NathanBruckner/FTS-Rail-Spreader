#include "controller.h"
#include "debug.h"
#include <math.h>
Controller::Controller() {
  mode_ = AUTO;
  gaugeIndex_ = 0;
  lastValue_ = 0;
  currentValue_ = 0;
  direction_ = STABLE;
  triggered_ = false;
  timingActive_ = false;
  startTime_ = 0;
  movementAccum_ = 0;
  triggerFlag_ = false;
}

void Controller::setMode(Mode m) { mode_ = m; }
Controller::Mode Controller::getMode() { return mode_; }
void Controller::nextGauge() {
  DBG_PRINT("next gauge triggered");
  gaugeIndex_++;
  if (gaugeIndex_ >= NUM_GAUGES) {
    DBG_PRINT("gauge index exceeds limits");
    gaugeIndex_ = NUM_GAUGES - 1;
  }
}
void Controller::prevGauge() { // cycles between the rail weights
  DBG_PRINT("previous gauge triggered");
  gaugeIndex_--;
  if (gaugeIndex_ < 0) {
    gaugeIndex_ = 0;
    DBG_PRINT(
        "gauge index is lower than limits"); // debug statement. To disable all
                                             // debug statements over serial,
                                             // change the macro in debug.h
  }
}
float Controller::getGauge() { return gauges_[gaugeIndex_]; }

float Controller::getDisplayValue() {
  // returns the value that is sent to the display as a measuarement readout
  float base = gauges_[gaugeIndex_];
  if (direction_ == EXPANDING)
    return base + currentValue_ - offset_expand_;
  else if (direction_ == CONTRACTING)
    return base + currentValue_ - offset_contract_;
  else
    return currentValue_;
}

void Controller::updateDirection(
    float delta) { // needed to keep track of whether the tool is extending or
                   // contracting so that the correct measurement offset can be
                   // applied
  if (delta > directionEps_)
    direction_ = EXPANDING;
  else if (delta < -directionEps_)
    direction_ = CONTRACTING;
  // if direction doesn't change, the last direction will be kept
}

float Controller::computeTarget() { // computes where the tool should disengage
                                    // based on the direction it is moving
  float base = gauges_[gaugeIndex_];
  if (direction_ == EXPANDING)
    return base + offset_expand_;
  else if (direction_ == CONTRACTING)
    return base + offset_contract_;
  else
    return base;
}

Controller::Direction Controller::getDirection() { return direction_; }

void Controller::update(float value) {
  if (mode_ == MANUAL) {
    triggerFlag_ = false;
    triggered_ = false;
  }
  // triggerFlag_ = false;
  currentValue_ = value;
  history[index] = currentValue_;
  int oldIndex = (index + 1) % N;
  float delta = currentValue_ - history[oldIndex];
  index = (index + 1) % N;
  updateDirection(delta);

  float target = computeTarget();
  // float error = currentValue_ + gauges_[gaugeIndex_] - target;
  float absoluteValue = gauges_[gaugeIndex_] + currentValue_;
  float error = absoluteValue - target;

  DBG_PRINT("error: ");
  DBG_PRINT(error);
  DBG_PRINT(" triggered: ");
  DBG_PRINTLN(triggered_);

  DBG_PRINT("Mode: ");
  DBG_PRINTLN(mode_ == AUTO ? "AUTO" : "MANUAL");

  // this is where most of the trigger logic is found
  if (mode_ == AUTO) {
    if (!triggered_ && fabs(error) <= tolerance_) {
      DBG_PRINT("Triggered");
      triggerFlag_ = true;
      triggered_ = true;
    }

    // hysterisis reset
    if (triggered_ && fabs(error) > tolerance_ * releaseFactor_) {
      triggered_ = false;
    }

    // timer auto shutoff logic
    if (direction_ == EXPANDING && !timingActive_) {
      timingActive_ = true;
      startTime_ = millis();
      movementAccum_ = 0;
    }

    if (fabs(delta) > movementEps_) {
      movementAccum_ += fabs(delta);
    }

    if (timingActive_ && millis() - startTime_ >= waitTime_) {
      float avgRate = movementAccum_ / waitTime_;
      if (avgRate > movementEps_) {
        triggerFlag_ = true;
      }
      timingActive_ = false;
    }
  }
  lastValue_ = currentValue_;
}

bool Controller::shouldTrigger() {
  if (triggerFlag_) {
    triggerFlag_ = false;
    return true;
  }
  return false;
}

float Controller::getTarget() { return computeTarget(); }
