#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
#include <CmdMessenger.h>

CmdMessenger cmdMessenger = CmdMessenger(Serial);
enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};
enum SystemStates{
      NOT_READY,
      READY,
      TUNING,
      DONE
    }systemState;

#define kSerialSpeed 115200

float note;
float filtered_note;
float prob;

int Error = 0;

bool RUN = true;

void setup() {
  audioSetup();
  motorSetup();
  scheduleRoutines();
  attachCommandCallbacks();
  Serial.begin(kSerialSpeed);
}

void loop() {
  cmdMessenger.feedinSerialData();
  performPolledRoutines();
}
