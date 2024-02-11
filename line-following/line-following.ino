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
int sensorPins[5]{ A0, A1, A2, A3, A4 };
#define OBSTACLE 22
#define LEFT_OBSTACLE 23
#define RIGHT_OBSTACLE 52
#define TILT 53
float weights[5] = { -0.5, -0.5, 0, 0.5, 0.5 };
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 1
#define BACK 0
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
  pinMode(OBSTACLE, INPUT);
  pinMode(LEFT_OBSTACLE, INPUT);
  pinMode(RIGHT_OBSTACLE, INPUT);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  initColorSensors();
  arm.attach(7);
  hand.attach(8);
  Serial3.begin(9600);
  // Serial.begin(9600);
}

void initColorSensors() {
  for (int i = 0; i < 2; i++) {
    chooseBus(i);
    tcs[i].begin();
  }
}

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

int readColors(byte sensorNum) {
  chooseBus(sensorNum);
  float r, g, b;
  tcs[sensorNum].getRGB(&r, &g, &b);
  if (g - r > 20 && g - b > 20) {
    return GREEN;
  } else if (r - g > 20 && r - b > 20) {
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

void highSpeedForward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, RAMP_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, RAMP_SPEED);
}

void checkTilt() {
  while (!digitalRead(TILT)) {
    highSpeedForward();
  }
}

void right() {
  greenPrevMillis = millis();
  while (
    !((reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2]))) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) || millis() - greenPrevMillis < 500) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}

void left() {
  greenPrevMillis = millis();
  while (
    !(!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && (reading(analogRead(sensorPins[2])) || reading(analogRead(sensorPins[3])) || reading(analogRead(sensorPins[4])))) || millis() - greenPrevMillis < 500) {
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
  while (!(!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) || (millis() - deadEndPrevMillis < interval)) {
    digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}

char checkGreen() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
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

char checkRed() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
  if (SensorOneReading == RED || SensorTwoReading == RED) {
    stop(6000);
  }
}

int reading(int analog) {
  return analog > 150 ? 1 : 0;
}

// int reading(uint8_t pin) {
//   int analog = analogRead(pin)
//   if(pin == A0) {
//     return analog > 150 ? 1 : 0;
//   } else if(pin == A1) {
//     return analog > 150 ? 1 : 0;
//   } else if(pin == A2) {
//     return analog > 150 ? 1 : 0;
//   } else if(pin == A3) {
//     return analog > 150 ? 1 : 0;
//   } else if(pin == A4) {
//     return analog > 150 ? 1 : 0;
//   }
// }

void obstacle() {
  if (!digitalRead(OBSTACLE)) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 127);
    analogWrite(RIGHT_MOTOR_PWM, 50);
    delay(2000);
    while (!reading(analogRead(sensorPins[2]))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 30);
      analogWrite(RIGHT_MOTOR_PWM, 127);
    }
    forward();
    delay(300);
    while (!(!(reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2]))) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4])))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 150);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(RIGHT_MOTOR_PWM, 150);
    }
  }
}
bool firstEvacuationIteration = true;
void loop() {
  if (inEvacuation) {
    Serial3.readBytes(message, 1);
    // Serial.print(message);
    if (message[0] == 70) {  // FORWARD
      // Serial.println("F");
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 40);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(RIGHT_MOTOR_PWM, 40);
    } else if (message[0] == 82) {  // RIGHT
      // Serial.println("R");
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 80);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(RIGHT_MOTOR_PWM, 80);
      // delay(100);
    } else if (message[0] == 76) {  // LEFT
      // Serial.println("L");
      digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
      analogWrite(LEFT_MOTOR_PWM, 80);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(RIGHT_MOTOR_PWM, 80);
      // delay(100);
    } else if (message[0] == 80) {  // PICK
      if (dropped) {
        // Serial.println("P");
        digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
        digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
        analogWrite(LEFT_MOTOR_PWM, 40);
        analogWrite(RIGHT_MOTOR_PWM, 40);
        delay(500);
        stop(0);
        hand.write(35);
        arm.write(45);
        delay(3000);
        arm.write(135);
        delay(200);
        arm.write(90);
        digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
        digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
        analogWrite(LEFT_MOTOR_PWM, 40);
        analogWrite(RIGHT_MOTOR_PWM, 40);
        delay(1000);
        stop(0);
        hand.write(120);
        delay(1000);
        arm.write(135);
        delay(1000);
        arm.write(90);
        dropped = false;
      }
    } else if (message[0] == 68) {  // DROP
      // Serial.println("D");
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 40);
      analogWrite(RIGHT_MOTOR_PWM, 40);
      delay(3000);
      stop(0);
      arm.write(45);
      delay(500);
      arm.write(90);
      hand.write(35);
      delay(1000);
      arm.write(135);
      delay(1500);
      arm.write(90);
      digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(LEFT_MOTOR_PWM, 40);
      analogWrite(RIGHT_MOTOR_PWM, 40);
      delay(1000);
      dropped = true;
      // arm.write(45);
      // delay(3000);
      // arm.write(135);
      // delay(100);
      // arm.write(90);
    } else if (message[0] == 66) {  // BACKWARD
                                    // Serial.println("B");
      digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
      analogWrite(LEFT_MOTOR_PWM, 40);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(RIGHT_MOTOR_PWM, 40);
    } else {

      // Serial.println(message);
      //   stop(0);
    }
  } else {
    obstacle();
    checkTilt();
    float position = 0;
    for (int i = 0; i < 5; i++) {
      position += reading(analogRead(sensorPins[i])) * weights[i];
    }
    int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
    int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);
    int leftMotorDirection = leftMotorSpeed > 0 ? FRONT : BACK;
    int rightMotorDirection = rightMotorSpeed > 0 ? FRONT : BACK;
    if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {
      forward();
      delay(100);
      stop(0);
      bool leftGreen = false;
      bool rightGreen = false;
      previousMillis = millis();
      while (millis() - previousMillis < interval) {
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
        delay(100);
      } else if (!leftGreen && rightGreen) {
        right();
        forward();
        delay(100);
      } else {
        if (!leftGreen && !rightGreen) {
          forward();
          delay(100);
        }
      }
    } else if ((reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1]))) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {
      stop(0);
      bool leftGreen = false;
      bool rightGreen = false;
      previousMillis = millis();
      while (millis() - previousMillis < interval) {
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
        delay(100);
      } else if (!leftGreen && rightGreen) {
        right();
        forward();
        delay(100);
      } else {
        if (!leftGreen && !rightGreen) {
          forward();
          delay(100);
        }
      }
    } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && (reading(analogRead(sensorPins[3])) || reading(analogRead(sensorPins[4])))) {
      stop(0);
      bool leftGreen = false;
      bool rightGreen = false;
      previousMillis = millis();
      while (millis() - previousMillis < interval) {
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
      } else if (!leftGreen && rightGreen) {
        right();
      } else {
        if (!leftGreen && !rightGreen) {
          forward();
          delay(100);
        }
      }
    } else {
      digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
      digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
      analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), MAX_SPEED));
      analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), MAX_SPEED));
    }
    if (!(reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2])) || reading(analogRead(sensorPins[3])) || reading(analogRead(sensorPins[4])))) {
      whiteCounter++;
      char result = checkRed();
      if (result == 81 || result == 82 || result == 83) {
        stop(6000);
      } else {
        if ((digitalRead(LEFT_OBSTACLE) + digitalRead(RIGHT_OBSTACLE) == 1) && whiteCounter >= 50) {
          inEvacuation = true;
          digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
          digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
          analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
          analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
          delay(500);
          Serial3.write(0);
        } else {
          forward();
        }
      }
    } else {
      whiteCounter = 0;
    }
  }
}
