#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

enum PomodoroState {
  NORMAL_MODE,
  WORK_MODE,
  BREAK_MODE
};

class PomodoroTimer {
private:
  RoboEyes<Adafruit_SSD1306>& eyes;
  Adafruit_SSD1306& display;
  
  PomodoroState currentState;
  unsigned long timerEnd;
  int remainingTime;
  int pomodoroCycles;
  bool isRunning;
  unsigned long lastDisplayUpdate;
  
  bool exitConfirmRequested;
  unsigned long exitConfirmTime;
  
  void startWork();
  void startBreak();
  void stop();
  void updateDisplay();
  void showTimer(int minutes, int seconds);
  void showMessage(const char* line1, const char* line2, int duration = 2000);
  void showExitConfirm();
  
public:
  PomodoroTimer(RoboEyes<Adafruit_SSD1306>& e, Adafruit_SSD1306& d);
  
  void update();
  void press();
  void pressB(); 
  
  PomodoroState getState();
};

#endif