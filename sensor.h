#ifndef __SENSOR__
#define __SENSOR__
#include <VL53L4CD.h>
class Sensor {
public:
  void begin();
  void update();
  double getFiltered();

private:
  VL53L4CD sensor;
  static const int N = 10;
  double numSamp_ = static_cast<double>(N);
  float buffer[N];
  int index = 0;
};
#endif
