#include <Arduino.h>

#include <INA219.h>
#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"

const uint8_t chipSelect = SS;
const uint32_t SAMPLE_INTERVAL_MS = 1000 * 15;
#define FILE_BASE_NAME "Data"
SdFat sd;
SdFile file;
uint32_t logTime;

INA219 monitor;

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/ U8X8_PIN_NONE);

const int switchPin =  9;
const int buttonPin = 7;
int switchState = LOW;
int buttonState = LOW;
unsigned long lastPressed = 0;
unsigned long lastDisplayUpdate = 0;

float bus_V = 0;
float mAh = 0;
float current_mA = 0;
float mWh = 0;
float power_mW = 0;
unsigned long lastread = 0; // used to calculate Ah
unsigned long tick;         // current read time - last read
int started = LOW;

#define error(msg) Serial.println(msg)

void writeHeader() {
  file.print(F("micros,V,mA,mAh,mWh"));
  file.println();
  file.sync();
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  } else {
    Serial.println("header written");
  }
}

void logData() {
  file.print(logTime);
  file.write(',');
  file.print(bus_V);
  file.write(',');
  file.print(current_mA);
  file.write(',');
  file.print(mAh);
  file.write(',');
  file.print(mWh);

  file.println();
}

void setupDisplay() {
  Serial.begin(9600);
  oled.begin();
  oled.setFlipMode(1);
  oled.setFont(u8x8_font_pressstart2p_r);
}

void setupMonitor() {
  monitor.begin();
  monitor.configure(INA219::RANGE_16V, INA219::GAIN_8_320MV, INA219::ADC_128SAMP, INA219::ADC_128SAMP, INA219::CONT_SH_BUS);
}

void loopDisplay(U8X8_SSD1306_128X64_NONAME_HW_I2C oled, INA219 monitor) {
  unsigned long newtime;
  oled.setCursor(0,0);
  oled.print(millis() / 1000);

  oled.setCursor(0,1);
  oled.print("s mV: "); oled.print(monitor.shuntVoltage() * 1000, 2);
  oled.print("   ");

  oled.setCursor(0,2);
  current_mA = monitor.shuntCurrent() * 1000;
  oled.print("s mA: "); oled.print(current_mA, 2);
  oled.print("   ");

  oled.setCursor(0,3);
  bus_V = monitor.busVoltage();
  oled.print("b V:  "); oled.print(bus_V, 2);
  oled.print("   ");

  oled.setCursor(0,4);
  power_mW = monitor.busPower() * 1000;
  oled.print("b mW: "); oled.print(power_mW, 2);
  oled.print("   ");

  newtime = millis();
  tick = newtime - lastread;
  mAh += (current_mA * tick)/3600000.0;
  mWh += (power_mW * tick)/3600000.0;
  lastread = newtime;

  oled.setCursor(0,5);
  oled.print("E mAh: "); oled.print(mAh);
  oled.print("  ");

  oled.setCursor(0,6);
  oled.print("E mWh: "); oled.print(mWh);
  oled.print("  ");

  oled.setCursor(0,7);
  oled.print("b: "); oled.print(buttonState);
  oled.print("  ");
  oled.print("s: "); oled.print(switchState);
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
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt(&oled);
  }
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
    error("file.open");
  }
}

#define BUTTON_DEBOUNCE_TIME_MS 100
#define DISPLAY_INTERVAL_MS 100

void loop() {
  unsigned long currentMillis = millis();

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    if (currentMillis - lastPressed > BUTTON_DEBOUNCE_TIME_MS) {
      switchState = !switchState;
      digitalWrite(switchPin, switchState);
    }
    if (switchState == HIGH) {
      started = HIGH;
      writeHeader();
    }
    lastPressed = millis();
  }

  if (currentMillis - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    loopDisplay(oled, monitor);
    loopMonitor(monitor);
    lastDisplayUpdate = currentMillis;
  }

  if (currentMillis - logTime >= SAMPLE_INTERVAL_MS && switchState == HIGH) {
    logTime = currentMillis;
    logData();
    file.flush();
    if (!file.sync() || file.getWriteError()) {
      error("write error");
    } else {
      Serial.println(logTime);
    }
  }

  if (switchState == LOW && started == HIGH) {
    file.close();
    Serial.println("logging turned off!");
  }

  if (bus_V < 3.0 && started == HIGH) {
    switchState = LOW;
    digitalWrite(switchPin, switchState);
    Serial.println("power turned off!");
  }
}
