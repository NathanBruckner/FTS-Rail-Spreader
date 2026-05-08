#ifndef __BUTTON__
#define __BUTTON__
#include <string>

class Button {
public:
  Button(int pin);
  ~Button();

  void begin();
  void update();
  bool isPressed();
  bool wasPressed();

private:
  int pin_;
  bool current_;
  bool last_;
  bool pressedEvent_;
  unsigned long lastDebounce_;
};

#endif
