#include "sensor.h"
#include "debug.h"
#include <Arduino.h>
#include <Wire.h>
void Sensor::begin() {
  Wire.begin();
  sensor.init();
  sensor.setRangeTiming(50, 100);
  sensor.startContinuous();
  for (int i = 0; i < N; i++)
    buffer[i] = 0;
}

void Sensor::update() {

  if (sensor.dataReady()) {
    // DBG_PRINTLN("data is ready");
    buffer[index] = sensor.read() * 0.03937;
    sensor.clearInterrupt();
    index = (index + 1) % N;
  }
}

double Sensor::getFiltered() {
  float sum = 0;
  for (int i = 0; i < N; i++)
    sum += buffer[i];
  return sum / numSamp_;
}
