#define BASE_SPEED 100
#define MAX_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 400
int sensorPins[5] = { A0, A1, A2, A3, A4 };  // left2 -- left1 -- middle -- right1 -- right2
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

// char[6] lastCase = "00100";

int reading(int analog) {
  return analog > 90 ? 1 : 0;
}


void forward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  // Serial.println("FORWARD");
}

void stop(int ms) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  delay(ms);
  // Serial.println("STOP");
}
void forwardFor(int ms) {
  forward();
  delay(ms);
  stop(ms);
}
// void backwardFor(int ms) {
//   backward();
//   delay(ms);
//   stop(ms);
// }
void weakRight() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  // Serial.println("WEAK RIGHT");
}
void weakLeft() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  // Serial.println("WEAK LEFT");
}

void strongRight() {
  // forwardFor(200);
  while(!(!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4])))) {
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
  // Serial.println("STRONG RIGHT");
}
void strongLeft() {
  // forwardFor(200);
  while(!(!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4])))) {
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
    analogWrite(LEFT_MOTOR_PWM, 150);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 150);
  }
  // Serial.println("STRONG LEFT");
}

void loop() {
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
  if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 00000 || 11111
    // if(//lastCase != "00000" &&) {
    // }
    forward();
    //lastCase = "00000";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {
    forward();
    //lastCase = "11111";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 00100
    forward();
    //lastCase = "00100";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 01110
    forward();
    //lastCase = "01110";
  } else if (reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 10101
    forward();
    //lastCase = "10101";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 01010
    forward();
    //lastCase = "01010";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 01000
    strongLeft();
    //lastCase = "01000";
  } else if (reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 10000
    strongLeft();
    //lastCase = "10000";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 11000
    strongLeft();
    //lastCase = "11000";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 11100
    strongLeft();
    //lastCase = "11100";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 01100
    strongLeft();
    //lastCase = "01100";
  } else if (reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 10100
    strongLeft();
    //lastCase = "10100";
  } else if (reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 10110
    strongLeft();
    //lastCase = "10110";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 11010
    strongLeft();
    //lastCase = "11010";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 11110
    strongLeft();
    //lastCase = "11110";
  } else if (reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 11101
    strongLeft();
    //lastCase = "11101";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) { // 00010
    strongRight();
    //lastCase = "00010";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 00001
    strongRight();
    //lastCase = "00001";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 00011
    strongRight();
    //lastCase = "00011";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 00111
    strongRight();
    //lastCase = "00111";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && !reading(analogRead(sensorPins[4]))) {  // 00110
    strongRight();
    //lastCase = "00110";
  } else if (!reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 00101
    strongRight();
    //lastCase = "00101";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && !reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 01101
    strongRight();
    //lastCase = "01101";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && !reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 01011
    strongRight();
    //lastCase = "01011";
  } else if (!reading(analogRead(sensorPins[0])) && reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  //  01111
    strongRight();
    //lastCase = "01111";
  } else if (reading(analogRead(sensorPins[0])) && !reading(analogRead(sensorPins[1])) && reading(analogRead(sensorPins[2])) && reading(analogRead(sensorPins[3])) && reading(analogRead(sensorPins[4]))) {  // 10111
    strongRight();
    //lastCase = "10111";
  } else {
    //lastCase = "00100";
    // stop(1000);
    forward();
  }
}
