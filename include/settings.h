#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"
#include "display.h"

class SettingsManager {
private:
  RoboEyes<Adafruit_SSD1306>& eyes;
  DisplayManager& display;
  
  int workDuration;
  int breakDuration;
  int tempWork;
  int tempBreak;
  int selectedOption;
  bool inSettings;
  
public:
  SettingsManager(RoboEyes<Adafruit_SSD1306>& e, DisplayManager& d);
  
  void load();
  void save();
  void enter();
  void exit();
  void nextOption();
  void adjustWork(int delta);
  void adjustBreak(int delta);
  void update();
  
  int getWorkDuration();
  int getBreakDuration();
  
private:
  void showMenu();
  void saveToEEPROM();
  void loadFromEEPROM();
};

#endif