#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define LOX1_ADDRESS 0x32
#define LOX2_ADDRESS 0x31
#define SHT_LOX1 32
#define SHT_LOX2 34

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
#define NONE 0
#define GREEN 1
#define RED 2
#define LEFT_GREEN 70   // F
#define BOTH_GREEN 71   // G
#define RIGHT_GREEN 72  // H
#define LEFT_RED 81     // Q
#define BOTH_RED 82     // R
#define RIGHT_RED 83    // S
#define BOTH_NONE 78    // N

Adafruit_TCS34725 tcs[] = {
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X),
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  // pinMode(24, INPUT);
  // pinMode(52, INPUT);
  chooseBus(0);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();
  initColorSensors();
}

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

float leftTOF() {
  lox2.rangingTest(&measure2, false);
  return measure2.RangeMilliMeter;
}

float frontTOF() {
  lox1.rangingTest(&measure1, false);
  return measure1.RangeMilliMeter;
}

unsigned long t = millis();

void loop() {
  int SensorOneReading = readColors(0);
  int SensorTwoReading = readColors(1);
  if (SensorOneReading == RED && SensorTwoReading == RED) {
    Serial.write(BOTH_RED);
  } else if (SensorOneReading == GREEN && SensorTwoReading == GREEN) {
    Serial.write(BOTH_GREEN);
  } else if (SensorOneReading == GREEN && SensorTwoReading == NONE) {
    Serial.write(LEFT_GREEN);
  } else if (SensorOneReading == NONE && SensorTwoReading == GREEN) {
    Serial.write(RIGHT_GREEN);
  } else if (SensorOneReading == RED && SensorTwoReading == NONE) {
    Serial.write(LEFT_RED);
  } else if (SensorOneReading == NONE && SensorTwoReading == RED) {
    Serial.write(RIGHT_RED);
  } else {
    Serial.write(BOTH_NONE);
  }
  chooseBus(0);
  float left = leftTOF();
  float front = frontTOF();
  Serial.print("\t\t");
  Serial.print(left);
  Serial.print("\t");
  Serial.print(front);
  Serial.print("\t");
  Serial.print(digitalRead(26));
  Serial.print(digitalRead(28));
  Serial.print(digitalRead(30));
  Serial.print("\t Itr/s: ");
  float dt = millis() - t;
  Serial.print(1/(dt/1000));
  t = millis();
  Serial.println(" ");
}


void initColorSensors() {
  for (int i = 0; i < 2; i++) {
    Serial.println(i);
    chooseBus(i+1);
    if (tcs[i].begin()) {
      Serial.print("Found sensor ");
      Serial.println(i + 1);
    } else {
      // Serial.println("No Sensor Found");
      // while (true)
      //   ;
    }
  }
}

int readColors(byte sensorNum) {
  chooseBus(sensorNum+1);
  float r, g, b;
  // tcs[sensorNum].getRGB(&r, &g, &b);
  tcs[sensorNum].getRGB(&r, &g, &b);
  Serial.print(sensorNum);
  Serial.print(": ");
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print("\t");
  Serial.print("G: ");
  Serial.print(g);
  Serial.print("\t");
  Serial.print("B: ");
  Serial.print(b);
  Serial.print("\t");
  // Serial.println();
  if (g - r > 20 && g - b > 20 ) {
    return GREEN;
  } else if (r - g > 20 && r - b > 20) {
    return RED;
  } else {
    return NONE;
  }
  // if ((g / r) >= 1.2 && (g / b) >= 1.3) {
  //   // 1.28, 1.38
  //   return GREEN;
  // } else if ((r / g) >= 1.1 && (r / b) >= 1.2) {
  //   // 1.17, 1.21
  //   return RED;
  // } else {
  //   return NONE;
  // }
}

void chooseBus(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}
