#include <Wire.h>
#include "defines.h"
#include "motors.h"
#include "colors.h"
#include "camera.h"
#include "imu.h"
#include "TOFs.h"
#include "obstacle.h"

// bool inEvacuation = false;
// char message[2];
int whiteCounter = 0;
bool shouldCheckColor = false;

void setup() {
  if (DEBUG_MODE) {
    Serial.begin(115200);
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
  digitalWrite(RIGHT_TOF_XSHUT, LOW);
  digitalWrite(FRONT_TOF_XSHUT, LOW);
  initServos();
  initTOFs();
  initColorSensors();
  Serial3.begin(9600);
  while (!digitalRead(CALIBRATION_PIN)) {
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
}

void loop() {
  if(Serial3.available()) {
    Serial3.readBytes(message, 1);
    // Serial.println(message);
    if(message[0] == 'e') {
      shouldCheckColor = true;
    }
  }
  // if(inEvacuation) {
  //   Serial3.readBytes(message, 1);
  //   Serial.println("hello");
  //   Serial.println(message);
  //   if(message[0] == 'E') {
  //     inEvacuation = false;
  //     exitedEvacuation = true;
  //     handleEvacuation('E');
  //   } else {
  //     if(wallFollowing) {
  //       //handleEvacuation(WALL_FOLLOW);
  //       wallFollow();
  //     } else {
  //       handleEvacuation(message[0]);
  //     }
  //    }
  // } else {
  if (SEES_OBSTACLE()) {
    avoidObstacle();
  }
  if (_IR0() && _IR1() && _IR2() && _IR3() && _IR4() && _IR6() && _IR7() && _IR8() && _IR9() && _IR10()) {
    handleGreen();
  } else if (reading(IR5) && (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() > 3 && !_IR7() && !_IR8() && !_IR9() && !_IR10())) {
    handleGreen();
  } else if (reading(IR5) && (!_IR0() && !_IR1() && !_IR2() && !_IR3() && _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 3)) {
    handleGreen();
  } else {
    const float position = - _IR0() - _IR1() - _IR2() - _IR3() - _IR4() + _IR6() + _IR7() + _IR8() + _IR9() + _IR10();
    // for (uint8_t i = 0; i < 11; i++) {
    //   position += digitalRead(sensorPins[i]) * weights[i];
    // }
    int16_t leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
    int16_t rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);
    drive(leftMotorSpeed, rightMotorSpeed);
    if (position == 0 && 
      (!_IR0() && 
      !_IR1() && 
      !_IR2() && 
      !_IR3() && 
      !_IR4() && 
      !reading(IR5) &&
      !_IR6() && 
      !_IR7() && 
      !_IR8() && 
      !_IR9() && 
      !_IR10())
    ) {
      if (SILVER_1() && SILVER_2() && SILVER_3()) {
        stop(1000);
      }
      if(shouldCheckColor) {
        checkRed();
      }
    }
  }
  // }
}
