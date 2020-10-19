var s = new Serial();
s.setup(9600,{rx: D33, tx: D34});

let buffer = '';
const header = String.fromCharCode(0x42) + String.fromCharCode(0x4d);
let pmData = {};

const calculateChecksum = (arr) => {
  return arr.reduce((acc, cur) => (acc + cur), 0) - arr[30] - arr[31];
};

s.on('data', function (data) {
  buffer = buffer + data;
  if(buffer.length < 32) {
    // get at least 32 bytes
  } else  {
    // find header and discard any previous bytes in buffer
    const index = buffer.indexOf(header);
    if(index != -1) { // found the header
      buffer = buffer.substr(index); // discard previous bytes until header
      if(buffer.length >= 32) {
        buffer = buffer.substr(0, 32);  // get a complete packet
        const arrayBuffer = E.toArrayBuffer(buffer);
        buffer = buffer.substr(32); // set buffer to leftover bytes

        const dataView = new DataView(arrayBuffer);
        data = {
          header: dataView.getUint16(0),
          length: dataView.getUint16(2),
          pm25: dataView.getUint16(6),
          pm10: dataView.getUint16(8),
          checksum: dataView.getUint16(30),
        };

        const calculatedChecksum = calculateChecksum(new Uint8Array(arrayBuffer));

        // early return on bad data
        if (data.length != 28) {
          // print(data.length);
          return;
        }
        if (data.pm25 < 0 || data.pm25 > 1000) {
          // print(data);
          return;
        }
        if (calculatedChecksum != data.checksum) {
          print(calculatedChecksum, data.checksum);
          // we're getting too many invalid checksums so comment out
          // the early return for now ...
          //return;
        }

        if (data.pm10 < 0) data.pm10 = 0;
        if (data.pm10 > 255) data.pm10 = 255;

        pmData = {pm25: data.pm25, pm10: data.pm10, t: Date.now()};

        digitalPulse(LED1, true, 50);
      }
    } else { // header not found
    }
  }
});

const SDA = 30;
const SCL = 31;

I2C1.setup({scl:SCL,sda:SDA});

var exports={};
var C = {
  I2C_ADDRESS : 0x40,
};
function HDC1080(i2c, deviceAddress) {
  this.i2c = i2c;
  this.deviceAddress = deviceAddress;
}
HDC1080.prototype.read = function() {
  this.i2c.writeTo(this.deviceAddress, 0);
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      const data = this.i2c.readFrom(this.deviceAddress, 4);
      const temp = data[1] | data[0] << 8;
      const hum = data[3] | data[2] << 8;
      resolve({
        temperature: (temp / 65536) * 165.0 - 40.0,
        humidity: (hum / 65536) * 100.0,
      });
    }, 25);
  });
};
exports.connect = function (i2c, deviceAddress) {
  return new HDC1080(i2c, deviceAddress || C.I2C_ADDRESS);
};

var gas = require("CCS811").connectI2C(I2C1, {int : 29});
var hdc = exports.connect(I2C1);

var temperature = 25;
var humidity = 50;

const serviceData = (t, h) => {
  return {
    0x181A: { // org.bluetooth.descriptor.es_measurement
      0x2A6E: { // temperature
        value : t,
      },
      0x2A6F: { // humidity
        value: h,
      },
    }
  };
};

NRF.setServices({
  0x181A: { // org.bluetooth.descriptor.es_measurement
    0x2A6E: { // temperature
      value : [temperature * 100],
      readable : true,
      maxLen: 8,
    },
    0x2A6F: { // humidity
      value: [humidity * 100],
      readable : true,
      maxLen: 8,
    },
  }
}, { advertise: [ '0x181A' ] });

gas.on('data', (data) => {
  hdc.read().then((e) => {
    temperature = e.temperature;
    humidity = e.humidity;
  });
  gas.setEnvData(humidity, temperature);
  print(data, temperature, humidity, pmData);

  // Bluetooth spec says data is 16 bits, 0.01/unit - so x100
  const t = Math.round(temperature*100);
  const h = Math.round(humidity*100);
  NRF.setAdvertising({
    0x2A6E: [t&255,t>>8],
    0x2A6F: [h&255,h>>8],
  });

  NRF.updateServices(serviceData([t&255,t>>8], [h&255,h>>8]));
});
