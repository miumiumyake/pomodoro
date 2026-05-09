#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BUTTON_A_PIN D5
#define BUTTON_B_PIN D6
#define DEBOUNCE_DELAY 80

enum ButtonAction {
  NO_ACTION,
  BUTTON_A_PRESS,
  BUTTON_B_PRESS
};

class ButtonManager {
private:
  bool lastA, lastB;
  bool stateA, stateB;
  unsigned long lastDebounceA, lastDebounceB;
  
public:
  ButtonManager();
  void init();
  ButtonAction update();
};

#endif