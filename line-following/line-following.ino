#include <Wire.h>
#include "defines.h"
#include "motors.h"
#include "colors.h"
#include "camera.h"
#include "imu.h"
#include "TOFs.h"
#include "obstacle.h"

bool shouldCheckRed = false;
long whiteCounter = 0;

void setup() {
  if (DEBUG_MODE) {
    Serial.begin(115200);
    Serial.println("Begin setup");
  }
  /* Line following sensors */
  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  pinMode(IR6, INPUT);
  pinMode(IR7, INPUT);
  pinMode(IR8, INPUT);
  pinMode(IR9, INPUT);
  pinMode(IR10, INPUT);
  pinMode(OBSTACLE, INPUT);
  pinMode(CALIBRATION_PIN, INPUT_PULLUP);
  pinMode(COLOR_ERROR_LED, OUTPUT);
  pinMode(TOF_ERROR_LED, OUTPUT);
  pinMode(IMU_ERROR_LED, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Wire.begin();
  initIMU();
  chooseBus(0);
  pinMode(RIGHT_TOF_XSHUT, OUTPUT);
  pinMode(FRONT_TOF_XSHUT, OUTPUT);
  initTOFs();
  initServos();
  initColorSensors();
  Serial3.begin(9600);
  if (!digitalRead(CALIBRATION_PIN)) {
    pinMode(THRESHOLD_INCREMENT_INTERRUPT, INPUT_PULLUP);
    pinMode(THRESHOLD_DECREMENT_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(THRESHOLD_INCREMENT_INTERRUPT), INCREMENT_THRESHOLD, FALLING);
    attachInterrupt(digitalPinToInterrupt(THRESHOLD_DECREMENT_INTERRUPT), DECREMENT_THRESHOLD, FALLING);
  }
  while (!digitalRead(CALIBRATION_PIN)) {
    Serial.println(BLACK_THRESHOLD);
    if (reading(sensorPins[5])) {
      turnOnLED(CENTER_IR_LED);
    } else {
      turnOffLED(CENTER_IR_LED);
    }
    if (!digitalRead(2) && !digitalRead(3) && !digitalRead(4)) {
      turnOnLED(SILVER_TAPE_LED);
    } else {
      turnOffLED(SILVER_TAPE_LED);
    }
  }
  turnOffLED(CENTER_IR_LED);
  turnOffLED(SILVER_TAPE_LED);
  chooseBus(0);
}

void loop() {
  if (inEvacuation) {
    if (wallFollowing) {
      handleEvacuation(WALL_FOLLOW);
    } else {
      Serial.print("State: ");
      Serial.print(state);
      cmessage = readSerial();
      handleEvacuation(cmessage);
    }
  } else {
    if (!shouldCheckRed && Serial3.available()) {
      Serial3.readBytes(message, 1);
      if (message[0] == 'e') {
        shouldCheckRed = true;
      }
    }
    if (SEES_OBSTACLE()) {
      avoidObstacle();
    }
    if (_IR0() && _IR1() && _IR2() && _IR3() && _IR4() && _IR6() && _IR7() && _IR8() && _IR9() && _IR10()) {
      handleGreen();
      whiteCounter = 0;
    } else if (reading(IR5) && (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() > 3 && !_IR7() && !_IR8() && !_IR9() && !_IR10())) {
      handleGreen();
      whiteCounter = 0;
    } else if (reading(IR5) && (!_IR0() && !_IR1() && !_IR2() && !_IR3() && _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 3)) {
      handleGreen();
      whiteCounter = 0;
    } else {
      const int8_t position = -_IR0() - _IR1() - _IR2() - _IR3() - _IR4() + _IR6() + _IR7() + _IR8() + _IR9() + _IR10();
      Serial.println(position);
      int16_t leftMotorSpeed = (shouldCheckRed ? 80 : BASE_SPEED) + (position * SPEED_CHANGE);
      int16_t rightMotorSpeed = (shouldCheckRed ? 80 : BASE_SPEED) - (position * SPEED_CHANGE);
      drive(leftMotorSpeed, rightMotorSpeed);
      if (position == 0 && (!_IR0() && !_IR1() && !_IR2() && !_IR3() && !_IR4() && !reading(IR5) && !_IR6() && !_IR7() && !_IR8() && !_IR9() && !_IR10())) {
        if (++whiteCounter == (shouldCheckRed ? 20 : 4000) && !exitedEvacuation) {
          checkEvacuationEntrance();
          whiteCounter = 0;
        }
        Serial.println(whiteCounter);
        if (shouldCheckRed) {
          checkRed();
        }
      } else {
        whiteCounter = 0;
      }
    }
  }
}
