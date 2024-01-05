#define BASE_SPEED 100
#define HIGH_SPEED 254
#define LOW_SPEED 50
#define SPEED_CHANGE 200
int sensorPins[5] = { 30, 28, 26, 24, 22 };  // left2 -- left1 -- middle -- right1 -- right2
float weights[5] = { -1, -0.5, 0, 0.5, 1 };

void setup() {
  pinMode(sensorPins[0], INPUT);
  pinMode(sensorPins[1], INPUT);
  pinMode(sensorPins[2], INPUT);
  pinMode(sensorPins[3], INPUT);
  pinMode(sensorPins[4], INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValues[5];
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
  Serial.print(sensorValues[0]);
  Serial.print(sensorValues[1]);
  Serial.print(sensorValues[2]);
  Serial.print(sensorValues[3]);
  Serial.print(sensorValues[4]);
  Serial.println();
}
