#include <PID_v1.h>
//#include "myEnums.h"
//#define NOTE_E5 659.3
//#define NOTE_A4 440.0
//#define NOTE_D4 293.7
//#define NOTE_G3 196.0

TuningTargets tuningTarget = STRING_D4;

double Setpoint, Input, Output;
double p_Kp=300, p_Ki=0, p_Kd=0;
PID tuningPID(&Input, &Output, &Setpoint, p_Kp, p_Ki, p_Kd, DIRECT);

float FREQ_TARGET; // Current setpoint pitch
float FREQ_BAND = 50; // Defines detection window. Target Pitch +/= FREQ_BAND
float focusBand = 3;
float validBand = 0.5;

int inRangeCounter = 0; // Counts the number of times the measured pitch is in acceptable range
#define MaxConsecValid 20
int tuningFaultCounter = 0; // Increments if motor moves but pitch doesn't change
#define MaxTuningFault 20

AudioInputAnalog              adc1;
AudioAnalyzeNoteFrequency     notefreq1;
AudioConnection               patchCord1(adc1,0, notefreq1,0);

long lastSampleTime = 0;
bool noteAvailable = false;
bool stream = false;

void setTuningTarget(TuningTargets target){
  switch(target){
    case STRING_G3: {
      FREQ_TARGET = 196.0;
      FREQ_BAND = 50;
      break;
    }
    case STRING_D4: {
      FREQ_TARGET = 293.7;
      FREQ_BAND = 50;
      break;
    }
    case STRING_A4: {
      FREQ_TARGET = 440.0;
      FREQ_BAND = 50;
      break;
    }
    case STRING_E5: {
      FREQ_TARGET = 659.3;
      FREQ_BAND = 80;
      break;
    }
  }
  Setpoint = FREQ_TARGET;
}

void audioSetup(){
  AudioMemory(30);
  notefreq1.begin(.15);
  tuningPID.SetMode(AUTOMATIC);
  tuningPID.SetOutputLimits(-speedLimit,speedLimit);
  setTuningTarget(STRING_E5);
}
void detectPitch(){
  if (notefreq1.available()) {
    noteAvailable = true;
    
    lastSampleTime = millis();
    note = notefreq1.read();
    
    
    prob = notefreq1.probability();

    
  }else{
    noteAvailable = false;
  }
}

void tuneString(){
  if (noteAvailable){
    if (note>FREQ_TARGET-FREQ_BAND && note <FREQ_TARGET+FREQ_BAND){
      filtered_note = StepFilter(note);
      if (stream){
        SERIALPORT.printf("%3.3f", note);
        SERIALPORT.print(" ");
        SERIALPORT.printf("%3.3f\n", filtered_note);
      }
      Input = filtered_note; // Pass measured value to PID
      
      // Check Process Status
      if (filtered_note>FREQ_TARGET-validBand && filtered_note<FREQ_TARGET+validBand){
          inRangeCounter = inRangeCounter + 1;
      }else{
          inRangeCounter = 0;
      }
      noInterrupts();
      stepperSpeed = map(filtered_note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -speedLimit, speedLimit);
//      stepperSpeed = -Output; // Use PID Controller
//      Serial.printf("%3.3f\n", -Output);
      interrupts();
    }
  }
  if ((millis()-lastSampleTime) > 100){
    noInterrupts();
    stepperSpeed = 0;
    interrupts();
  }
}

void runStateMachine(){
  switch(systemState){
    case NOT_READY: {
      break;
    }
    case READY:{
      break;
    }
    case TUNING:{
      tuneString();
      if (inRangeCounter > MaxConsecValid){
        systemState = DONE;
        inRangeCounter = 0;
        stopMotor();
//        SERIALPORT.println("Done");
//        cmdMessenger.sendCmd(kAcknowledge, "Done");
      }
      break;
    }
    case DONE:{
      break;
    }
  }
}
