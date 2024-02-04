#define BASE_SPEED 100
#define HIGH_SPEED 254
// #define LOW_SPEED 50
#define SPEED_CHANGE 400
int sensorPins[5]{ A0, A1, A2, A3, A4 };
#define OBSTACLE 23
float weights[5] = { -1, -0.5, 0, 0.5, 1 };
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 1
#define BACK 0

char message[2];
unsigned long previousMillis = 0;
unsigned long deadEndPrevMillis = 0;
unsigned long interval = 1000;

void setup() {
  /* Line following sensors */
  pinMode(sensorPins[0], INPUT);
  pinMode(sensorPins[1], INPUT);
  pinMode(sensorPins[2], INPUT);
  pinMode(sensorPins[3], INPUT);
  pinMode(sensorPins[4], INPUT);
  pinMode(OBSTACLE, INPUT);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600);
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
  while (!((reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2]))) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4])))) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
}
void left() {
  while (!(!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && (reading(analogRead(sensorPins[2])) || reading(analogRead(sensorPins[3])) || reading(analogRead(sensorPins[4]))))) {
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
  Serial2.readBytes(message, 1);
  if (message[0] == 70 || message[0] == 72 || message[0] == 71) {
    return message[0];
  } else {
    return 'N';
  }
}

int reading(int analog) {
  return analog > 120 ? 1 : 0;
}

void obstacle() {
  if (!digitalRead(OBSTACLE)) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
    analogWrite(LEFT_MOTOR_PWM, 127);
    analogWrite(RIGHT_MOTOR_PWM, 50);
    delay(1700);
    while (!reading(analogRead(sensorPins[2]))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 10);
      analogWrite(RIGHT_MOTOR_PWM, 127);
    }
    forward();
    delay(300);
    while (!(!(reading(analogRead(sensorPins[0])) || !reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2]))) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4])))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, 150);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(RIGHT_MOTOR_PWM, 150);
    }
  }
}

void loop() {
  Serial.print(reading(analogRead(sensorPins[0])));
  Serial.print(reading(analogRead(sensorPins[1])));
  Serial.print(reading(analogRead(sensorPins[2])));
  Serial.print(reading(analogRead(sensorPins[3])));
  Serial.print(reading(analogRead(sensorPins[4])));
  Serial.println();
  obstacle();
  float position = 0;
  for (int i = 0; i < 5; i++) {
    position += reading(analogRead(sensorPins[i])) * weights[i];
  }
  int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
  int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);
  int leftMotorDirection = leftMotorSpeed > 0 ? FRONT : BACK;
  int rightMotorDirection = rightMotorSpeed > 0 ? FRONT : BACK;

  if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {
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
    } else if (!leftGreen && rightGreen) {
      right();
    } else {
      if (!leftGreen && !rightGreen) {
        digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
        digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
        analogWrite(LEFT_MOTOR_PWM, min(abs(100), HIGH_SPEED));
        analogWrite(RIGHT_MOTOR_PWM, min(abs(100), HIGH_SPEED));
      }
    }
  }
  // else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {
  //   stop(0);
  //   bool leftGreen = false;
  //   previousMillis = millis();
  //   while (millis() - previousMillis < interval) {
  //     char result = checkGreen();
  //     if (result == 70) {
  //       leftGreen = true;
  //       break;
  //     }
  //   }
  //   if (leftGreen) {
  //     left();
  //   } else {
  //     int goForward = digitalRead(FRONT_IR);
  //     if (goForward) {
  //       forward();
  //     } else {
  //       left();
  //     }
  //   }
  // } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {
  //   stop(0);
  //   bool rightGreen = false;
  //   previousMillis = millis();
  //   while (millis() - previousMillis < interval) {
  //     char result = checkGreen();
  //     if (result == 72) {
  //       rightGreen = true;
  //       break;
  //     }
  //   }
  //   if (rightGreen) {
  //     right();
  //   } else {
  //     int goForward = digitalRead(FRONT_IR);
  //     if (goForward) {
  //       forward();
  //     } else {
  //       right();
  //     }
  //   }
  // }
  else {
    digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
    digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
    analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
    analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
  }
  while (!(reading(analogRead(sensorPins[0])) || reading(analogRead(sensorPins[1])) || reading(analogRead(sensorPins[2])) || reading(analogRead(sensorPins[3])) || reading(analogRead(sensorPins[4])))) {
    obstacle();
    Serial2.readBytes(message, 1);
    if (message[0] == 81 || message[0] == 82 || message[0] == 83) {
      stop(6000);
    } else {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
      analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
      analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
    }
  }
}
