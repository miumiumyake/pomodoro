#include "display.h"

DisplayManager::DisplayManager(Adafruit_SSD1306& disp, RoboEyes<Adafruit_SSD1306>& e) 
  : display(disp), eyes(e) {
  currentMode = DISPLAY_EYES;
  needClear = true;
}

void DisplayManager::init() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  currentMode = DISPLAY_EYES;
}

void DisplayManager::setMode(DisplayMode mode) {
  if(currentMode != mode) {
    currentMode = mode;
    display.clearDisplay();
    display.display();
    delay(50);
  }
}

DisplayMode DisplayManager::getMode() {
  return currentMode;
}

void DisplayManager::clear() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void DisplayManager::showEyes() {
  setMode(DISPLAY_EYES);
  display.clearDisplay();
  display.display();
}

void DisplayManager::showMessage(const char* line1, const char* line2, int duration) {
  setMode(DISPLAY_MESSAGE);
  clear();
  display.setCursor((128 - strlen(line1) * 6) / 2, 25);
  display.print(line1);
  if(strlen(line2) > 0) {
    display.setCursor((128 - strlen(line2) * 6) / 2, 40);
    display.print(line2);
  }
  display.display();
  delay(duration); 
}

void DisplayManager::showTimer(int minutes, int seconds, bool isPaused, const char* mode) {
  if(currentMode != DISPLAY_TIMER) {
    setMode(DISPLAY_TIMER);
  }
  
  char timeStr[10];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  
  clear();
  
  display.setTextSize(3);
  display.setCursor((128 - strlen(timeStr) * 18) / 2, 25);
  display.print(timeStr);
  
  if(isPaused) {
    display.setTextSize(1);
    display.setCursor(49, 55);
    display.print("PAUSED");
  }
  
  display.display();
}

void DisplayManager::showProgress(int elapsed, int total) {
  int progressWidth = map(elapsed, 0, total, 0, 124);
  display.drawRect(2, 62, 124, 2, SSD1306_WHITE);
  display.fillRect(3, 62, progressWidth, 2, SSD1306_WHITE);
  display.display();
}

void DisplayManager::showNormalModeMessage() {
  setMode(DISPLAY_MESSAGE);
  clear();
  display.setCursor(25, 20);
  display.print("Robot Mode");
  display.setCursor(15, 40);
  display.print("A+B = Settings");
  display.display();
  delay(2500);  
  showEyes();
}

void DisplayManager::showWorkModeMessage(int minutes) {
  setMode(DISPLAY_MESSAGE);
  char msg[20];
  sprintf(msg, "Work: %d min", minutes);
  clear();
  display.setCursor((128 - strlen("Work Mode!") * 6) / 2, 25);
  display.print("Work Mode!");
  display.setCursor((128 - strlen(msg) * 6) / 2, 40);
  display.print(msg);
  display.display();
  delay(1500);
}

void DisplayManager::showBreakModeMessage(int minutes) {
  setMode(DISPLAY_MESSAGE);
  char msg[20];
  sprintf(msg, "Break: %d min", minutes);
  clear();
  display.setCursor((128 - strlen("Break Time!") * 6) / 2, 25);
  display.print("Break Time!");
  display.setCursor((128 - strlen(msg) * 6) / 2, 40);
  display.print(msg);
  display.display();
  delay(1500);
}

void DisplayManager::showPauseMessage() {
  setMode(DISPLAY_MESSAGE);
  clear();
  display.setCursor(39, 25);
  display.print("PAUSED");
  display.setCursor(24, 40);
  display.print("A: Resume B: Cancel");
  display.display();
  delay(1500);
  setMode(DISPLAY_TIMER);
}

void DisplayManager::showCompleteMessage(int cycles) {
  setMode(DISPLAY_MESSAGE);
  clear();
  display.setCursor(15, 25);
  display.print("Cycle Complete!");
  display.setCursor(20, 40);
  display.print("Cycles: ");
  display.print(cycles);
  display.display();
  delay(2500);
  showEyes();
}

void DisplayManager::showSettingsMenu(int workMin, int breakMin, int selected) {
  setMode(DISPLAY_SETTINGS);
  clear();
  
  display.setCursor(45, 5);
  display.print("SETTINGS");
  display.drawLine(20, 12, 108, 12, SSD1306_WHITE);
  
  if(selected == 0) {
    display.fillRect(5, 20, 118, 15, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  }
  display.setCursor(15, 24);
  display.print("Work: ");
  display.print(workMin);
  display.print(" min");
  
  display.setTextColor(SSD1306_WHITE);
  
  if(selected == 1) {
    display.fillRect(5, 40, 118, 15, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  }
  display.setCursor(15, 44);
  display.print("Break: ");
  display.print(breakMin);
  display.print(" min");
  
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(0);
  display.setCursor(10, 60);
  display.print("A+B:Switch  A:Exit");
  
  display.display();
  // НЕТ вызова showEyes() или showTimer() - только меню!
}