#define BASE_SPEED 100
#define MAX_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 400
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

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPins[0], INPUT);
  pinMode(sensorPins[1], INPUT);
  pinMode(sensorPins[2], INPUT);
  pinMode(sensorPins[3], INPUT);
  pinMode(sensorPins[4], INPUT);
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  // Serial.begin(9600);
}

void forward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  Serial.println("FORWARD");
}
void backward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}
void stop(int ms) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  delay(ms);
  Serial.println("STOP");
}
void forwardFor(int ms) {
  forward();
  delay(ms);
  stop(ms);
}
void backwardFor(int ms) {
  backward();
  delay(ms);
  stop(ms);
}
void weakRight() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  Serial.println("WEAK RIGHT");
}
void weakLeft() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  Serial.println("WEAK LEFT");
}
void strongRight() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
  analogWrite(LEFT_MOTOR_PWM, MAX_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, MAX_SPEED);
  Serial.println("STRONG RIGHT");
}
void strongLeft() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, MAX_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
  analogWrite(RIGHT_MOTOR_PWM, MAX_SPEED);
  Serial.println("STRONG LEFT");
}

void loop() {
  int sensorValues[5];
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
    sum += 1 - sensorValues[i];
  }
  // 0 white 1 black
  /* COVERED CASES 31
    FORWARD:
      00000 ✅
      11111 ✅
      00100 ✅
      01110 ✅
      10101 ✅
      01010 ✅
      10001
      11011
    LEFT:
      01000 ✅
      10000 ✅
      11000 ✅
      11100 ✅
      01100 ✅
      10100 ✅
      10110 ✅
      11010 ✅
      11110 ✅
      11101 ✅
      10010
      11001
    RIGHT:
      00010 ✅
      00001 ✅
      00011 ✅
      00111 ✅
      00110 ✅
      00101 ✅
      01101 ✅
      01011 ✅
      01111 ✅
      10111 ✅
      01001
      10011
  */
  // forward();
  if (sum == 0) {  // 00000 || 11111
    forward();
  } else if (sum == 5) {
    stop(1000);
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 00100
    forward();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 01110
    forward();
  } else if (digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 10101
    forward();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 01010
    forward();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 01000
    weakLeft();
  } else if (digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 10000
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 11000
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 11100
    strongLeft();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 01100
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 10100
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 10110
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 11010
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 11110
    strongLeft();
  } else if (digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 11101
    strongLeft();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {
    weakRight();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 00001
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 00011
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 00111
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && !digitalRead(sensorPins[4])) {  // 00110
    weakRight();
  } else if (!digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 00101
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && !digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 01101
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && !digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 01011
    strongRight();
  } else if (!digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  //  01111
    strongRight();
  } else if (digitalRead(sensorPins[0]) && !digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) && digitalRead(sensorPins[3]) && digitalRead(sensorPins[4])) {  // 10111
    strongRight();
  } else {
    // stop(1000);
  }
}
