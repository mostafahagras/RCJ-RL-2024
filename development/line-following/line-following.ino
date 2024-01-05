// ALL VALUES ARE PLACEHOLDERS
#define BASE_SPEED 100
#define HIGH_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 200
int sensorPins[5] = { 45, 47, 49, 51, 53 };  // left2 -- left1 -- middle -- right1 -- right2
float weights[5] = { -1, -0.5, 0, 0.5, 1 };
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define LEFT_MOTOR_FRONT 1
#define LEFT_MOTOR_BACK 0
#define RIGHT_MOTOR_FRONT 1
#define RIGHT_MOTOR_BACK 0
// #define LEFT_RED 1
// #define LEFT_GREEN 2
// #define RIGHT_RED 3
// #define RIGHT_GREEN 4

unsigned long previousMillis = 0;
unsigned long interval = 2000UL;

void setup() {
  /* Line following sensors */
  pinMode(sensorPins[0], INPUT);
  pinMode(sensorPins[1], INPUT);
  pinMode(sensorPins[2], INPUT);
  pinMode(sensorPins[3], INPUT);
  pinMode(sensorPins[4], INPUT);
  /* Color sensors */
  // pinMode(LEFT_RED, INPUT);
  // pinMode(LEFT_GREEN, INPUT);
  // pinMode(RIGHT_RED, INPUT);
  // pinMode(RIGHT_GREEN, INPUT);
  /* Motors */
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Serial.begin(9600);
}

int lastLeftMotorSpeed = BASE_SPEED;
int lastRightMotorSpeed = BASE_SPEED;
int lastLeftMotorDirection = LEFT_MOTOR_FRONT;
int lastRightMotorDirection = RIGHT_MOTOR_FRONT;
int count = 0;

void resetLastValues() {
  lastLeftMotorSpeed = BASE_SPEED;
  lastRightMotorSpeed = BASE_SPEED;
  lastLeftMotorDirection = LEFT_MOTOR_FRONT;
  lastRightMotorDirection = RIGHT_MOTOR_FRONT;
  count = 0;
}

void loop() {
  int sensorValues[5];
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
    sum += 1 - sensorValues[i];
  }
  float position = 0;
  for (int i = 0; i < 5; i++) {
    position += (1 - sensorValues[i]) * weights[i];
  }
  Serial.print(sensorValues[0]);
  Serial.print(sensorValues[1]);
  Serial.print(sensorValues[2]);
  Serial.print(sensorValues[3]);
  Serial.print(sensorValues[4]);
  Serial.print("\t");

  int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
  int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);

  int leftMotorDirection = leftMotorSpeed > 0 ? LEFT_MOTOR_FRONT : LEFT_MOTOR_BACK;
  int rightMotorDirection = rightMotorSpeed > 0 ? RIGHT_MOTOR_FRONT : RIGHT_MOTOR_BACK;
  Serial.print("Left Speed: ");
  Serial.print(leftMotorSpeed);
  Serial.print(", Direction: ");
  Serial.print(leftMotorDirection);
  Serial.print(", Right Speed: ");
  Serial.print(rightMotorSpeed);
  Serial.print(", Direction: ");
  Serial.print(rightMotorDirection);
  Serial.println();
  if (sum == 0 && count == 5) {
    leftMotorSpeed = lastLeftMotorSpeed;
    rightMotorSpeed = lastRightMotorSpeed;
    leftMotorDirection = lastLeftMotorDirection;
    rightMotorDirection = lastRightMotorDirection;
    digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
    digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
    analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
    analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
    while (millis() - previousMillis > interval) {
      previousMillis += interval;
      for (int i = 0; i < 5; i++) {
        sensorValues[i] = digitalRead(sensorPins[i]);
        sum += 1 - sensorValues[i];
      }
      if (sum > 0) {
        break;
      }
      digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
      digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
      analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
      analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
    }
    resetLastValues();
  } else if (sum == 0 && count < 5) {
    count++;
  }
  // else if (sum >= 3) {
  //   analogWrite(LEFT_MOTOR_PWM, 0);
  //   analogWrite(RIGHT_MOTOR_PWM, 0);
  //   delay(1000);
  //   digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  //   digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  //   analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  //   analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  //   delay(500);
  // }
  else {
    lastLeftMotorSpeed = leftMotorSpeed;
    lastRightMotorSpeed = rightMotorSpeed;
    lastLeftMotorDirection = leftMotorDirection;
    lastRightMotorDirection = rightMotorDirection;
  }

  digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
  digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);

  analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
  analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
  // int sensorsValuesSum = 0;
  // for(int i = 0; i < 5; i++) {
  //   sensorValues[i] = digitalRead(sensorPins[i]);
  //   sensorsValuesSum += sensorValues[i];
  // }

  // if(sensorsValuesSum = 5) {
  //   int leftRedValue = digitalRead(LEFT_RED);
  //   int rightRedValue = digitalRead(RIGHT_RED);

  //   if (leftRedValue == 1 || rightRedValue == 1) {
  //     motorSpeed1 = 0;
  //     motorSpeed2 = 0;
  //   }
  // }

  // if(sensorsValuesSum < 3) {
  //   int leftGreenValue = digitalRead(LEFT_GREEN);
  //   int rightGreenValue = digitalRead(RIGHT_GREEN);
  //   if (leftGreenValue + rightGreenValue == 2) {
  //     // rotate 180deg
  //   } else if (leftGreenValue == 1 && rightGreenValue == 0) {
  //     // rotate -90deg
  //   } else if (leftGreenValue == 0 && rightGreenValue == 1) {
  //     // rotate 90deg
  //   }
  // }
}
