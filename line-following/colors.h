#ifndef COLORS_H
#define COLORS_H

#include "Adafruit_TCS34725.h"
#include "defines.h"
#include "motors.h"
#include "mux.h"

Adafruit_TCS34725 tcs[] = {
  Adafruit_TCS34725(),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
};

void initColorSensors() {
  for (int i = 1; i < 3; i++) {
    chooseBus(i);
    if (tcs[i].begin()) {
      if(DEBUG_MODE) {
        Serial.print("Found Color Sensor ");
        Serial.println(i == 1 ? "(Left)" : "(Right)");
      }
    } else {
      if(DEBUG_MODE) {
        Serial.print("Failed to find Color Sensor ");
        Serial.println(i == 1 ? "(Left)" : "(Right)");
        while (true); 
      }
      turnOnLED(COLOR_ERROR_LED);
    }
  }
}

uint8_t readColors(uint8_t sensorNum) {
  chooseBus(sensorNum);
  float r, g, b;
  tcs[sensorNum].getRGB(&r, &g, &b);
  if(DEBUG_MODE) {
    // Serial.print(sensorNum);
    // Serial.print(": ");
    // Serial.print(" R: ");
    // Serial.print(r);
    // Serial.print("\t");
    // Serial.print("G: ");
    // Serial.print(g);
    // Serial.print("\t");
    // Serial.print("B: ");
    // Serial.print(b);
    // Serial.print("\t");
  }
  if (g - r > GREEN_THRESHOLD && g - b > GREEN_THRESHOLD) {
    return GREEN;
  } else if (r - g > RED_THRESHOLD && r - b > RED_THRESHOLD) {
    return RED;
  } else {
    return NO_COLOR;
  }
}

void checkRed() {
  // if(!(digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2]) || digitalRead(sensorPins[3]) || digitalRead(sensorPins[4]) || digitalRead(sensorPins[5]) || digitalRead(sensorPins[6]) || digitalRead(sensorPins[7]) || digitalRead(sensorPins[8]) || digitalRead(sensorPins[9]) || digitalRead(sensorPins[10]))) {
    uint8_t leftColor = readColors(LEFT_COLOR);
    uint8_t rightColor = readColors(RIGHT_COLOR);
    if(leftColor == RED || rightColor == RED) {
      stop(6000);
    }
  // }
}

char checkGreen() {
  uint8_t SensorOneReading = readColors(LEFT_COLOR);
  uint8_t SensorTwoReading = readColors(RIGHT_COLOR);
  if (SensorOneReading == GREEN && SensorTwoReading == GREEN) {
    return 'G';
  } else if (SensorOneReading == GREEN && SensorTwoReading != GREEN) {
    return 'F';
  } else if (SensorOneReading != GREEN && SensorTwoReading == GREEN) {
    return 'H';
  } else {
    return 'N';
  }
}

unsigned long previousMillis = 0;

void handleGreen() {
  stop();
  bool leftGreen = false;
  bool rightGreen = false;
  previousMillis = millis();
  while (millis() - previousMillis < 500) {
    char result = checkGreen();
    if (result == 71) {
      leftGreen = true;
      rightGreen = true;
      deadEnd();
      break;
    } else if (result == 70) {
      leftGreen = true;
    } else if (result == 72) {
      rightGreen = true;
    }
  }
  if (leftGreen && !rightGreen) {
    leftIntersection();
    forward(BASE_SPEED, 200);
  } else if (!leftGreen && rightGreen) {
    rightIntersection();
    forward(BASE_SPEED, 200);
  } else if (!leftGreen && !rightGreen) {
    forward(BASE_SPEED, 100);
  }
}

#endif