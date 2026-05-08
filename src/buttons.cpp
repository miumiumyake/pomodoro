#include "buttons.h"

ButtonManager::ButtonManager() {
  // Инициализация состояний
  lastButtonAState = HIGH;
  lastButtonBState = HIGH;
  buttonAState = HIGH;
  buttonBState = HIGH;
  
  // Инициализация времени
  lastDebounceTimeA = 0;
  lastDebounceTimeB = 0;
  pressStartTimeA = 0;
  pressStartTimeB = 0;
  lastProcessTimeA = 0;
  lastProcessTimeB = 0;
  
  // Инициализация флагов
  longPressTriggeredA = false;
  longPressTriggeredB = false;
  pressProcessedA = false;
  pressProcessedB = false;
  
  // Инициализация буферов
  sampleIndexA = 0;
  sampleIndexB = 0;
  for(int i = 0; i < BTN_REQUIRED_SAMPLES; i++) {
    sampleBufferA[i] = HIGH;
    sampleBufferB[i] = HIGH;
  }
}

void ButtonManager::init() {
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
  Serial.println("TTP223 - ULTRA LOW SENSITIVITY MODE");
  Serial.print("Debounce: "); Serial.print(BTN_DEBOUNCE_DELAY); Serial.println("ms");
  Serial.print("Required samples: "); Serial.println(BTN_REQUIRED_SAMPLES);
}

bool ButtonManager::isStableReading(int pin, int* buffer, int& index) {
  // Читаем текущее значение
  int reading = digitalRead(pin);
  
  // Добавляем в буфер
  buffer[index] = reading;
  index = (index + 1) % BTN_REQUIRED_SAMPLES;
  
  // Проверяем, все ли значения одинаковые
  int first = buffer[0];
  for(int i = 1; i < BTN_REQUIRED_SAMPLES; i++) {
    if(buffer[i] != first) {
      return false; // Нестабильно
    }
  }
  return true; // Стабильно
}

ButtonAction ButtonManager::update() {
  unsigned long currentTime = millis();
  
  // ========== Кнопка A ==========
  if(currentTime - lastProcessTimeA > BTN_IGNORE_AFTER_PRESS) {
    
    // Стабильное чтение с несколькими сэмплами
    bool stableA = isStableReading(BUTTON_A_PIN, sampleBufferA, sampleIndexA);
    
    if(stableA) {
      bool readingA = (sampleBufferA[0] == LOW);
      
      if(readingA != lastButtonAState) {
        lastDebounceTimeA = currentTime;
      }
      
      if((currentTime - lastDebounceTimeA) > BTN_DEBOUNCE_DELAY) {
        if(readingA != buttonAState) {
          buttonAState = readingA;
          
          if(buttonAState == LOW) {
            // Нажатие
            pressStartTimeA = currentTime;
            longPressTriggeredA = false;
            pressProcessedA = false;
          }
        }
      }
      
      // Обработка нажатия (только после задержки)
      if(buttonAState == LOW && !pressProcessedA && 
         (currentTime - pressStartTimeA) > BTN_PRESS_DELAY) {
        
        // Проверяем, не длинное ли нажатие
        if(!longPressTriggeredA && (currentTime - pressStartTimeA) >= BTN_LONG_PRESS_TIME) {
          longPressTriggeredA = true;
          pressProcessedA = true;
          lastProcessTimeA = currentTime;
          Serial.println("A: LONG PRESS");
          return BUTTON_A_LONG;
        }
        
        // Короткое нажатие
        if(!longPressTriggeredA && (currentTime - pressStartTimeA) < BTN_LONG_PRESS_TIME) {
          pressProcessedA = true;
          lastProcessTimeA = currentTime;
          Serial.println("A: SHORT PRESS");
          delay(50);
          return BUTTON_A_PRESS;
        }
      }
      
      // Сброс при отпускании
      if(buttonAState == HIGH && pressProcessedA) {
        pressProcessedA = false;
        longPressTriggeredA = false;
      }
      
      lastButtonAState = readingA;
    }
  }
  
  // ========== Кнопка B ==========
  if(currentTime - lastProcessTimeB > BTN_IGNORE_AFTER_PRESS) {
    
    bool stableB = isStableReading(BUTTON_B_PIN, sampleBufferB, sampleIndexB);
    
    if(stableB) {
      bool readingB = (sampleBufferB[0] == LOW);
      
      if(readingB != lastButtonBState) {
        lastDebounceTimeB = currentTime;
      }
      
      if((currentTime - lastDebounceTimeB) > BTN_DEBOUNCE_DELAY) {
        if(readingB != buttonBState) {
          buttonBState = readingB;
          
          if(buttonBState == LOW) {
            pressStartTimeB = currentTime;
            longPressTriggeredB = false;
            pressProcessedB = false;
          }
        }
      }
      
      if(buttonBState == LOW && !pressProcessedB && 
         (currentTime - pressStartTimeB) > BTN_PRESS_DELAY) {
        
        if(!longPressTriggeredB && (currentTime - pressStartTimeB) >= BTN_LONG_PRESS_TIME) {
          longPressTriggeredB = true;
          pressProcessedB = true;
          lastProcessTimeB = currentTime;
          Serial.println("B: LONG PRESS");
          return BUTTON_B_LONG;
        }
        
        if(!longPressTriggeredB && (currentTime - pressStartTimeB) < BTN_LONG_PRESS_TIME) {
          pressProcessedB = true;
          lastProcessTimeB = currentTime;
          Serial.println("B: SHORT PRESS");
          delay(50);
          return BUTTON_B_PRESS;
        }
      }
      
      if(buttonBState == HIGH && pressProcessedB) {
        pressProcessedB = false;
        longPressTriggeredB = false;
      }
      
      lastButtonBState = readingB;
    }
  }
  
  return NO_ACTION;
}