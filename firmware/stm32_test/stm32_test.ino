void setup() {
  Serial.begin(115200);
  pinMode(PB1, INPUT_ANALOG);

}

void loop() {
  Serial.println(analogRead(PB1));
  delay(200);
}
