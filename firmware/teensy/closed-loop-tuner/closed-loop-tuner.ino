#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define kSerialSpeed 115200

float note;
float prob;

void setup() {
  audioSetup();
  motorSetup();
  scheduleRoutines();
  
  Serial.begin(kSerialSpeed);
}

void loop() {
  performPolledRoutines();
}
