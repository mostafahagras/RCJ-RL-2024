char message[2];

uint8_t readSerial() {
  Serial3.readBytes(message, 1);
  Serial.print("Recieved: ");
  Serial.println(message);
  return message[0];
}

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  Serial3.write("T");
  Serial.print("Sent T\t");
  readSerial();
}
