// #define BASE_SPEED 100
// #define HIGH_SPEED 254
// #define LOW_SPEED 50
// #define SPEED_CHANGE 200z
#define P0 A0
#define P1 A1
#define P2 A2
#define P3 A3
#define P4 A4
#define P5 52
#define OBSTACLE 22
#define LEFT_OBSTACLE 39
#define RIGHT_OBSTACLE 52
#define TILT 53
// float weights[5] = { -1, -0.5, 0, 0.5, 1 };

void setup() {
  pinMode(P0, INPUT);
  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  pinMode(P4, INPUT);
  // pinMode(P5, INPUT);
  // pinMode(P6, INPUT);
  Serial.begin(9600);
}

int reading(int analog) {
  return analog ;//> 100 ? 1 : 0;
}

void loop() {
  Serial.print(reading(analogRead(P0)));
  Serial.print("\t");
  Serial.print(reading(analogRead(P1)));
  Serial.print("\t");
  Serial.print(reading(analogRead(P2)));
  Serial.print("\t");
  Serial.print(reading(analogRead(P3)));
  Serial.print("\t");
  Serial.print(reading(analogRead(P4)));
  Serial.print("\t\t\t");
  // Serial.print(digitalRead(P5));
  Serial.print(digitalRead(LEFT_OBSTACLE));
  Serial.print("\t");
  Serial.print(digitalRead(OBSTACLE));
  Serial.print("\t");
  Serial.print(digitalRead(RIGHT_OBSTACLE));
  Serial.print("\t");
  Serial.print(digitalRead(TILT));
  // Serial.print("\t");
  // Serial.print(sensorValues[6);
  Serial.println();
}
