#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
#include <CmdMessenger.h>

//Hardware Serial//
#define SERIALPORT Serial1 

//USB Serial//
//#define SERIALPORT Serial

CmdMessenger cmdMessenger = CmdMessenger(SERIALPORT);
enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};
enum StepSizes{WHOLE,HALF,QUARTER,EIGHTH};
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
  SERIALPORT.begin(kSerialSpeed);
}

void loop() {
  cmdMessenger.feedinSerialData();
  performPolledRoutines();
}
