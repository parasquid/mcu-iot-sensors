#include <Arduino.h>
#include <SoftwareSerial.h>
#include <hpm.h>

SoftwareSerial sensor(D4, D0);
HoneywellHPM hpm(&sensor);

void setup() {
  Serial.begin(9600);
  Serial.println("Software Serial test");
  sensor.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  while (hpm.available() > 0) {
		Serial.write(hpm.read());
		yield();
	}
}
