#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "imu.h"
#include "TOFs.h"
#include "motors.h"

void avoidObstacle() {
  chooseBus(0);
  uint16_t rightDistance = distanceRight();
  if (rightDistance <= 200) {
    if (DEBUG_MODE) { Serial.println("Detected a wall on the right, turning left to avoid obstacle"); }
    left90();
    forward(BASE_SPEED, 850);
    right90();
    unsigned long startMillis = millis();
    while (millis() - startMillis < 2000 && 
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])
    ) {
      forward();
    }
    if(millis() - startMillis >= 2000) {
      right90();
      startMillis = millis();
      while(millis() - startMillis < 2000 && 
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])
      ) {
        forward();
      }
      if(millis() - startMillis >= 2000) {
        right90();
        forward(BASE_SPEED, 1000);
      }
      left90();
    } else {
      forward(BASE_SPEED, 250);
      left90();
    }
  } else {
    if (DEBUG_MODE) { Serial.println("No walls detected on the right, avoiding obstacle..."); }
    right90();
    forward(BASE_SPEED, 850);
    left90();
    unsigned long startMillis = millis();
    while (millis() - startMillis < 2000 && 
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])
    ) {
      forward();
    }
    if(millis() - startMillis >= 2000) {
      left90();
      startMillis = millis();
      while(millis() - startMillis < 2000 && 
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])
      ) {
        forward();
      }
      if(millis() - startMillis >= 2000) {
        left90();
        forward(BASE_SPEED, 1000);
      }
      right90();
    } else {
      forward(BASE_SPEED, 250);
      right90();
    }
  }
}

// void obstacle() {
//   if (!digitalRead(OBSTACLE)) {
//     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
//     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
//     analogWrite(LEFT_MOTOR_PWM, 127);
//     analogWrite(RIGHT_MOTOR_PWM, 50);
//     delay(2500);
//     while (!digitalRead(sensorPins[2])) {
//       digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
//       digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
//       analogWrite(LEFT_MOTOR_PWM, 30);
//       analogWrite(RIGHT_MOTOR_PWM, 127);
//     }
//     forward();
//     delay(300);
//     while (!(!(digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2])) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]))) {
//       digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
//       analogWrite(LEFT_MOTOR_PWM, 150);
//       digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
//       analogWrite(RIGHT_MOTOR_PWM, 150);
//     }
//   }
// }

#endif