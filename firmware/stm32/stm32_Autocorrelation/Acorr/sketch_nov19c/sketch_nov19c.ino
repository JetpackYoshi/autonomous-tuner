int soundPin = PA6;
 
void setup()
{
  Serial.begin(2000000);
}
 
void loop()
{
    long sum = 0;
    for(int i=0; i<100; i++)
    {
//        Serial.println(analogRead(soundPin));
        sum += analogRead(soundPin);
    }
//    Serial.println(sum);
    sum = sum/100;
 
    Serial.println(sum);
 
    delay(10);
}
