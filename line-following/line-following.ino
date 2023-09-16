// ALL VALUES ARE PLACEHOLDERS 
#define BASE_SPEED 100
#define HIGH_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 400
int sensorPins[5] = { 32, 34, 36, 38, 40 }; // left2 -- left1 -- middle -- right1 -- right2
float weights[5] = { -1, -0.5, 0, 0.5, 1 };
#define LEFT_MOTOR_DIRECTION 7
#define RIGHT_MOTOR_DIRECTION 9
#define LEFT_MOTOR_PWM 8
#define RIGHT_MOTOR_PWM 10
#define LEFT_MOTOR_FRONT 0
#define LEFT_MOTOR_BACK 1
#define RIGHT_MOTOR_FRONT 1
#define RIGHT_MOTOR_BACK 0
// #define LEFT_RED 1
// #define LEFT_GREEN 2
// #define RIGHT_RED 3
// #define RIGHT_GREEN 4

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
}

int lastMotorSpeed1 = BASE_SPEED;
int lastMotorSpeed2 = BASE_SPEED;
int lastMotorDirection1 = 0;
int lastMotorDirection2 = 1;

void loop() {
  int sensorValues[5];
  for(int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
  float position = 0;
  for(int i = 0; i < 5; i++) {
    position += sensorValues[i] * weights[i];
  }
  int leftMotorSpeed = BASE_SPEED + (position * SPEED_CHANGE );
  int rightMotorSpeed = BASE_SPEED - (position * SPEED_CHANGE );

  int leftMotorDirection = leftMotorSpeed > 0 ? LEFT_MOTOR_FRONT : LEFT_MOTOR_BACK;
  int rightMotorDirection = rightMotorSpeed > 0 ? RIGHT_MOTOR_FRONT : RIGHT_MOTOR_BACK;

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