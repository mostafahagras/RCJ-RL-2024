#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define NONE 0
#define GREEN 1
#define RED 2
#define LEFT_GREEN 70   // F
#define BOTH_GREEN 71   // G
#define RIGHT_GREEN 72  // H
#define LEFT_RED 81     // Q
#define BOTH_RED 82     // R
#define RIGHT_RED 83    // S
#define BOTH_NONE 78    // N

Adafruit_TCS34725 tcs[] = {
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  initColorSensors();
}

void loop() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
  // Serial.println();
  if (SensorOneReading == RED && SensorTwoReading == RED) {
    Serial.write(BOTH_RED);
  } else if (SensorOneReading == GREEN && SensorTwoReading == GREEN) {
    Serial.write(BOTH_GREEN);
  } else if (SensorOneReading == GREEN && SensorTwoReading == NONE) {
    Serial.write(LEFT_GREEN);
  } else if (SensorOneReading == NONE && SensorTwoReading == GREEN) {
    Serial.write(RIGHT_GREEN);
  } else if (SensorOneReading == RED && SensorTwoReading == NONE) {
    Serial.write(LEFT_RED);
  } else if (SensorOneReading == NONE && SensorTwoReading == RED) {
    Serial.write(RIGHT_RED);
  } else {
    Serial.write(BOTH_NONE);
  }
}

void initColorSensors() {
  for (int i = 0; i < 2; i++) {
    // Serial.println(i);
    chooseBus(i);
    if (tcs[i].begin()) {
      // Serial.print("Found sensor ");
      // Serial.println(i + 1);
    } else {
      // Serial.println("No Sensor Found");
      while (true)
        ;
    }
  }
}

int readColors(byte sensorNum) {
  chooseBus(sensorNum);
  float r, g, b;
  tcs[sensorNum].getRGB(&r, &g, &b);
  // Serial.print(sensorNum);
  // Serial.print(": ");
  // Serial.print(r);
  // Serial.print("\t");
  // Serial.print(g);
  // Serial.print("\t");
  // Serial.print(b);
  // Serial.print("\t");
  // Serial.println();
  if (g - r > 25 && g - b > 25 ) {
    return GREEN;
  } else if (r - g > 40 && r - b > 40) {
    return RED;
  } else {
    return NONE;
  }
}

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}
