#define SPEED 120
#define DELAY 1000
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 0
#define BACK 1

void stop(unsigned long ms = 0) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  if (ms) delay(ms);
}

void forward(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void backward(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void right(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void left(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void setup() {
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
}

void loop() {
  forward(SPEED, DELAY);
  stop(DELAY);
  backward(SPEED, DELAY);
  stop(DELAY);
  right(SPEED, DELAY);
  stop(DELAY);
  left(SPEED, DELAY);
  stop(DELAY);
}
