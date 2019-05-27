/*
 * autonomous-tuner-firmware
 * 
 * Runs the core functionality for the robotic violin tuner. The device recieves commands over serial
 * which are used to initialize the device and control the setpoint. The commands are also used to
 * retrieve critical information such as pitch telemetry and system state. Details on the valid commands
 * can be found in the 'd_Commands' tab and are all of the format: 
 *                                  [COMMAND ID],{OPR1},{OPR2},…,{OPRn};
 *                                  
 * This program is designed to work with the MP6500 Stepper Motor Driver, but can be easily modified to use others.
 * 
 * Circuit:
 *  On the MP6500
 *  enable -> 39
 *  MS1    -> 38
 *  MS2    -> 37
 * 
 *  Audio:
 *  Output of Amp -> A2
 * 
 *  If Using Hardware Serial:
 *  Pin 0 is RX
 *  Pin 1 is TX
 * 
 * This firmware was written to run on a Teensy 3.5 Microcontroller, but should work on any Teensy 3.x variant.
 * 
 * Initial Release 27 May 2019
 * by Yoshin Govender
 */
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
  systemState = READY;
}

void loop() {
  cmdMessenger.feedinSerialData();
  performPolledRoutines();
}
