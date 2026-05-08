#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

enum DisplayMode {
  DISPLAY_EYES_ONLY,      // Только глаза
  DISPLAY_TIMER_ONLY,     // Только таймер
  DISPLAY_MESSAGE_ONLY,   // Только сообщение
  DISPLAY_SETTINGS_ONLY   // Только настройки
};

class DisplayManager {
private:
  Adafruit_SSD1306& display;
  RoboEyes<Adafruit_SSD1306>& eyes;
  DisplayMode currentMode;
  
  // Для хранения предыдущего режима
  DisplayMode previousMode;
  
  // Для очистки экрана между режимами
  bool needClear;
  
public:
  DisplayManager(Adafruit_SSD1306& disp, RoboEyes<Adafruit_SSD1306>& e);
  
  void init();
  void setMode(DisplayMode mode);
  DisplayMode getMode();
  
  void showWelcome();
  void showMessage(const char* line1, const char* line2, int duration = 2000);
  void showTimer(int minutes, int seconds, bool isPaused, const char* mode);
  void showProgress(int elapsed, int total);
  void showNormalMode();
  void showWorkMode(int minutes);
  void showBreakMode(int minutes);
  void showPauseMenu();
  void showComplete(int cycles);
  void showSettingsMenu(int workMin, int breakMin, int selected);
  void clear();
  void update();  // Обновляет отображение в зависимости от режима
};

#endif