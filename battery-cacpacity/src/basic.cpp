#include <Arduino.h>

#include <INA.h>
#include <U8x8lib.h>
#include <Wire.h>

INA_Class monitor;

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/ U8X8_PIN_NONE);

unsigned long lastDisplayUpdate = 0;

float bus_V = 0;
float mAh = 0;
float current_mA = 0;
float mWh = 0;
float power_mW = 0;
unsigned long lastread = 0; // used to calculate Ah
unsigned long tick;         // current read time - last read

#define error(msg) Serial.println(msg)

void setupDisplay() {
  Serial.begin(9600);
  oled.begin();
  oled.setFlipMode(1);
  oled.setFont(u8x8_font_pressstart2p_r);
}

void setupMonitor() {
  monitor.begin(1,100000);
  monitor.setBusConversion(8500);            // Maximum conversion time 8.244ms
  monitor.setShuntConversion(8500);          // Maximum conversion time 8.244ms
  monitor.setAveraging(128);                 // Average each reading n-times
  monitor.setMode(INA_MODE_CONTINUOUS_BOTH); // Bus/shunt measured continuously
}

void loopDisplay() {
  unsigned long newtime;
  oled.setCursor(0,0);
  oled.print(millis() / 1000);

  oled.setCursor(0,1);
  oled.print("s mV: "); oled.print(monitor.getShuntMicroVolts() / 1000.0, 2);
  oled.print("   ");

  oled.setCursor(0,2);
  current_mA = monitor.getBusMicroAmps() / 1000.0;
  oled.print("s mA: "); oled.print(current_mA, 2);
  oled.print("   ");

  oled.setCursor(0,3);
  bus_V = monitor.getBusMilliVolts() / 1000.0;
  oled.print("b V:  "); oled.print(bus_V, 2);
  oled.print("   ");

  oled.setCursor(0,4);
  power_mW = monitor.getBusMicroWatts() / 1000.0;
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
}

void setup() {
  setupDisplay();
  setupMonitor();
}

#define DISPLAY_INTERVAL_MS 100

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastDisplayUpdate >= DISPLAY_INTERVAL_MS) {
    loopDisplay();
    lastDisplayUpdate = currentMillis;
  }
}
