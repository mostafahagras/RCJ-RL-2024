#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#define BLACK_THRESHOLD 150
#define NONE 0
#define GREEN 1
#define RED 2
#define BASE_SPEED 60
#define RAMP_SPEED 150
#define MAX_SPEED 254
#define SPEED_CHANGE 250
// int sensorPins[5]{ 53, 51, 49, 47, 45 };
int sensorPins[11]{ 53, 51, 49, 47, 45, A0, 43, 41, 39, 37, 35 };
#define OBSTACLE 22
#define LEFT_OBSTACLE 39
#define RIGHT_OBSTACLE 52
#define TILT 53
float weights[11] = { -0.5, -0.5, -0.5, -0.5, -0.5, 0, 0.5, 0.5, 0.5, 0.5, 0.5 };
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 0
#define BACK 1
#define interval 1000

Adafruit_TCS34725 tcs[] = {
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
};

unsigned long previousMillis = 0;
unsigned long deadEndPrevMillis = 0;
unsigned long greenPrevMillis = 0;
int whiteCounter = 0;
bool inEvacuation = false;
Servo arm, hand;
char message[2];
bool dropped = true;

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
  pinMode(LEFT_OBSTACLE, INPUT);
  pinMode(RIGHT_OBSTACLE, INPUT);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Serial.begin(9600);
  initColorSensors();
  // Serial.println("after init");
  // arm.attach(7);
  // hand.attach(8);
  // Serial2.begin(9600);
}

void initColorSensors() {
  for (int i = 0; i < 2; i++) {
    chooseBus(i);
    Serial.print(i+1);
    if(tcs[i].begin()) {
      Serial.println("Done");
    } else {
      Serial.println("Failed");
    }
  }
}

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

int readColors(byte sensorNum) {
  chooseBus(sensorNum+1);
  float r, g, b;
  tcs[sensorNum].getRGB(&r, &g, &b);
  Serial.print(sensorNum+1);
  Serial.print(": ");
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print("\t");
  Serial.print("G: ");
  Serial.print(g);
  Serial.print("\t");
  Serial.print("B: ");
  Serial.print(b);
  Serial.print("\t");
  if (g - r > 30 && g - b > 30) {
    return GREEN;
  } else if (r - g > 30 && r - b > 30) {
    return RED;
  } else {
    return NONE;
  }
}

void stop(int ms) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  delay(ms);
}

void forward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void right() {
  greenPrevMillis = millis();
  while (
    !(
      (
        digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2]) || digitalRead(sensorPins[3]) || digitalRead(sensorPins[4]) || reading(sensorPins[5]))
      && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10]))
    || millis() - greenPrevMillis < 500) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}

void left() {
  greenPrevMillis = millis();
  while (
    !(
      !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && (reading(sensorPins[5]) || digitalRead(sensorPins[6]) || digitalRead(sensorPins[7]) || digitalRead(sensorPins[8]) || digitalRead(sensorPins[9]) || digitalRead(sensorPins[10])))
    || millis() - greenPrevMillis < 500) {
    digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}

void deadEnd() {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  delay(200);
  deadEndPrevMillis = millis();
  while (!(
           !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && reading(sensorPins[5]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10]))
         || (millis() - deadEndPrevMillis < interval)) {
    digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}

char checkGreen() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
  // Serial.println();
  if (SensorOneReading == GREEN && SensorTwoReading == GREEN) {
    // Serial.println("BOTH GREEN");
    return 'G';
  } else if (SensorOneReading == GREEN && SensorTwoReading != GREEN) {
    // Serial.println("LEFT GREEN");
    return 'F';
  } else if (SensorOneReading != GREEN && SensorTwoReading == GREEN) {
    // Serial.println("RIGHT GREEN");
    return 'H';
  } else {
    return 'N';
  }
}

char checkRed() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
  if (SensorOneReading == RED || SensorTwoReading == RED) {
    stop(6000);
  }
}

int reading(int pin) {
  return analogRead(pin) > 500 ? 1 : 0;
}

void obstacle() {
  if (!digitalRead(OBSTACLE)) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 127);
    analogWrite(RIGHT_MOTOR_PWM, 50);
    delay(2000);
    while (!digitalRead(sensorPins[2])) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 30);
      analogWrite(RIGHT_MOTOR_PWM, 127);
    }
    forward();
    delay(300);
    while (!(!(digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2])) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 150);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(RIGHT_MOTOR_PWM, 150);
    }
  }
}

void handleGreen() {
  stop(0);
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
      left();
      forward();
      delay(200);
    } else if (!leftGreen && rightGreen) {
      right();
      forward();
      delay(200);
    } else if (!leftGreen && !rightGreen) {
      forward();
      delay(100);
    }
}

void loop() {
  readColors(0);
  readColors(1);
  Serial.println();
  // if(!(digitalRead(2) || digitalRead(3) || digitalRead(4))) {
  //   inEvacuation = true;
  // }
  // if (inEvacuation) {
  //   Serial2.readBytes(message, 1);
  //   Serial.print(message[0]);
  //   if (message[0] == 70) {  // FORWARD
  //     // Serial.println("F");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //   } else if (message[0] == 82) {  // RIGHT
  //     // Serial.println("R");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 80);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(RIGHT_MOTOR_PWM, 80);
  //     // delay(100);
  //   } else if (message[0] == 76) {  // LEFT
  //     // Serial.println("L");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 80);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(RIGHT_MOTOR_PWM, 80);
  //     // delay(100);
  //   } else if (message[0] == 80) {  // PICK
  //     if (dropped) {
  //       // Serial.println("P");
  //       digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //       digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //       analogWrite(LEFT_MOTOR_PWM, 40);
  //       analogWrite(RIGHT_MOTOR_PWM, 40);
  //       delay(500);
  //       stop(0);
  //       hand.write(35);
  //       arm.write(45);
  //       delay(3000);
  //       arm.write(135);
  //       delay(200);
  //       arm.write(90);
  //       digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //       digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //       analogWrite(LEFT_MOTOR_PWM, 40);
  //       analogWrite(RIGHT_MOTOR_PWM, 40);
  //       delay(1000);
  //       stop(0);
  //       hand.write(120);
  //       delay(1000);
  //       arm.write(135);
  //       delay(1000);
  //       arm.write(90);
  //       dropped = false;
  //     }
  //   } else if (message[0] == 68) {  // DROP
  //     // Serial.println("D");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //     delay(3000);
  //     stop(0);
  //     arm.write(45);
  //     delay(500);
  //     arm.write(90);
  //     hand.write(35);
  //     delay(1000);
  //     arm.write(135);
  //     delay(1500);
  //     arm.write(90);
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //     delay(1000);
  //     dropped = true;
  //     // arm.write(45);
  //     // delay(3000);
  //     // arm.write(135);
  //     // delay(100);
  //     // arm.write(90);
  //   } else if (message[0] == 66) {  // BACKWARD
  //                                   // Serial.println("B");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //   } else {
  //     // Serial.println(message);
  //     //   stop(0);
  //   } 
  // } 
  // else {
  // obstacle();
  // checkTilt();
  // }
  // else {
  float position = 0;
  for (int i = 0; i < 11; i++) {
    position += digitalRead(sensorPins[i]) * weights[i];
  }
  int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
  int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);
  int leftMotorDirection = leftMotorSpeed > 0 ? FRONT : BACK;
  int rightMotorDirection = rightMotorSpeed > 0 ? FRONT : BACK;
  if (
    (digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) > 3)
    && (digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) > 3)) {
    forward();
    delay(100);
    handleGreen();
  } else if (
    (digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) >= 4) && reading(sensorPins[5]) && !digitalRead(sensorPins[6]) && !digitalRead(sensorPins[7]) && !digitalRead(sensorPins[8]) && !digitalRead(sensorPins[9]) && !digitalRead(sensorPins[10])) {
    handleGreen();
  } else if (
    !digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4]) && reading(sensorPins[5]) && (digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) >= 4)) {
    handleGreen();
  } else {
    digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
    digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
    analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), MAX_SPEED));
    analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), MAX_SPEED));
  }
  
  // if (!(digitalRead(sensorPins[0]) || digitalRead(sensorPins[1]) || digitalRead(sensorPins[2]) || digitalRead(sensorPins[3]) || digitalRead(sensorPins[4]) || digitalRead(sensorPins[5]) || digitalRead(sensorPins[6]) || digitalRead(sensorPins[7]) || digitalRead(sensorPins[8]) || digitalRead(sensorPins[9]) || digitalRead(sensorPins[10]))) {
  //   whiteCounter++;
  //   char result = checkRed();
  //   if (result == 81 || result == 82 || result == 83) {
  //     stop(6000);
  //   } else {
  //     if ((digitalRead(LEFT_OBSTACLE) + digitalRead(RIGHT_OBSTACLE) == 1) && whiteCounter >= 50) {
  //       inEvacuation = true;
  //       digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //       digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //       analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  //       analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  //       delay(500);
  //       Serial2.write(0);
  //     } else {
  //       forward();
  //     }
  //   }
  // } else {
  //   whiteCounter = 0;
  // }
  // }
}
