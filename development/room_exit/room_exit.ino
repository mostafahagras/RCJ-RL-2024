#include "Adafruit_VL53L0X.h"
#include <Wire.h>

int digitalPins[10]{ 53, 51, 49, 47, 45, 39, A1, A2, A3, A4 };
#define analogIR A0
#define LOX1_ADDRESS 0x32
#define LOX2_ADDRESS 0x31
#define SHT_LOX1 6
#define SHT_LOX2 5
#define m(s) (min(254, max(s, 0)))
#define d(s) (s > 0 ? 0 : 1)

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
#define FRONT 0
#define BACK 1

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot left VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot front VL53L0X"));
    // while(1);
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
  lox2.rangingTest(&measure2, false);
  return measure2.RangeMilliMeter;
}

float frontTOF() {
  lox1.rangingTest(&measure1, false);
  return measure1.RangeMilliMeter;
}

void stop() {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
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

void forwardNextToWall(float distanceMmLeft, float distanceMmFront) {
  float dsLeft = (distanceMmLeft - 120); // ideal distance: 15cm
  float dsFront = (distanceMmFront - 120);
  if(dsFront < 0 && dsLeft < 0) {
    left();
    delay(2000);
  } else if(dsFront < 0 & distanceMmLeft > 500) {
    forward();
    delay(500);
    right();
    delay(2000);
    forward();
    delay(500);
  } else if(dsLeft < 50 && dsLeft > -50) {
    forward();
  } else {
  // too left -> dsLeft -ve, turn right, left motor > right motor
  // too right -> dsLeft +ve, turn left, left motor < right motor
    digitalWrite(LEFT_MOTOR_DIRECTION, BASE_SPEED + dsLeft > 0 ? FRONT : BACK);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BASE_SPEED - dsLeft > 0 ? FRONT : BACK);
    analogWrite(LEFT_MOTOR_PWM, m(BASE_SPEED + dsLeft));
    analogWrite(RIGHT_MOTOR_PWM, m(BASE_SPEED - dsLeft));
    delay(150);
    stop();
    forward();
    delay(150);
    stop();
    digitalWrite(LEFT_MOTOR_DIRECTION, BASE_SPEED - dsLeft > 0 ? FRONT : BACK);
    digitalWrite(RIGHT_MOTOR_DIRECTION, BASE_SPEED + dsLeft > 0 ? FRONT : BACK);
    analogWrite(LEFT_MOTOR_PWM, m(BASE_SPEED - dsLeft));
    analogWrite(RIGHT_MOTOR_PWM, m(BASE_SPEED + dsLeft));
    delay(150);
    stop();
  }
}

void setup() {
  // pinMode(25, INPUT);
  // pinMode(52, INPUT);
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  chooseBus(0);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();
}

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void loop() {
  // Serial.print(digitalRead(FRONT_OBSTACLE));
  // Serial.println(digitalRead(RIGHT_OBSTACLE));
  // Serial.println("loop");
  // for(int i = 0; i < 10; i++) {
  //   if(digitalRead(digitalPins[i])) {
  //     stop();
  //     while(true) {}
  //   }
  // }
  float left = leftTOF();
  float front = frontTOF();
  Serial.print(left);
  Serial.print(" ");
  Serial.print(front);
  Serial.println();
  forwardNextToWall(left, front);
  // read_dual_sensors();
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
}
