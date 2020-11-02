const neopixel = require("neopixel");
const numPixels = 7;
const pixelPin = 29;

const filledArr = (val, length) => {
  const arr = new Uint8ClampedArray(length);
  return arr.fill(val);
};
const emptyArr = (length) => {
  return filledArr(0, length);
};

// fill all pixels with this color
const paint = (color) => {
  const arr = new Uint8Array(numPixels * 3);
  for (let i=0; i < numPixels * 3; i += 3) {
    arr[i]     = color[1]; // g
    arr[i + 1] = color[0]; // r
    arr[i + 2] = color[2]; // b
  }
  return arr;
};

E.on('init', function() {
  NRF.setConnectionInterval(7.5);
  NRF.setTxPower(8);

  NRF.setServices({
    "0x181C": {
      "0x2A3D": {
        value: [0,0,0],
        writable : true,
        description: "sets the value of a pixel: [r, g, b]",
        onWrite : function(e) {
          const data = new Uint8Array(e.data);
          neopixel.write(pixelPin, paint(data));
        }
      }
    }
  }, { advertise: ['181C'] });

  NRF.on('disconnect', function() {
    print("disconnected!");
    neopixel.write(pixelPin, emptyArr(numPixels * 3));
  });

  NRF.on('connect', function() {
    print("connected!");
    neopixel.write(pixelPin, filledArr(64, numPixels * 3));
  });
});
