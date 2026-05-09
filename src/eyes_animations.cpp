#include "eyes_animations.h"

EyeAnimator::EyeAnimator(RoboEyes<Adafruit_SSD1306>& e) : eyes(e) {
  lastExpressionChange = 0;
  lastBlinkTime = 0;
  randomModeActive = true;
  currentEmotion = ROBODEFAULT;
}

void EyeAnimator::init() {
  eyes.setWidth(36, 36);
  eyes.setHeight(36, 36);
  eyes.setBorderradius(8, 8);
  eyes.setSpacebetween(10);
  eyes.setIdleMode(true, 4, 2);
  eyes.setAutoblinker(true, 2, 3);
  eyes.setCuriosity(true);
  eyes.setPosition(ROBODEFAULT);
  setDefault();
}

void EyeAnimator::update() {
  if(!randomModeActive) return;
  
  if(millis() - lastExpressionChange > 15000) {
    randomExpression();
    lastExpressionChange = millis();
  }
}

void EyeAnimator::setRandomMode(bool active) {
  randomModeActive = active;
  if(active) {
    lastExpressionChange = millis();
    randomExpression();
  }
}

void EyeAnimator::forceNextExpression() {
  randomExpression();
  lastExpressionChange = millis();
}

void EyeAnimator::setSpecificExpression(int emotion) {
  randomModeActive = false;
  currentEmotion = emotion;
  switch(emotion) {
    case 0: setDefault(); break;
    case 1: setHappy(); break;
    case 2: setAngry(); break;
    case 3: setTired(); break;
    case 4: setCurious(); break;
    case 5: setSurprised(); break;
  }
}

void EyeAnimator::setDefault() {
  eyes.setMood(ROBODEFAULT);
  eyes.setCuriosity(false);
  currentEmotion = ROBODEFAULT;
}

void EyeAnimator::setHappy() {
  eyes.setMood(HAPPY);
  eyes.setCuriosity(false);
  currentEmotion = HAPPY;
}

void EyeAnimator::setAngry() {
  eyes.setMood(ANGRY);
  eyes.setCuriosity(false);
  currentEmotion = ANGRY;
}

void EyeAnimator::setTired() {
  eyes.setMood(TIRED);
  eyes.setCuriosity(false);
  currentEmotion = TIRED;
}

void EyeAnimator::setCurious() {
  eyes.setMood(ROBODEFAULT);
  eyes.setCuriosity(true);
  currentEmotion = ROBODEFAULT;
}

void EyeAnimator::setSurprised() {
  // Удивление - большие круглые глаза
  eyes.setWidth(42, 42);
  eyes.setHeight(42, 42);
  eyes.setBorderradius(20, 20);
  eyes.setMood(ROBODEFAULT);
  delay(2000); 
  eyes.setWidth(36, 36);
  eyes.setHeight(36, 36);
  eyes.setBorderradius(8, 8);
}

void EyeAnimator::randomExpression() {
  int r = random(emotionsCount);
  
  switch(r) {
    case 0:
      setDefault();
      Serial.println("Eyes: DEFAULT");
      break;
    case 1:
      setHappy();
      Serial.println("Eyes: HAPPY");
      break;
    case 2:
      setAngry();
      Serial.println("Eyes: ANGRY");
      break;
    case 3:
      setTired();
      Serial.println("Eyes: TIRED");
      break;
    case 4:
      setCurious();
      Serial.println("Eyes: CURIOUS");
      break;
  }
  
  if(random(3) == 0) {
    delay(100);
    eyes.blink();
  }
}