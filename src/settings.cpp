#include "settings.h"
#include <EEPROM.h>

#define EEPROM_SIZE 8
#define EEPROM_WORK_ADDR 0
#define EEPROM_BREAK_ADDR 4

SettingsManager::SettingsManager(RoboEyes<Adafruit_SSD1306>& e, DisplayManager& d) 
  : eyes(e), display(d) {
  workDuration = 25;
  breakDuration = 5;
  tempWork = workDuration;
  tempBreak = breakDuration;
  selectedOption = 0;
  inSettings = false;
}

void SettingsManager::load() {
  EEPROM.begin(EEPROM_SIZE);
  loadFromEEPROM();
}

void SettingsManager::save() {
  workDuration = tempWork;
  breakDuration = tempBreak;
  saveToEEPROM();
  inSettings = false;
  eyes.setMood(ROBODEFAULT);
  display.showMessage("Settings Saved!", "", 1000);
}

void SettingsManager::enter() {
  inSettings = true;
  tempWork = workDuration;
  tempBreak = breakDuration;
  selectedOption = 0;
  eyes.setIdleMode(false);
  eyes.setAutoblinker(false);
  display.showSettingsMenu(tempWork, tempBreak, selectedOption);
}

void SettingsManager::exit() {
  inSettings = false;
  eyes.setIdleMode(true, 3, 2);
  eyes.setAutoblinker(true, 2, 3);
}

void SettingsManager::nextOption() {
  if(inSettings) {
    selectedOption = !selectedOption;
    eyes.blink();
    display.showSettingsMenu(tempWork, tempBreak, selectedOption);
  }
}

void SettingsManager::adjustWork(int delta) {
  tempWork += delta;
  if(tempWork < 5) tempWork = 5;
  if(tempWork > 60) tempWork = 60;
  display.showSettingsMenu(tempWork, tempBreak, selectedOption);
}

void SettingsManager::adjustBreak(int delta) {
  tempBreak += delta;
  if(tempBreak < 1) tempBreak = 1;
  if(tempBreak > 30) tempBreak = 30;
  display.showSettingsMenu(tempWork, tempBreak, selectedOption);
}

void SettingsManager::update() {
  // Для будущих обновлений
}

int SettingsManager::getWorkDuration() {
  return workDuration;
}

int SettingsManager::getBreakDuration() {
  return breakDuration;
}

void SettingsManager::showMenu() {
  display.showSettingsMenu(tempWork, tempBreak, selectedOption);
}

void SettingsManager::saveToEEPROM() {
  EEPROM.put(EEPROM_WORK_ADDR, workDuration);
  EEPROM.put(EEPROM_BREAK_ADDR, breakDuration);
  EEPROM.commit();
}

void SettingsManager::loadFromEEPROM() {
  int tempWorkVal, tempBreakVal;
  EEPROM.get(EEPROM_WORK_ADDR, tempWorkVal);
  EEPROM.get(EEPROM_BREAK_ADDR, tempBreakVal);
  
  if(tempWorkVal >= 5 && tempWorkVal <= 60) workDuration = tempWorkVal;
  if(tempBreakVal >= 1 && tempBreakVal <= 30) breakDuration = tempBreakVal;
}