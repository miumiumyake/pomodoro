#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FluxGarage_RoboEyes.h"
#include "buttons.h"
#include "pomodoro.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RoboEyes<Adafruit_SSD1306> eyes(display);
ButtonManager buttons;
PomodoroTimer timer(eyes, display);

void setup() {
    Serial.begin(115200);
    
    Wire.begin(4, 5);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        for(;;);
    }
    
    buttons.init();
    
    eyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 30);
    eyes.setDisplayColors(0, 1);
    eyes.setWidth(36, 36);
    eyes.setHeight(36, 36);
    eyes.setBorderradius(8, 8);
    eyes.setSpacebetween(10);
    eyes.setIdleMode(true, 4, 2);
    eyes.setAutoblinker(true, 2, 3);
    eyes.setCuriosity(true);
    eyes.setPosition(ROBODEFAULT);
    eyes.setMood(ROBODEFAULT);
    eyes.open();
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(15, 20);
    display.print("Pomodoro Robot");
    display.setCursor(10, 40);
    display.print("A:Start B:Emotion");
    display.display();
    delay(3000);
    
    display.clearDisplay();
    display.display();
}

void loop() {
    ButtonAction action = buttons.update();
    
    if(action == BUTTON_A_PRESS) {
        timer.press();
    } else if(action == BUTTON_B_PRESS) {
        timer.pressB();
    }
    
    timer.update();
    
    if(timer.getState() == NORMAL_MODE) {
        eyes.update();
    }
    
    delay(20);
}