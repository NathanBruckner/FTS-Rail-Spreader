#ifndef __CONTROLLER__
#define __CONTROLLER__
#include <Arduino.h>

class Controller {
public:
  enum Mode { AUTO, MANUAL };
  enum Direction { EXPANDING, CONTRACTING, STABLE };

  Controller();

  void setMode(Mode m);
  Mode getMode();

  void nextGauge();
  void prevGauge();

  float getGauge();        // selected gauge
  float getDisplayValue(); // compensated value

  void update(float value); // call every loop

  bool shouldTrigger(); // one-shot trigger signal
  float getTarget();

  Direction getDirection();

private:
  // --- configuration ---
  static const int NUM_GAUGES = 5;
  float gauges_[NUM_GAUGES] = {56.5, 57.0, 58.0, 59.0, 60.0};

  float tolerance_ = 0.0625;  // 1/16 inch
  float releaseFactor_ = 1.2; // hysteresis multiplier

  float offset_expand_ = 3.520 + 0.4; // tune these experimentally
  float offset_contract_ = 1.732;

  float directionEps_ = 0.06; // noise threshold
  float movementEps_ = 0.01;  // ignore tiny noise

  // --- state ---
  Mode mode_;
  int gaugeIndex_;

  float lastValue_;
  float currentValue_;
  Direction direction_;

  bool triggered_;

  // --- fallback timer ---
  float waitTime_ = 20000;
  bool timingActive_;
  unsigned long startTime_;
  float movementAccum_;

  // --- trigger output ---
  bool triggerFlag_;

  static const int N = 30; // prevents sampling too close to each other. Helps
                           // to update direction properly.
  float history[N];        // array of previous raw measurement values
  int index = 0;

  // --- helpers ---
  float computeTarget();
  void updateDirection(float delta);
};
#endif
