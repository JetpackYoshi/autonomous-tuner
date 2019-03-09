#include <PID_v1.h>
//#include "myEnums.h"
//#define NOTE_E5 659.3
//#define NOTE_A4 440.0
//#define NOTE_D4 293.7
//#define NOTE_G3 196.0

//enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};
TuningTargets tuningTarget = STRING_D4;

double Setpoint, Input, Output;
double p_Kp=2, p_Ki=5, p_Kd=1;
PID tuningPID(&Input, &Output, &Setpoint, p_Kp, p_Ki, p_Kd, DIRECT);

float FREQ_TARGET;
float FREQ_BAND = 50;
float focusBand = 3;
int inRangeCounter = 0;
#define MaxConsecValid 20

AudioInputAnalog              adc1; //adc(A2);
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
}

void audioSetup(){
  AudioMemory(30);
  notefreq1.begin(.15);
  tuningPID.SetMode(AUTOMATIC);
  tuningPID.SetOutputLimits(-speedLimit,speedLimit);
  setTuningTarget(STRING_E5);
}
void detectPitch(){
  //Serial.println("TEST");
  if (notefreq1.available()) {
    noteAvailable = true;
    
    lastSampleTime = millis();
    note = notefreq1.read();
    prob = notefreq1.probability();

    // Check Process Status
    if (note>FREQ_TARGET-0.5 && note<FREQ_TARGET+0.5){
        inRangeCounter = inRangeCounter + 1;
    }else{
        inRangeCounter = 0;
    }
    if (stream){
      Serial.printf("%3.2f\n", note);
    }
    //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    
  }else{
    noteAvailable = false;
  }
}

void tuneString(){
  if (noteAvailable){
    if (note>FREQ_TARGET-FREQ_BAND && note <FREQ_TARGET+FREQ_BAND){
      Serial.printf("%3.2f\n", note);
      noInterrupts();
      stepperSpeed = map(note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -speedLimit, speedLimit);
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
  switch(tuningState){
    case NOT_READY: {
      break;
    }
    case READY:{
      break;
    }
    case TUNING:{
      tuneString();
      //Serial.println(inRangeCounter);
      if (inRangeCounter > MaxConsecValid){
        tuningState = DONE;
        //cmdMessenger.sendCmd(kAcknowledge, "Done");
        noInterrupts();
        stepperSpeed = 0;
        interrupts();
        Serial.println("Done");
      }
      break;
    }
    case DONE:{
      break;
    }
  }
}
