#include <Stream.h>
#include "hpm.h"

HoneywellHPM::HoneywellHPM (Stream* sensor_param) {
  sensor = sensor_param;
};

int HoneywellHPM::available() {
  return sensor->available();
}

int HoneywellHPM::read() {
  return sensor->read();
}
