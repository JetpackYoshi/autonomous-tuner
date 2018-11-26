void setup() {
  Serial.begin(115200);
  pinMode(PB14, INPUT);

}

void loop() {
  Serial.println(digitalRead(PB14));

}
