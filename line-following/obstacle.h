#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "imu.h"
#include "TOFs.h"
#include "motors.h"

void avoidObstacle() {
  uint16_t rightDistance = distanceRight();
  if (rightDistance <= 300) {
    if (DEBUG_MODE) { Serial.println("Detected a wall on the right, turning left to avoid obstacle"); }
    left90();
    forward(BASE_SPEED, 500);
    right90();
    forward(BASE_SPEED, 2000);
    right90();
    forward(BASE_SPEED, 500);
    left90();
  } else {
    if (DEBUG_MODE) { Serial.println("No walls detected on the right, avoiding obstacle...") }
    right90();
    forward(BASE_SPEED, 500);
    left90();
    forward(BASE_SPEED, 2000);
    left90();
    forward(BASE_SPEED, 500);
    right90();
  }
  stop();
}

// void obstacle() {
//   if (!digitalRead(OBSTACLE)) {
//     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
//     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
//     analogWrite(LEFT_MOTOR_PWM, 127);
//     analogWrite(RIGHT_MOTOR_PWM, 50);
//     delay(2000);
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