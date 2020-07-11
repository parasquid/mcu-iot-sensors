// modified from http://forum.espruino.com/comments/13071783/

function isDeviceOnBus(i2c, addr) {
  try {
    return i2c.readFrom(addr, 1);
  }
  catch (err) {
    return false;
  }
}

function i2cdetect(i2c, first, last) {
  if (typeof first === "undefined") first = 0x03;
  if (typeof (last) === "undefined") last = 0x77;
  print("    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
  for (var upper = 0; upper < 8; ++upper) {
    var line = upper + "0: ";
    for (var lower = 0; lower < 16; ++lower) {
      var address = (upper << 4) + lower;
      // Skip unwanted addresses
      if ((address < first) || (address > last)) {
        line += "   ";
        continue;
      }
      let deviceOnBus = isDeviceOnBus(i2c, address);
      if (deviceOnBus) {
        line += (address + 0x100).toString(16).substr(-2) + " ";
      } else
        line += "-- ";
    }
    print(line);
  }
}

I2C1.setup({ sda: A4, scl: A5 });
i2cdetect(I2C1, 0x03, 0x77);
