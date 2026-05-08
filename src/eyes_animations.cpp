#include "eyes_animations.h"
#include "display.h"

EyeAnimator::EyeAnimator(RoboEyes<Adafruit_SSD1306>& e) : eyes(e) {
  lastExpressionChange = 0;
  currentExpression = 0;
}

void EyeAnimator::init() {
  eyes.setWidth(36, 36);
  eyes.setHeight(36, 36);
  eyes.setBorderradius(8, 8);
  eyes.setSpacebetween(10);
  eyes.setIdleMode(true, 3, 2);
  eyes.setAutoblinker(true, 2, 3);
  eyes.setCuriosity(true);
  eyes.setPosition(ROBODEFAULT);
}

void EyeAnimator::update() {
  if(millis() - lastExpressionChange > 15000) {
    randomExpression();
    lastExpressionChange = millis();
  }
}

void EyeAnimator::randomExpression() {
  int r = random(5);
  
  switch(r) {
    case 0:
      eyes.setMood(ROBODEFAULT);
      break;
    case 1:
      eyes.setMood(HAPPY);
      delay(2000);
      eyes.setMood(ROBODEFAULT);
      break;
    case 2:
      eyes.anim_confused();
      break;
    case 3:
      eyes.anim_laugh();
      break;
    case 4:
      eyes.setMood(TIRED);
      delay(2000);
      eyes.setMood(ROBODEFAULT);
      break;
  }
}

void EyeAnimator::celebrate() {
  for(int i = 0; i < 5; i++) {
    eyes.anim_laugh();
    delay(100);
  }
}

void EyeAnimator::blinkSequence(int count) {
  for(int i = 0; i < count; i++) {
    eyes.blink();
    delay(150);
  }
}