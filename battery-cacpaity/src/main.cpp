#include <Arduino.h>

#include <INA219.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"

void setupDisplay();

const uint8_t chipSelect = SS;
const uint32_t SAMPLE_INTERVAL_MS = 1000;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;
uint32_t logTime;

INA219 monitor;

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/ U8X8_PIN_NONE);

const int transistorPin =  9;
const int buttonPin = 7;
int transistor = LOW;
int buttonState = LOW;
uint32_t lastPressed = 0;

void setup()
{
  setupDisplay();
  monitor.begin();
  pinMode(transistorPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void setupDisplay() {
  Serial.begin(9600);
  oled.begin();
  oled.setFlipMode(1);
  oled.setFont(u8x8_font_chroma48medium8_r);
}

void loopDisplay(U8X8_SSD1306_128X64_NONAME_HW_I2C oled, INA219 monitor) {
  oled.setCursor(0,0);
  oled.print(millis());

  oled.setCursor(0,1);
  oled.print("sVolts:  ");
  oled.setCursor(8, 1); oled.print(monitor.shuntVoltage() * 1000, 2);
  oled.setCursor(13, 1); oled.print("  V");

  oled.setCursor(0,2);
  oled.print("sAmps:  ");
  oled.setCursor(8, 2); oled.print(monitor.shuntCurrent() * 1000, 2);
  oled.setCursor(13, 2); oled.print(" mA");

  oled.setCursor(0,3);
  oled.print("bVolts:  ");
  oled.setCursor(8, 3); oled.print(monitor.busVoltage(), 2);
  oled.setCursor(13, 3); oled.print("  V");

  oled.setCursor(0,4);
  oled.print("bWatts: ");
  oled.setCursor(8, 4); oled.print(monitor.busPower() * 1000, 2);
  oled.setCursor(13, 4); oled.print(" mW");

  oled.setCursor(0,5);
  oled.print(buttonState);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    if (millis() - lastPressed > 100) {
      transistor = !transistor;
      digitalWrite(transistorPin, transistor);
    }
    lastPressed = millis();
  }

  loopDisplay(oled, monitor);
}
