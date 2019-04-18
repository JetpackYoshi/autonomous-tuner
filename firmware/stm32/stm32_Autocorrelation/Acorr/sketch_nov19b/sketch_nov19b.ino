int soundPin = PA6;
int incomingByte = 0;
void setup()
{
  Serial.begin(115200);
}
 
void loop()
{
    // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
        }
}
