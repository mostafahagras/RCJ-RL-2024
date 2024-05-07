#ifndef MUX_H
#define MUX_H

#include <Wire.h>

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

#endif