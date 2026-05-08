#include "button.h"
#include "controller.h"
#include "display.h"
#include "sensor.h"
#include "solenoid.h"

Display display(10, 8, 7, 6, 5, 4, 3, 2, 22, 23);
Button upButton(25);
Button downButton(24);
Solenoid solenoid(33);
Sensor sensor;
Controller controller;
int modePin = 26;

void setup() {
  display.begin();
  upButton.begin();
  downButton.begin();
  solenoid.begin();
  sensor.begin();
  Serial.begin(115200);
  pinMode(modePin, INPUT_PULLUP);
  while (!Serial && millis() < 3000)
    ;
}

void loop() {

  upButton.update();
  downButton.update();
  solenoid.update();
  sensor.update();
  //  bool isAuto = (digitalRead(modePin)==HIGH);
  bool isAuto = (digitalRead(modePin) == HIGH);

  controller.setMode(isAuto ? Controller::AUTO : Controller::MANUAL);

  float value = sensor.getFiltered();
  controller.update(value);
  if (upButton.wasPressed()) {
    controller.nextGauge();
  }
  if (downButton.wasPressed()) {
    controller.prevGauge();
  }
  if (!isAuto)
    controller.setMode(Controller::MANUAL);
  else
    controller.setMode(Controller::AUTO);
  if (controller.shouldTrigger()) {
    solenoid.trigger();
  }

  display.update(controller.getDisplayValue(), controller.getDirection(),
                 controller.getGauge(), controller.getMode()==Controller::AUTO);

}
