#include "buttons.h"

ButtonManager::ButtonManager() {
  lastA = HIGH; lastB = HIGH;
  stateA = HIGH; stateB = HIGH;
  lastDebounceA = 0; lastDebounceB = 0;
}

void ButtonManager::init() {
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
}

ButtonAction ButtonManager::update() {
  unsigned long now = millis();
  
  // Кнопка A
  bool readingA = digitalRead(BUTTON_A_PIN) == LOW;
  if(readingA != lastA) lastDebounceA = now;
  if((now - lastDebounceA) > DEBOUNCE_DELAY && readingA != stateA) {
    stateA = readingA;
    if(stateA == LOW) {
      lastA = readingA;
      return BUTTON_A_PRESS;
    }
  }
  lastA = readingA;
  
  // Кнопка B
  bool readingB = digitalRead(BUTTON_B_PIN) == LOW;
  if(readingB != lastB) lastDebounceB = now;
  if((now - lastDebounceB) > DEBOUNCE_DELAY && readingB != stateB) {
    stateB = readingB;
    if(stateB == LOW) {
      lastB = readingB;
      return BUTTON_B_PRESS;
    }
  }
  lastB = readingB;
  
  return NO_ACTION;
}