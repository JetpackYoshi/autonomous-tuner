#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};

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
