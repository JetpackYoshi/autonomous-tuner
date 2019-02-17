#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
#include <CmdMessenger.h>

CmdMessenger cmdMessenger = CmdMessenger(Serial);
enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};

#define kSerialSpeed 115200

float note;
float prob;

bool RUN = true;

void setup() {
  audioSetup();
  motorSetup();
  scheduleRoutines();
  attachCommandCallbacks();

  
  Serial.begin(kSerialSpeed);
  Serial.println("Begin Tuning A");
}

void loop() {
  cmdMessenger.feedinSerialData();
  performPolledRoutines();
}
