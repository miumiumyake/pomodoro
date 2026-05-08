#include "display.h"

DisplayManager::DisplayManager(Adafruit_SSD1306& disp, RoboEyes<Adafruit_SSD1306>& e) 
  : display(disp), eyes(e) {
  currentMode = DISPLAY_EYES_ONLY;
  previousMode = DISPLAY_EYES_ONLY;
  needClear = true;
}

void DisplayManager::init() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  currentMode = DISPLAY_EYES_ONLY;
}

void DisplayManager::setMode(DisplayMode mode) {
  if(currentMode != mode) {
    previousMode = currentMode;
    currentMode = mode;
    needClear = true;
    
    // Очищаем дисплей при смене режима
    display.clearDisplay();
    display.display();
    
    // Маленькая задержка для плавности
    delay(50);
  }
}

DisplayMode DisplayManager::getMode() {
  return currentMode;
}

void DisplayManager::update() {
  if(needClear) {
    display.clearDisplay();
    needClear = false;
  }
}

void DisplayManager::clear() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  needClear = false;
}

void DisplayManager::showWelcome() {
  setMode(DISPLAY_MESSAGE_ONLY);
  clear();
  display.setCursor(15, 20);
  display.print("Pomodoro Robot");
  display.setCursor(25, 35);
  display.print("Ready!");
  display.display();
  delay(2000);
  setMode(DISPLAY_EYES_ONLY);
}

void DisplayManager::showMessage(const char* line1, const char* line2, int duration) {
  setMode(DISPLAY_MESSAGE_ONLY);
  clear();
  display.setCursor((128 - strlen(line1) * 6) / 2, 25);
  display.print(line1);
  if(strlen(line2) > 0) {
    display.setCursor((128 - strlen(line2) * 6) / 2, 40);
    display.print(line2);
  }
  display.display();
  delay(duration);
  setMode(DISPLAY_EYES_ONLY);
}

void DisplayManager::showTimer(int minutes, int seconds, bool isPaused, const char* mode) {
  if(currentMode != DISPLAY_TIMER_ONLY) {
    setMode(DISPLAY_TIMER_ONLY);
  }
  
  char timeStr[10];
  sprintf(timeStr, "%02d:%02d", minutes, seconds);
  
  clear();
  
  display.setTextSize(2);
  display.setCursor((128 - strlen(timeStr) * 12) / 2, 20);
  display.print(timeStr);
  
  if(isPaused) {
    display.setTextSize(1);
    display.setCursor(49, 45);
    display.print("PAUSED");
  }
  
  display.setTextSize(1);
  display.setCursor((128 - strlen(mode) * 6) / 2, 55);
  display.print(mode);
  
  display.display();
}

void DisplayManager::showProgress(int elapsed, int total) {
  // Только обновляем прогресс-бар, не меняя режим
  int progressWidth = map(elapsed, 0, total, 0, 124);
  display.drawRect(2, 62, 124, 2, SSD1306_WHITE);
  display.fillRect(3, 62, progressWidth, 2, SSD1306_WHITE);
  display.display();
}

void DisplayManager::showNormalMode() {
  setMode(DISPLAY_MESSAGE_ONLY);
  clear();
  display.setCursor(25, 20);
  display.print("Robot Mode");
  display.setCursor(20, 35);
  display.print("A: Start B: Settings");
  display.display();
  delay(2000);
  setMode(DISPLAY_EYES_ONLY);
}

void DisplayManager::showWorkMode(int minutes) {
  setMode(DISPLAY_MESSAGE_ONLY);
  char msg[20];
  sprintf(msg, "Work: %d min", minutes);
  clear();
  display.setCursor((128 - strlen("Work Mode Started!") * 6) / 2, 25);
  display.print("Work Mode Started!");
  display.setCursor((128 - strlen(msg) * 6) / 2, 40);
  display.print(msg);
  display.display();
  delay(1500);
  setMode(DISPLAY_TIMER_ONLY);
}

void DisplayManager::showBreakMode(int minutes) {
  setMode(DISPLAY_MESSAGE_ONLY);
  char msg[20];
  sprintf(msg, "Break: %d min", minutes);
  clear();
  display.setCursor((128 - strlen("Break Time!") * 6) / 2, 25);
  display.print("Break Time!");
  display.setCursor((128 - strlen(msg) * 6) / 2, 40);
  display.print(msg);
  display.display();
  delay(1500);
  setMode(DISPLAY_TIMER_ONLY);
}

void DisplayManager::showPauseMenu() {
  setMode(DISPLAY_MESSAGE_ONLY);
  clear();
  display.setCursor(39, 25);
  display.print("PAUSED");
  display.setCursor(24, 40);
  display.print("A: Resume B: Cancel");
  display.display();
  delay(1500);
  setMode(DISPLAY_TIMER_ONLY);
}

void DisplayManager::showComplete(int cycles) {
  setMode(DISPLAY_MESSAGE_ONLY);
  clear();
  display.setCursor(15, 25);
  display.print("Cycle Complete!");
  display.setCursor(20, 40);
  display.print("Cycles: ");
  display.print(cycles);
  display.display();
  delay(2500);
  setMode(DISPLAY_EYES_ONLY);
}

void DisplayManager::showSettingsMenu(int workMin, int breakMin, int selected) {
  setMode(DISPLAY_SETTINGS_ONLY);
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
  display.print("B:Switch A:Save");
  
  display.display();
}