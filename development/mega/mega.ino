// #define LEFT_GREEN 70   // F
// #define BOTH_GREEN 71   // G
// #define RIGHT_GREEN 72  // H
// #define LEFT_RED 81     // Q
// #define BOTH_RED 82     // R
// #define RIGHT_RED 83    // S
// #define BOTH_NONE 78    // N
#define BASE_SPEED 100
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define LEFT_MOTOR_FRONT 1
#define LEFT_MOTOR_BACK 0
#define RIGHT_MOTOR_FRONT 1
#define RIGHT_MOTOR_BACK 0
char message[2];

// void forwardFor(int ms) {
//   forward();
//   delay(ms);
// stop(ms);
// }

void forward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  // Serial.println("FORWARD");
}


void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  // pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  // pinMode(LEFT_MOTOR_PWM, OUTPUT);
  // pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  // pinMode(RIGHT_MOTOR_PWM, OUTPUT);
}

void loop() {
  // Serial.print(Serial2.available());
  Serial3.readBytes(message, 1);
  Serial.print(message);
  Serial.print("\t");
  if (message[0] == 70) {
    Serial.println("f");
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
    analogWrite(LEFT_MOTOR_PWM, 40);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 40);
  } else if (message[0] == 76) {
    Serial.println("l");
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK);
    analogWrite(LEFT_MOTOR_PWM, 80);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_FRONT);
    analogWrite(RIGHT_MOTOR_PWM, 80);
  } else if(message[0] == 82) {
    Serial.println("r");
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_FRONT);
    analogWrite(LEFT_MOTOR_PWM, 80);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK);
    analogWrite(RIGHT_MOTOR_PWM, 80);
  } else if(message[0] == 80) {
    Serial.println("p");
    analogWrite(LEFT_MOTOR_PWM, 40);
    digitalWrite(LEFT_MOTOR_DIRECTION, LEFT_MOTOR_BACK); 
    analogWrite(RIGHT_MOTOR_PWM, 40);
    digitalWrite(RIGHT_MOTOR_DIRECTION, RIGHT_MOTOR_BACK); 
  } else {
    Serial.println("N");
    analogWrite(LEFT_MOTOR_PWM, 0);
    analogWrite(RIGHT_MOTOR_PWM, 0);
  }
  // delay(1000);
}