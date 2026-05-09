#include "pomodoro.h"

PomodoroTimer::PomodoroTimer(RoboEyes<Adafruit_SSD1306>& e, Adafruit_SSD1306& d) 
  : eyes(e), display(d) {
  currentState = NORMAL_MODE;
  pomodoroCycles = 0;
  isRunning = false;
  remainingTime = 25 * 60;
  lastDisplayUpdate = 0;
  exitConfirmRequested = false;
  exitConfirmTime = 0;
}

void PomodoroTimer::update() {
  if(!isRunning) return;
  
  if(millis() >= timerEnd) {
    if(currentState == WORK_MODE) {
      startBreak();
    } else if(currentState == BREAK_MODE) {
      stop();
    }
    return;
  }
  
  if(millis() - lastDisplayUpdate >= 1000) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
}

void PomodoroTimer::press() {
  // Сбрасываем подтверждение выхода при любом нажатии A
  exitConfirmRequested = false;
  
  if(currentState == NORMAL_MODE) {
    startWork();
  } 
  else if(currentState == WORK_MODE && isRunning) {
    isRunning = false;
    remainingTime = (timerEnd - millis()) / 1000;
    showMessage("PAUSED", "Press A to continue", 1500);
    updateDisplay();
  } 
  else if(currentState == WORK_MODE && !isRunning) {
    timerEnd = millis() + (remainingTime * 1000);
    isRunning = true;
    showMessage("CONTINUE", "", 1000);
    updateDisplay();
  } 
  else if(currentState == BREAK_MODE) {
    stop();
  }
}

void PomodoroTimer::pressB() {
  if(currentState == NORMAL_MODE) {
    // В обычном режиме - смена эмоции
    int r = random(4);
    if(r == 0) eyes.setMood(ROBODEFAULT);
    else if(r == 1) eyes.setMood(HAPPY);
    else if(r == 2) eyes.setMood(ANGRY);
    else eyes.setMood(TIRED);
    eyes.blink();
    showMessage("MOOD", "Changed!", 1000);
  }
  else if((currentState == WORK_MODE || currentState == BREAK_MODE) && isRunning) {
    // В режиме таймера - запрос подтверждения выхода
    if(!exitConfirmRequested) {
      exitConfirmRequested = true;
      exitConfirmTime = millis();
      showExitConfirm();
    } else {
      // Повторное нажатие - подтверждаем выход
      exitConfirmRequested = false;
      stop();
      showMessage("EXIT", "Back to robot mode", 1500);
      display.clearDisplay();
      display.display();
    }
  }
  else if((currentState == WORK_MODE || currentState == BREAK_MODE) && !isRunning) {
    // На паузе - просто выходим без подтверждения
    exitConfirmRequested = false;
    stop();
    showMessage("EXIT", "Back to robot mode", 1500);
    display.clearDisplay();
    display.display();
  }
}

PomodoroState PomodoroTimer::getState() {
  return currentState;
}

void PomodoroTimer::startWork() {
  currentState = WORK_MODE;
  isRunning = true;
  remainingTime = 25 * 60;
  timerEnd = millis() + (remainingTime * 1000);
  pomodoroCycles++;
  exitConfirmRequested = false;
  
  display.clearDisplay();
  display.display();
  
  showMessage("WORK", "25 minutes", 2000);
  updateDisplay();
}

void PomodoroTimer::startBreak() {
  currentState = BREAK_MODE;
  isRunning = true;
  remainingTime = 5 * 60;
  timerEnd = millis() + (remainingTime * 1000);
  exitConfirmRequested = false;
  
  display.clearDisplay();
  display.display();
  
  showMessage("BREAK", "5 minutes", 2000);
  updateDisplay();
  
  for(int i = 0; i < 2; i++) {
    eyes.blink();
    delay(200);
  }
}

void PomodoroTimer::stop() {
  currentState = NORMAL_MODE;
  isRunning = false;
  exitConfirmRequested = false;
  
  char msg[20];
  sprintf(msg, "Cycles: %d", pomodoroCycles);
  showMessage("COMPLETE!", msg, 2500);
  
  eyes.setMood(ROBODEFAULT);
  eyes.setIdleMode(true, 3, 2);
  eyes.setAutoblinker(true, 2, 3);
  eyes.open();
  
  display.clearDisplay();
  display.display();
}

void PomodoroTimer::updateDisplay() {
  if(!isRunning) return;
  
  int timeToShow = (timerEnd - millis()) / 1000;
  if(timeToShow < 0) timeToShow = 0;
  
  showTimer(timeToShow / 60, timeToShow % 60);
}

void PomodoroTimer::showTimer(int minutes, int seconds) {
  char timeStr[10];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((128 - strlen(timeStr) * 18) / 2, 20);
  display.print(timeStr);
  
  int total = (currentState == WORK_MODE) ? 25 * 60 : 5 * 60;
  int elapsed = total - (minutes * 60 + seconds);
  int progress = map(elapsed, 0, total, 0, 124);
  display.drawRect(2, 58, 124, 3, SSD1306_WHITE);
  display.fillRect(3, 59, progress, 2, SSD1306_WHITE);
  
  display.display();
}

void PomodoroTimer::showMessage(const char* line1, const char* line2, int duration) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor((128 - strlen(line1) * 6) / 2, 25);
  display.print(line1);
  if(strlen(line2) > 0) {
    display.setCursor((128 - strlen(line2) * 6) / 2, 40);
    display.print(line2);
  }
  display.display();
  delay(duration);
}

void PomodoroTimer::showExitConfirm() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print("Exit Pomodoro?");
  display.setCursor(15, 40);
  display.print("B again to confirm");
  display.display();
  delay(2000);
  

  if(exitConfirmRequested && currentState != NORMAL_MODE) {
    updateDisplay();
  }
}