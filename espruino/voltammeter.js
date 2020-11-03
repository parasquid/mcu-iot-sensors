const P1_01 = 33;
const P1_02 = 34;
const P1_12 = 44;
const P1_13 = 45;
const P1_14 = 46;
const A = {
  MOSI: 11, MISO: 12, SCK: 13, // SPI
  SDA: 28, SCL: 29, // I2C
  A0: 30, A1: 31, A2: 2, A3: 3, A4: 28, A5: 29, // ADC
  D0: P1_01,
  D1: P1_02,
  D2: P1_12,
  D3: P1_13,
  D4: P1_14,
  D5: 5,
  D6: 6,
  D7: 7,
  D8: 8,
  D9: 21,
  D10: 17,
  D11: 22,
  D12: 19,
  D13: 20,
};
const i2cOpts = { sda: A.SDA, scl: A.SCL, bitrate: 400000 };
//const i2c = new I2C();
const i2c = I2C1;
i2c.setup(i2cOpts);

let ina219;
let oled;
let refreshRate = 100; // in ms

const options = {
  maximumExpectedCurrent: 3.2768 / 8, // in amps
  rShunt: 1.7, // in ohms
};

const measure = (sensor) => {
  return {
    v: sensor.getBusMilliVolts() / 1000,
    mA: sensor.getBusMicroAmps() * 1000,
    mW: sensor.getBusMicroWatts() * 1000,
  };
};

const padRight = (input, places) => {
  let out = input + "";
  out = out + ("0").repeat((places - out.length) > 0 ? (places - out.length) : 0);
  return out;
};

const display = (out, data) => {
  oled.clear();
  oled.drawString(padRight(data.v, 6) + " V", 0, 0);
  oled.drawString(padRight(data.mA, 6) + " mA", 0, 11);
  oled.drawString(padRight(data.mW, 6) + " mW", 0, 22);
  oled.flip();
};

E.on('init', () => {
  ina219 = require("INA219").connect(i2c, options);
  console.log(ina219.initDevice());

  setTimeout(() => {
    oled = require("SSD1306").connect(i2c, () => {
      oled.clear();
      oled.setFontVector(10);
    }, { height : 32 });
  }, 2000);

  setInterval(() => {
    data = measure(ina219);
    print(data);

    digitalPulse(LED1, true, 5);

    if(oled) display(oled, data);
  }, refreshRate);
});
