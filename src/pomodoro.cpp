#include "pomodoro.h"

PomodoroTimer::PomodoroTimer(RoboEyes<Adafruit_SSD1306>& e, DisplayManager& d) 
  : eyes(e), display(d) {
  currentState = NORMAL_MODE;
  previousState = NORMAL_MODE;
  workDuration = 25;
  breakDuration = 5;
  pomodoroCycles = 0;
  running = false;
  paused = false;
  remainingTime = workDuration * 60;
  lastDisplayUpdate = 0;
  lastEyeUpdate = 0;
}

void PomodoroTimer::update() {
  if(!running || paused) return;
  
  if(millis() >= timerEnd) {
    timerComplete();
    return;
  }
  
  if(millis() - lastDisplayUpdate >= 1000) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  if(millis() - lastEyeUpdate >= 100) {
    updateState();
    lastEyeUpdate = millis();
  }
}

void PomodoroTimer::start() {
  if(currentState == NORMAL_MODE) {
    enterWorkMode();
  } else if(currentState == COMPLETED_MODE) {
    pomodoroCycles = 0;
    enterWorkMode();
  }
}

void PomodoroTimer::pause() {
  if(running && !paused) {
    running = false;
    paused = true;
    remainingTime = (timerEnd - millis()) / 1000;
    if(remainingTime < 0) remainingTime = 0;
    eyes.setMood(TIRED);
    display.showPauseMenu();
  }
}

void PomodoroTimer::resume() {
  if(!running && paused) {
    timerEnd = millis() + (remainingTime * 1000);
    running = true;
    paused = false;
    if(currentState == WORK_MODE) eyes.setMood(ANGRY);
    else eyes.setMood(HAPPY);
    display.showMessage("Resumed!", "", 800);
  }
}

void PomodoroTimer::reset() {
  running = false;
  paused = false;
  remainingTime = workDuration * 60;
  enterNormalMode();
}

void PomodoroTimer::setWorkDuration(int minutes) {
  workDuration = minutes;
  if(currentState == NORMAL_MODE) {
    remainingTime = workDuration * 60;
  }
}

void PomodoroTimer::setBreakDuration(int minutes) {
  breakDuration = minutes;
}

PomodoroState PomodoroTimer::getState() {
  return currentState;
}

bool PomodoroTimer::isRunning() {
  return running;
}

bool PomodoroTimer::isPaused() {
  return paused;
}

void PomodoroTimer::showStatus() {
  char msg[30];
  sprintf(msg, "Cycles: %d", pomodoroCycles);
  display.showMessage("Pomodoro Status", msg, 1500);
}

void PomodoroTimer::enterNormalMode() {
  currentState = NORMAL_MODE;
  display.setMode(DISPLAY_EYES_ONLY);
  
  eyes.setMood(ROBODEFAULT);
  eyes.setIdleMode(true, 3, 2);
  eyes.setAutoblinker(true, 2, 3);
  eyes.setSweat(false);
  eyes.open();
  
  display.showNormalMode();
}

void PomodoroTimer::enterWorkMode() {
  currentState = WORK_MODE;
  running = true;
  paused = false;
  remainingTime = workDuration * 60;
  timerEnd = millis() + (remainingTime * 1000);
  
  display.setMode(DISPLAY_TIMER_ONLY);
  
  eyes.setMood(ANGRY);
  eyes.setIdleMode(false);
  eyes.setAutoblinker(false);
  eyes.setSweat(true);
  
  display.showWorkMode(workDuration);
}

void PomodoroTimer::enterBreakMode() {
  currentState = BREAK_MODE;
  running = true;
  paused = false;
  remainingTime = breakDuration * 60;
  timerEnd = millis() + (remainingTime * 1000);
  
  display.setMode(DISPLAY_TIMER_ONLY);
  
  eyes.setMood(HAPPY);
  eyes.setIdleMode(true, 2, 1);
  eyes.setAutoblinker(true, 1, 2);
  eyes.setSweat(false);
  
  display.showBreakMode(breakDuration);
  
  for(int i = 0; i < 3; i++) {
    eyes.blink();
    delay(200);
  }
}

void PomodoroTimer::enterCompletedMode() {
  currentState = COMPLETED_MODE;
  running = false;
  paused = false;
  celebrate();
  timerEnd = millis() + 3000;
}

void PomodoroTimer::enterSettingsMode() {
  currentState = SETTINGS_MODE;
  running = false;
  paused = false;
  display.setMode(DISPLAY_SETTINGS_ONLY);
}

void PomodoroTimer::timerComplete() {
  if(currentState == WORK_MODE) {
    pomodoroCycles++;
    enterBreakMode();
  } else if(currentState == BREAK_MODE) {
    enterCompletedMode();
  }
}

void PomodoroTimer::updateDisplay() {
  if(!running && !paused) return;
  
  if(display.getMode() == DISPLAY_TIMER_ONLY) {
    int timeToShow = running ? (timerEnd - millis()) / 1000 : remainingTime;
    if(timeToShow < 0) timeToShow = 0;
    
    int minutes = timeToShow / 60;
    int seconds = timeToShow % 60;
    const char* mode = (currentState == WORK_MODE) ? "WORK" : "BREAK";
    
    display.showTimer(minutes, seconds, paused, mode);
    
    int total = (currentState == WORK_MODE) ? workDuration * 60 : breakDuration * 60;
    int elapsed = total - timeToShow;
    display.showProgress(elapsed, total);
  }
}

void PomodoroTimer::celebrate() {
  eyes.setMood(HAPPY);
  eyes.setCuriosity(false);
  
  for(int i = 0; i < 6; i++) {
    eyes.anim_laugh();
    delay(150);
    eyes.blink();
    delay(150);
  }
  
  display.showComplete(pomodoroCycles);
  delay(2500);
  
  eyes.setCuriosity(true);
}

void PomodoroTimer::updateState() {
  if(previousState != currentState) {
    previousState = currentState;
  }
}