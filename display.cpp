#include "display.h"
#include "debug.h"
#include <Arduino.h>
#include <string.h>

Display::Display(int rs, int en, int d0, int d1, int d2, int d3, int d4, int d5,
                 int d6, int d7)
    : lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7) {}

void Display::begin() {
  // --- CRITICAL FIXES ---
  pinMode(10, OUTPUT); // RS
  pinMode(8, OUTPUT);  // E

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

  delay(100); // LCD power-up delay

  lcd.begin(16, 2);
  lcd.clear();
}

void Display::printPadded(const char *label, float value, int width) {
  lcd.print(label);

  // Print value with 2 decimal places
  lcd.print(value, 2);

  // Pad with spaces to clear leftovers
  int printed = strlen(label) + 5; // approx "xx.xx"
  for (int i = printed; i < width; i++) {
    lcd.print(' ');
  }
}
const char *directionToString(Controller::Direction dir) {
  switch (dir) {
  case Controller::EXPANDING:
    return "Extending";
  case Controller::CONTRACTING:
    return "Contracting";
  default:
    return "";
  }
}

void Display::update(float distance, Controller::Direction direction,
                     float gauge, bool isAuto) {
  // -------- LINE 1 --------

  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(distance, 3);

  lcd.setCursor(8, 0); // fixed column instead of guessing width
  lcd.print(directionToString(direction));
  // -------- LINE 2 --------
  lcd.setCursor(0, 1);

  if (isAuto)
    lcd.print("AUTO ");
  else
    lcd.print("MAN  ");

  lcd.setCursor(8, 1);
  lcd.print("G:");
  lcd.print(gauge, 2);

  // Clear any trailing garbage
  lcd.print("     ");
}
