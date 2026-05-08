#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"

#include "display.h"
#include "buttons.h"
#include "pomodoro.h"
#include "eyes_animations.h"
#include "settings.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RoboEyes<Adafruit_SSD1306> eyes(display);

DisplayManager displayManager(display, eyes);
ButtonManager buttonManager;
PomodoroTimer pomodoroTimer(eyes, displayManager);
EyeAnimator eyeAnimator(eyes);
SettingsManager settings(eyes, displayManager);

unsigned long lastButtonProcessTime = 0;
const unsigned long BUTTON_PROCESS_INTERVAL = 600;

void handleButtonAction(ButtonAction action);
void handleButtonA();
void handleButtonB();
void handleButtonALong();
void handleButtonBLong();

void setup() {
    Serial.begin(115200);
    Serial.println("\nPomodoro Robot Starting...");
    
    Wire.begin(4, 5);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    buttonManager.init();
    displayManager.init();
    eyeAnimator.init();
    settings.load();
    
    displayManager.showWelcome();
    eyes.open();
    
    Serial.println("Ready!");
}

void loop() {
    ButtonAction action = buttonManager.update();
    
    if(action != NO_ACTION) {
        if(millis() - lastButtonProcessTime > BUTTON_PROCESS_INTERVAL) {
            eyes.blink();
            delay(100);
            handleButtonAction(action);
            lastButtonProcessTime = millis();
        }
    }
    
    pomodoroTimer.update();
    
    // Обновляем глаза ТОЛЬКО если дисплей в режиме глаз
    if(displayManager.getMode() == DISPLAY_EYES_ONLY) {
      eyeAnimator.update();
      eyes.update();
    } else {
      // В других режимах глаза не обновляем (они не видны)
      // Но нужно периодически вызывать update для поддержки анимаций
      static unsigned long lastMinimalUpdate = 0;
      if(millis() - lastMinimalUpdate > 500) {
        eyes.update();  // Минимальное обновление
        lastMinimalUpdate = millis();
      }
    }
    
    delay(50);
}

void handleButtonAction(ButtonAction action) {
    switch(action) {
        case BUTTON_A_PRESS:
            handleButtonA();
            break;
        case BUTTON_B_PRESS:
            handleButtonB();
            break;
        case BUTTON_A_LONG:
            handleButtonALong();
            break;
        case BUTTON_B_LONG:
            handleButtonBLong();
            break;
        default:
            break;
    }
}

void handleButtonA() {
    PomodoroState state = pomodoroTimer.getState();
    
    switch(state) {
        case NORMAL_MODE:
            pomodoroTimer.start();
            break;
        case WORK_MODE:
        case BREAK_MODE:
            if(pomodoroTimer.isRunning()) {
                pomodoroTimer.pause();
            } else if(pomodoroTimer.isPaused()) {
                pomodoroTimer.resume();
            }
            break;
        case COMPLETED_MODE:
            pomodoroTimer.reset();
            break;
        case SETTINGS_MODE:
            settings.save();
            pomodoroTimer.reset();
            break;
    }
}

void handleButtonB() {
    PomodoroState state = pomodoroTimer.getState();
    
    if(state == NORMAL_MODE) {
        settings.enter();
    } else if(state == WORK_MODE || state == BREAK_MODE) {
        pomodoroTimer.reset();
    } else if(state == COMPLETED_MODE) {
        pomodoroTimer.start();
    } else if(state == SETTINGS_MODE) {
        settings.nextOption();
    }
}

void handleButtonALong() {
    if(pomodoroTimer.getState() != NORMAL_MODE) {
        pomodoroTimer.reset();
        displayManager.showMessage("Reset", "Back to normal");
    }
}

void handleButtonBLong() {
    pomodoroTimer.showStatus();
}