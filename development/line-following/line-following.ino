// ALL VALUES ARE PLACEHOLDERS
#define BASE_SPEED 100
#define HIGH_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 400
// int sensorPins[5] = { 45, 47, 49, 51, 53 };  // left2 -- left1 -- middle -- right1 -- right2
int sensorPins[5] { A0, A1, A2, A3, A4 };
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
unsigned long interval = 1000;

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
  // Serial.begin(9600);
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
void stop(int ms) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  delay(ms);
}
bool checkGreen() {
  return false;
}

int reading(int analog) {
  return analog > 90 ? 1 : 0;
}

void loop() {
  int sensorValues[5];
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = reading(analogRead(sensorPins[i]));
    sum += sensorValues[i];
  }
  float position = 0;
  for (int i = 0; i < 5; i++) {
    position +=  reading(analogRead(sensorPins[i])) * weights[i];
  }

  int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE);
  int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE);

  int leftMotorDirection = leftMotorSpeed > 0 ? LEFT_MOTOR_FRONT : LEFT_MOTOR_BACK;
  int rightMotorDirection = rightMotorSpeed > 0 ? RIGHT_MOTOR_FRONT : RIGHT_MOTOR_BACK;

  if (
    reading(analogRead(sensorPins[0])) && 
    reading(analogRead(sensorPins[1])) && 
    reading(analogRead(sensorPins[2])) && 
    reading(analogRead(sensorPins[3])) && 
    reading(analogRead(sensorPins[4]))
  ) {
    previousMillis = millis();
    stop(0);
    while(millis() - previousMillis < interval) {
      if(checkGreen()) break;
    }
    leftMotorSpeed = lastLeftMotorSpeed;
    rightMotorSpeed = lastRightMotorSpeed;
    leftMotorDirection = lastLeftMotorDirection;
    rightMotorDirection = lastRightMotorDirection;
    digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
    digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
    analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
    analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
    delay(250);
  } else {
    if(leftMotorSpeed != rightMotorSpeed || leftMotorDirection != rightMotorDirection) {
      lastLeftMotorSpeed = rightMotorSpeed;
      lastRightMotorSpeed = leftMotorSpeed;
      lastLeftMotorDirection = rightMotorDirection;
      lastRightMotorDirection = leftMotorDirection;
    }
  }

  digitalWrite(LEFT_MOTOR_DIRECTION, leftMotorDirection);
  digitalWrite(RIGHT_MOTOR_DIRECTION, rightMotorDirection);
  analogWrite(LEFT_MOTOR_PWM, min(abs(leftMotorSpeed), HIGH_SPEED));
  analogWrite(RIGHT_MOTOR_PWM, min(abs(rightMotorSpeed), HIGH_SPEED));
}
