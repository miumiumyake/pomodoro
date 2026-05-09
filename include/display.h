#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

enum DisplayMode {
  DISPLAY_EYES,
  DISPLAY_TIMER,
  DISPLAY_MESSAGE,
  DISPLAY_SETTINGS
};

class DisplayManager {
private:
  Adafruit_SSD1306& display;
  RoboEyes<Adafruit_SSD1306>& eyes;
  DisplayMode currentMode;
  bool needClear;
  
public:
  DisplayManager(Adafruit_SSD1306& disp, RoboEyes<Adafruit_SSD1306>& e);
  
  void init();
  void setMode(DisplayMode mode);
  DisplayMode getMode();
  void clear();
  
  void showMessage(const char* line1, const char* line2, int duration = 2000);
  void showTimer(int minutes, int seconds, bool isPaused, const char* mode);
  void showProgress(int elapsed, int total);
  void showNormalModeMessage();
  void showWorkModeMessage(int minutes);
  void showBreakModeMessage(int minutes);
  void showPauseMessage();
  void showCompleteMessage(int cycles);
  void showSettingsMenu(int workMin, int breakMin, int selected);
  void showEyes();
};

#endif