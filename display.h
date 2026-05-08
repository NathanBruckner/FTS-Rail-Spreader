#pragma once
#include "controller.h"
#include <LiquidCrystal.h>

class Display {
public:
  Display(int rs, int en, int d0, int d1, int d2, int d3, int d4, int d5,
          int d6, int d7);

  void begin();
  void update(float distance, Controller::Direction direction, float gauge,
              bool isAuto);

private:
  LiquidCrystal lcd;

  void printPadded(const char *label, float value, int width);
};
