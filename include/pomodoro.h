#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"
#include "display.h"

enum PomodoroState {
  NORMAL_MODE,
  WORK_MODE,
  BREAK_MODE,
  COMPLETED_MODE,
  SETTINGS_MODE
};

class PomodoroTimer {
private:
  RoboEyes<Adafruit_SSD1306>& eyes;
  DisplayManager& display;
  
  PomodoroState currentState;
  PomodoroState previousState;
  
  unsigned long timerEnd;
  int remainingTime;
  int workDuration;
  int breakDuration;
  int pomodoroCycles;
  
  bool running;
  bool paused;
  
  unsigned long lastDisplayUpdate;
  unsigned long lastEyeUpdate;  // Для управления частотой обновления глаз
  
public:
  PomodoroTimer(RoboEyes<Adafruit_SSD1306>& e, DisplayManager& d);
  
  void update();
  void start();
  void pause();
  void resume();
  void reset();
  void setWorkDuration(int minutes);
  void setBreakDuration(int minutes);
  
  PomodoroState getState();
  bool isRunning();
  bool isPaused();
  void showStatus();
  
private:
  void updateState();
  void enterNormalMode();
  void enterWorkMode();
  void enterBreakMode();
  void enterCompletedMode();
  void enterSettingsMode();
  void timerComplete();
  void updateDisplay();
  void celebrate();
};

#endif