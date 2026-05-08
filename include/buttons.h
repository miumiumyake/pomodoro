#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// Пины для сенсорных кнопок TTP223
#define BUTTON_A_PIN D5  // GPIO14 - первая кнопка
#define BUTTON_B_PIN D6  // GPIO12 - вторая кнопка

// МАКСИМАЛЬНЫЕ задержки для снижения чувствительности
#define BTN_DEBOUNCE_DELAY 300      // Антидребезг 300ms
#define BTN_PRESS_DELAY 250         // Задержка после нажатия 250ms  
#define BTN_LONG_PRESS_TIME 1500    // Длительное нажатие 1.5 сек
#define BTN_MIN_PRESS_TIME 100      // Минимальное время нажатия 100ms
#define BTN_IGNORE_AFTER_PRESS 500  // Игнорировать кнопку 500ms после нажатия
#define BTN_REQUIRED_SAMPLES 5      // Требуется 5 одинаковых показаний

enum ButtonAction {
  NO_ACTION,
  BUTTON_A_PRESS,
  BUTTON_B_PRESS,
  BUTTON_A_LONG,
  BUTTON_B_LONG
};

class ButtonManager {
private:
  bool buttonAState;
  bool buttonBState;
  bool lastButtonAState;
  bool lastButtonBState;
  
  unsigned long lastDebounceTimeA;
  unsigned long lastDebounceTimeB;
  unsigned long pressStartTimeA;
  unsigned long pressStartTimeB;
  unsigned long lastProcessTimeA;
  unsigned long lastProcessTimeB;
  
  bool longPressTriggeredA;
  bool longPressTriggeredB;
  bool pressProcessedA;
  bool pressProcessedB;
  
  int sampleBufferA[BTN_REQUIRED_SAMPLES];
  int sampleBufferB[BTN_REQUIRED_SAMPLES];
  int sampleIndexA;
  int sampleIndexB;
  
  bool isStableReading(int pin, int* buffer, int& index);
  
public:
  ButtonManager();
  void init();
  ButtonAction update();
};

#endif