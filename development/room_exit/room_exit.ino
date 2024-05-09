#include "Adafruit_VL53L0X.h"
#include <Wire.h>

int sensorPins[10]{ 53, 51, 49, 47, 45, 43, 41, 39, 37, 35 };
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

bool has_exited_room = false;
bool turn_right = false;

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
    Serial.println(F("Failed to boot right VL53L0X"));
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

void leftt() {
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

void forwardNextToWall(float rightTOF, float front) {
  Serial.print(rightTOF);
  Serial.print(" ");
  Serial.print(front);
  Serial.println();
  float dsRight = (rightTOF - 120);
  float dsFront = (front - 120);
  if(
      digitalRead(sensorPins[0])+
      digitalRead(sensorPins[1])+
      digitalRead(sensorPins[2])+
      digitalRead(sensorPins[3])+
      digitalRead(sensorPins[4])+
      digitalRead(sensorPins[5])+
      digitalRead(sensorPins[6])+
      digitalRead(sensorPins[7])+
      digitalRead(sensorPins[8])+
      digitalRead(sensorPins[9])+
      digitalRead(sensorPins[10]) >= 5
    ) {
      backward();
      delay(200);
      leftt();
      delay(200);
    } 
  else if(front < 150 && rightTOF <= 300) {
    leftt();
    delay(100);
  } else if(rightTOF > 500) {
    while (!(
      (digitalRead(sensorPins[0])+
      digitalRead(sensorPins[1])+
      digitalRead(sensorPins[2])+
      digitalRead(sensorPins[3])+
      digitalRead(sensorPins[4])+
      digitalRead(sensorPins[5])+
      digitalRead(sensorPins[6])+
      digitalRead(sensorPins[7])+
      digitalRead(sensorPins[8]) == 0
      && digitalRead(sensorPins[9]) && digitalRead(sensorPins[10]))
    )&&
    !(digitalRead(sensorPins[9])+
      digitalRead(sensorPins[10])+
      digitalRead(sensorPins[2])+
      digitalRead(sensorPins[3])+
      digitalRead(sensorPins[4])+
      digitalRead(sensorPins[5])+
      digitalRead(sensorPins[6])+
      digitalRead(sensorPins[7])+
      digitalRead(sensorPins[8]) == 0
      && digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]))) {
      digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
      digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
      analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
      analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED/2);
    }
    if(digitalRead(sensorPins[10])) {
      turn_right = true;
    }
    has_exited_room = true;
  } else if(rightTOF < 300 && rightTOF > 50) {
    forward();
  } else if(rightTOF <= 50) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
    analogWrite(LEFT_MOTOR_PWM, BASE_SPEED/2);
    analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED);
  } else if(rightTOF >= 300 ) {
    digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
    digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
    analogWrite(LEFT_MOTOR_PWM, BASE_SPEED);
    analogWrite(RIGHT_MOTOR_PWM, BASE_SPEED/2);
  } else {
    forward();
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
bool inRoom = true;
void loop() {
  if(!inRoom) {
    while(digitalRead(2) || digitalRead(3) || digitalRead(4)) {
      forward();
    }
    inRoom = true;
  } else {
    if(!has_exited_room && inRoom) {
      if(digitalRead(2)&&digitalRead(3)&&digitalRead(4)) {
        backward();
        delay(1500);
        leftt();
        delay(2500);
        forward();
        delay(2000);
      }
      float front = leftTOF();
      float right = frontTOF();
      forwardNextToWall(right, front);
    } else {
      if(turn_right) {
        right();
      } else {
        leftt();
      }
      delay(300);
      while(!(digitalRead(2)||digitalRead(3)||digitalRead(4))) {
        forward();
      }
      stop();
    }
  }
}
