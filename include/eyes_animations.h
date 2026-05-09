#ifndef EYES_ANIMATIONS_H
#define EYES_ANIMATIONS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

class EyeAnimator {
private:
  RoboEyes<Adafruit_SSD1306>& eyes;
  unsigned long lastExpressionChange;
  unsigned long lastBlinkTime;
  bool randomModeActive;
  
 
  const int emotionsCount = 5;
  int currentEmotion;
  
public:
  EyeAnimator(RoboEyes<Adafruit_SSD1306>& e);
  
  void init();
  void update();
  void setRandomMode(bool active);
  void forceNextExpression();
  void setSpecificExpression(int emotion);
  
 
  void setDefault();
  void setHappy();
  void setAngry();
  void setTired();
  void setCurious();
  void setSurprised();  
  
private:
  void randomExpression();
};

#endif