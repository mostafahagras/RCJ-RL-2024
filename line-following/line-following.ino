#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include "defines.h"
#include "motors.h"
#include "colors.h"
#include "camera.h"
#include "imu.h"
#include "TOFs.h"

#define SPEED_CHANGE 250
#define OBSTACLE 22
#define interval 1000

unsigned long previousMillis = 0;
unsigned long deadEndPrevMillis = 0;
unsigned long greenPrevMillis = 0;
int whiteCounter = 0;
// bool inEvacuation = false;
// char message[2];

void setup() {
  /* Line following sensors */
  pinMode(sensorPins[0], INPUT);
  pinMode(sensorPins[1], INPUT);
  pinMode(sensorPins[2], INPUT);
  pinMode(sensorPins[3], INPUT);
  pinMode(sensorPins[4], INPUT);
  pinMode(sensorPins[5], INPUT);
  pinMode(sensorPins[6], INPUT);
  pinMode(sensorPins[7], INPUT);
  pinMode(sensorPins[8], INPUT);
  pinMode(sensorPins[9], INPUT);
  pinMode(sensorPins[10], INPUT);
  pinMode(OBSTACLE, INPUT);
  Wire.begin();
  initIMU();
  chooseBus(0);
  pinMode(RIGHT_TOF_XSHUT, OUTPUT);
  pinMode(FRONT_TOF_XSHUT, OUTPUT);
  digitalWrite(RIGHT_TOF_XSHUT, LOW);
  digitalWrite(FRONT_TOF_XSHUT, LOW);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  if(DEBUG_MODE) {
    Serial.begin(115200);
  }
  initServos();
  initTOFs();
  initColorSensors();
  Serial.println("after init");
  Serial3.begin(9600);
}

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
void loop() {
  // readColors(LEFT_COLOR);
  // readColors(RIGHT_COLOR);
  // Serial.println(); 
  Serial.println(getHeading());
  // checkGreen();
  // chooseBus(0);
  // Serial.print(", ");
  // distanceRight();
  // Serial.print(", ");
  // distanceFront();
  // Serial.print(", ");
  right90();

  // if(inEvacuation) {
    Serial3.readBytes(message, 1);
  //   Serial.println("hello");
  //   Serial.println(message);
  //   // if(message[0] == 'E') {
  //   //   inEvacuation = false;
  //   //   exitedEvacuation = true;
  //   //   handleEvacuation('E');
  //   // } else {
  //     if(wallFollowing) {
  //       //handleEvacuation(WALL_FOLLOW);
  //       wallFollow();
  //     } else {
  //       handleEvacuation(message[0]);
  //     }
  //   // }
  // } else {
  // if (
  //   (digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) > 4)
  //   && (digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) > 4)) {
  //   // forward();
  //   // delay(100);
  //   handleGreen(true);
  // } else if (
  //   (digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) >= 4) && reading(sensorPins[5]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
  //   handleGreen();
  // } else if (
  //   !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && reading(sensorPins[5]) && (digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) >= 4)) {
  //   handleGreen();
  // } else {
    // float position = 0;
  // for (uint8_t i = 0; i < 11; i++) {
  //   position += digitalRead(sensorPins[i]) * weights[i];
  // }
  // int16_t leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
  // int16_t rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);
  //   drive(leftMotorSpeed, rightMotorSpeed);
  // }
  // checkRed();
  // }
}
