#include <Arduino.h>

#include <INA219.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"

const uint8_t chipSelect = SS;
const uint32_t SAMPLE_INTERVAL_MS = 1000;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;
uint32_t logTime;

INA219 monitor;
#define R_SHUNT 0.00375
#define V_SHUNT_MAX 0.075
#define V_BUS_MAX 16
#define I_MAX_EXPECTED 20

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/ U8X8_PIN_NONE);

const int switchPin =  9;
const int buttonPin = 7;

int swithState = LOW;
int buttonState = LOW;
uint32_t lastPressed = 0;

void setupDisplay() {
  Serial.begin(9600);
  oled.begin();
  oled.setFlipMode(1);
  oled.setFont(u8x8_font_pressstart2p_r);
}

void setupMonitor() {
  monitor.begin();
  monitor.configure(INA219::RANGE_16V, INA219::GAIN_2_80MV, INA219::ADC_16SAMP, INA219::ADC_16SAMP, INA219::CONT_SH_BUS);
  monitor.calibrate(R_SHUNT, V_SHUNT_MAX, V_BUS_MAX, I_MAX_EXPECTED);
}

void loopDisplay(U8X8_SSD1306_128X64_NONAME_HW_I2C oled, INA219 monitor) {
  oled.setCursor(0,0);
  oled.print(millis());

  oled.setCursor(0,1);
  oled.print("sVolts:  ");
  oled.setCursor(8, 1); oled.print(monitor.shuntVoltage() * 1000, 2);
  oled.setCursor(14, 1); oled.print(" V");

  oled.setCursor(0,2);
  oled.print("sAmps:  ");
  oled.setCursor(8, 2); oled.print(monitor.shuntCurrent() * 1000, 2);
  oled.setCursor(14, 2); oled.print("mA");

  oled.setCursor(0,3);
  oled.print("bVolts:  ");
  oled.setCursor(8, 3); oled.print(monitor.busVoltage(), 2);
  oled.setCursor(14, 3); oled.print(" V");

  oled.setCursor(0,4);
  oled.print("bWatts: ");
  oled.setCursor(8, 4); oled.print(monitor.busPower() * 1000, 2);
  oled.setCursor(14, 4); oled.print("mW");

  oled.setCursor(0,5);
  oled.print("sum mAh: ");

  oled.setCursor(0,6);
  oled.print("button: "); oled.print(buttonState);

  oled.setCursor(0,7);
  oled.print("switch: "); oled.print(swithState);
}

void loopMonitor(INA219 monitor) {
  monitor.recalibrate();
  monitor.reconfig();
}

void setup() {
  setupDisplay();
  setupMonitor();
  pinMode(switchPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

#define DEBOUNCE_TIME_MS 100
void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    if (millis() - lastPressed > DEBOUNCE_TIME_MS) {
      swithState = !swithState;
      digitalWrite(switchPin, swithState);
    }
    lastPressed = millis();
  }

  loopDisplay(oled, monitor);
  loopMonitor(monitor);
}