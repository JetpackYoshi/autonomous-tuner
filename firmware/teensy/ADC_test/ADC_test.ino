int sum = 0;

void setup() {
  Serial.begin(115200);
  

}

void loop() {
  sum = 0;
  for(int i=0; i<100; i++){
    sum += analogRead(A2);
  }
  //reading  = analogRead(A2);
  Serial.println(sum/100);
  //delay(200);
}
