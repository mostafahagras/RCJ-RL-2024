#include "Adafruit_VL53L0X.h"

#define LOX1_ADDRESS 0x31
#define LOX2_ADDRESS 0x30
#define SHT_LOX1 52
#define SHT_LOX2 24

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

#define FRONT_OBSTACLE 23
#define RIGHT_OBSTACLE 52
#define BASE_SPEED 80
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 1
#define BACK 0

void setID() {
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println("Failed to boot left VL53L0X");
    while(1);
  }
  delay(10);

  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println("Failed to boot front VL53L0X");
    while(1);
  }
}

void read_dual_sensors() {
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false);

  Serial.print("1: ");
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  Serial.print(" 2: ");
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.println();
}

float leftTOF() {
  lox1.rangingTest(&measure1, false);
  return measure1.RangeMilliMeter;
}

float frontTOF() {
  lox2.rangingTest(&measure2, false);
  return measure2.RangeMilliMeter;
}

void forward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void backward() {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void left() {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void right() {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
}

void forwardNextToWall(float distanceMm) {
  float ds = distanceMm - 120; // ideal distance: 15cm
  // too left -> ds -ve, turn right, left motor > right motor
  // too right -> ds +ve, turn left, left motor < right motor
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, max(BASE_SPEED - ds/2, 0));
  analogWrite(RIGHT_MOTOR_PWM, min(BASE_SPEED + ds/2, 254));
  Serial.print(", ");
  Serial.print(ds);
  Serial.print(", ");
  Serial.print(min(BASE_SPEED + ds/2, 254));
  Serial.print(", ");
}

void setup() {
  pinMode(25, INPUT);
  pinMode(52, INPUT);
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Serial.begin(9600);
  Serial.println("asdf");
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();
}

void loop() {
  // Serial.print(digitalRead(FRONT_OBSTACLE));
  // Serial.println(digitalRead(RIGHT_OBSTACLE));
  // Serial.println("loop");
  float left = leftTOF();
  float front = frontTOF();
  Serial.print(left);
  Serial.print(" ");
  Serial.print(front);
  Serial.println();
  forwardNextToWall(left);
  // if (digitalRead(FRONT_OBSTACLE) && digitalRead(RIGHT_OBSTACLE)) { // doesn't read doesn't read
  //   backward();
  // } else if (digitalRead(FRONT_OBSTACLE) && !digitalRead(RIGHT_OBSTACLE)) { // doesn't read reads
  //   forward();
  // } else if (!digitalRead(FRONT_OBSTACLE) && digitalRead(RIGHT_OBSTACLE)) { // reads doesn't read
  //   right();
  // } else if (!digitalRead(FRONT_OBSTACLE) && !digitalRead(RIGHT_OBSTACLE)) { // reads reads
  //   while(!digitalRead(FRONT_OBSTACLE)) {
  //     left();
  //   }
  //   // forward();
  //   // delay(250);
  //   while(digitalRead(RIGHT_OBSTACLE)) {
  //     left();
  //   }
    // forward();
    // delay(250);
  // }
}

