#include <SoftSerial_INT0.h>
#include "DigiKeyboard.h"

#define P_RX 2 // Reception PIN (SoftSerial)
#define P_TX 0 // Transmition PIN (SoftSerial)
SoftSerial btSerial(P_RX, P_TX);

#define ledPin 1
int state = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  btSerial.begin(9600);
}

void loop()
{
  if (btSerial.available() > 0)
  {
    state = btSerial.read();
  }

  // the led is only used for testing in case the keyboard doesn't work;
  // it assumes that there's an LED on ledPin
  if (state == '0')
  {
    digitalWrite(ledPin, LOW);
    btSerial.println("OFF");
    DigiKeyboard.sendKeyStroke(0x4b); // Keyboard PageUp
    state = 0;
  }
  else if (state == '1')
  {
    digitalWrite(ledPin, HIGH);
    btSerial.println("ON");
    DigiKeyboard.sendKeyStroke(0x4e); // Keyboard PageDown
    state = 0;
  }

  // Update the USB connection (maintain alive the connection)
  DigiKeyboard.update();

  // also see https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
}
