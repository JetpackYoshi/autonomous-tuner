#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define kSerialSpeed 115200

float note;
float prob;

bool RUN = true;

void setup() {
  audioSetup();
  motorSetup();
  scheduleRoutines();
  
  Serial.begin(kSerialSpeed);
  Serial.println("Begin Tuning A");
}

void loop() {
  performPolledRoutines();
}
