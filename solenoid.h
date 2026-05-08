#ifndef __SOLENOID__
#define __SOLENOID__
class Solenoid {
public:
  Solenoid(int pin);
  ~Solenoid();

  void begin();
  void trigger();
  void update();

  bool isActive();

private:
  int pin_;
  bool active_;
  unsigned long startTime_;
};
#endif
