// #define BASE_SPEED 100
// #define HIGH_SPEED 254
// #define LOW_SPEED 50
// #define SPEED_CHANGE 200
#define P0 53
#define P1 51
#define P2 49
#define P3 47
#define P4 45
#define P5 A0
#define P6 43
#define P7 41
#define P8 39
#define P9 37
#define P10 35
// int sensorPins[11]{ 53, 51, 49, 47, 45, A0, 43, 41, 39, 37, 35 };

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
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  // pinMode(P5, INPUT);
  // pinMode(P6, INPUT);
  Serial.begin(115200);
}
#define SILVER_1()  ((PIND & _BV(PD2)) ? 0 : 1)  // PD2 corresponds to digital pin 2
#define SILVER_2()  ((PIND & _BV(PD3)) ? 0 : 1)  // PD3 corresponds to digital pin 3
#define SILVER_3()  ((PING & _BV(PG5)) ? 0 : 1)

// int reading(int analog) {
//   return analog ;//> 100 ? 1 : 0;
// }

void loop() {
  Serial.print(digitalRead(P1));
  Serial.print("\t");
  Serial.print(digitalRead(P2));
  Serial.print("\t");
  Serial.print(digitalRead(P3));
  Serial.print("\t");
  Serial.print(digitalRead(P4));
  Serial.print("\t");
  Serial.print(digitalRead(P0));
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

  Serial.print(digitalRead(36));
  Serial.print(digitalRead(2));
  Serial.print(digitalRead(3));
  Serial.print(digitalRead(4));
  Serial.print(digitalRead(38));
  // Serial.print(SILVER_1());
  // Serial.print(SILVER_2());
  // Serial.print(SILVER_3());
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
