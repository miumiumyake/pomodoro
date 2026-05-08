#ifndef EYES_ANIMATIONS_H
#define EYES_ANIMATIONS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

class EyeAnimator {
private:
  RoboEyes<Adafruit_SSD1306>& eyes;
  unsigned long lastExpressionChange;
  int currentExpression;
  
public:
  EyeAnimator(RoboEyes<Adafruit_SSD1306>& e);
  
  void init();
  void update();
  void randomExpression();
  void celebrate();
  void blinkSequence(int count);
};

#endif