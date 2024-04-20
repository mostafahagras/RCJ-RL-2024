// #define BASE_SPEED 100
// #define HIGH_SPEED 254
// #define LOW_SPEED 50
// #define SPEED_CHANGE 200z
#define P0 53
#define P1 51
#define P2 49
#define P3 47
#define P4 45
#define P5 A0
#define P6 39
#define P7 A1
#define P8 A2
#define P9 A3
#define P10 A4

// #define OBSTACLE 22
// #define LEFT_OBSTACLE 39
// #define RIGHT_OBSTACLE 52
// #define TILT 53
// float weights[5] = { -1, -0.5, 0, 0.5, 1 };

void setup() {
  pinMode(P0, INPUT);
  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  pinMode(P4, INPUT);
  pinMode(P5, INPUT);
  pinMode(P6, INPUT);
  pinMode(P7, INPUT);
  pinMode(P8, INPUT);
  pinMode(P9, INPUT);
  pinMode(P10, INPUT);
  // pinMode(P5, INPUT);
  // pinMode(P6, INPUT);
  Serial.begin(9600);
}

// int reading(int analog) {
//   return analog ;//> 100 ? 1 : 0;
// }

void loop() {
  Serial.print(digitalRead(P0));
  Serial.print("\t");
  Serial.print(digitalRead(P1));
  Serial.print("\t");
  Serial.print(digitalRead(P2));
  Serial.print("\t");
  Serial.print(digitalRead(P3));
  Serial.print("\t");
  Serial.print(digitalRead(P4));
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(analogRead(P5));
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(digitalRead(P6));
  Serial.print("\t");
  Serial.print(digitalRead(P7));
  Serial.print("\t");
  Serial.print(digitalRead(P8));
  Serial.print("\t");
  Serial.print(digitalRead(P9));
  Serial.print("\t");
  Serial.print(digitalRead(P10));
  // Serial.print("\t\t\t");
  // Serial.print(digitalRead(P5));
  // Serial.print(digitalRead(LEFT_OBSTACLE));
  // Serial.print("\t");
  // Serial.print(digitalRead(OBSTACLE));
  // Serial.print("\t");
  // Serial.print(digitalRead(RIGHT_OBSTACLE));
  // Serial.print("\t");
  // Serial.print(digitalRead(TILT));
  // Serial.print("\t");
  // Serial.print(sensorValues[6);
  Serial.println();
}
