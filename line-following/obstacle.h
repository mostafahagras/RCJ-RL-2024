#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "imu.h"
#include "TOFs.h"
#include "motors.h"

void avoidObstacle() {
  stop();
  if(abs(getPitch()) > 15) return; // To stop seeing obstacle after seesaw
  chooseBus(0);
  uint16_t rightDistance = distanceRight();
  // if(rightDistance <= 200) {
  //   left90();
  //   drive(254, 40);
  //   while(_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + _IR6() + _IR7() + _IR8() + _IR8() + _IR9() + _IR10() < 3) {}
  //   forward(BASE_SPEED, 250);
  //   left90();
  //   backward(BASE_SPEED, 200);
  // } else {
  //   right90();
  //   drive(40, 254);
  //   while(_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + _IR6() + _IR7() + _IR8() + _IR8() + _IR9() + _IR10() < 3) {}
  //   forward(BASE_SPEED, 250);
  //   right90();
  //   backward(BASE_SPEED, 200);
  // }
  if (rightDistance <= 200) {
    if (DEBUG_MODE) { Serial.println("Detected a wall on the right, turning left to avoid obstacle"); }
    left90();
    forward(120, 850);
    right90();
    unsigned long startMillis = millis();
    while (millis() - startMillis < 1800 && !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
      forward();
    }
    if (millis() - startMillis >= 1800) {
      right90();
      startMillis = millis();
      while (millis() - startMillis < 1800 && !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
        forward();
      }
      if (millis() - startMillis >= 1800) {
        right90();
        while (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
          forward();
        }
        forward(120, 250);
      }
      left90();
    } else {
      forward(120, 250);
      left90();
    }
  } else {
    if (DEBUG_MODE) { Serial.println("No walls detected on the right, avoiding obstacle..."); }
    right90();
    forward(120, 850);
    left90();
    unsigned long startMillis = millis();
    while (millis() - startMillis < 1800 && !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
      forward();
    }
    if (millis() - startMillis >= 1800) {
      left90();
      startMillis = millis();
      while (millis() - startMillis < 1800 && !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
        forward();
      }
      if (millis() - startMillis >= 1800) {
        left90();
        while (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
          forward();
        }
        forward(120, 250);
      }
      right90();
    } else {
      forward(120, 250);
      right90();
    }
  }
}

#endif