#ifndef MOTORS_H
#define MOTORS_H

#include "defines.h"
#include "imu.h"

void stop(unsigned long ms = 0) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  if (ms) delay(ms);
}

void forward(uint8_t speed = BASE_SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void backward(uint8_t speed = BASE_SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void right(uint8_t speed = BASE_SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void left(uint8_t speed = BASE_SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void drive(int16_t leftSpeed, int16_t rightSpeed, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, d(leftSpeed));
  digitalWrite(RIGHT_MOTOR_DIRECTION, d(rightSpeed));
  analogWrite(LEFT_MOTOR_PWM, s(leftSpeed));
  analogWrite(RIGHT_MOTOR_PWM, s(rightSpeed));
  if (ms) delay(ms);
}

void leftIntersection() {
  unsigned long previousMillis = millis();
  while (
    !(
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) /*&& !digitalRead(sensorPins[4])*/
      && (digitalRead(sensorPins[6]) || digitalRead(sensorPins[7]) || digitalRead(sensorPins[8]) || digitalRead(sensorPins[9]) || digitalRead(sensorPins[10])))
    || millis() - previousMillis < 500) {
    left(150);
  }
}

void rightIntersection() {
  unsigned long previousMillis = millis();
  while (
    !(
      (digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2]) || digitalRead(sensorPins[3]) || digitalRead(sensorPins[4]))
      && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])
    )|| millis() - previousMillis < 500
  ) {
    right(150);
  }
}

void left180() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 225) {
    left();
    deltaHeading = getHeading() - initialHeading;
  }
}

void deadEnd() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 225) {
    right();
    deltaHeading = getHeading() - initialHeading;
  }
  backward(120, 250);
}

void right90() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 115) {
    right();
    deltaHeading = getHeading() - initialHeading;
  }
}

void left90() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 115) {
    left();
    deltaHeading = getHeading() - initialHeading;
  }
}

void rotate(int16_t angle) {
  if(angle != 0) {
    int initialHeading = getHeading();
    int deltaHeading = getHeading() - initialHeading;
    while(abs(deltaHeading) < (angle * IMU_ERROR_FACTOR)) {
      angle > 0 ? left() : right();
      deltaHeading = getHeading() - initialHeading;
    }
  }
}

#endif