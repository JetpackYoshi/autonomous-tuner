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
float focusBand = 3;

AudioInputAnalog              adc1; //adc(A2);
AudioAnalyzeNoteFrequency     notefreq1;
AudioConnection               patchCord1(adc1,0, notefreq1,0);

long lastSampleTime = 0;

void setTuningTarget(TuningTargets target){
  switch(target){
    case STRING_G3: {
      FREQ_TARGET = 196.0;
      break;
    }
    case STRING_D4: {
      FREQ_TARGET = 293.7;
      break;
    }
    case STRING_A4: {
      FREQ_TARGET = 440.0;
      break;
    }
    case STRING_E5: {
      FREQ_TARGET = 659.3;
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
    
    lastSampleTime = millis();
    note = notefreq1.read();
    prob = notefreq1.probability();
    //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    
    if (note>FREQ_TARGET-50 && note <FREQ_TARGET+50){
      Serial.printf("%3.2f\n", note);
      noInterrupts();
      stepperSpeed = map(note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -speedLimit, speedLimit);
      interrupts();
    }
    
  }

  if ((millis()-lastSampleTime) > 250){
   stepperSpeed = 0;
  }
}

void tuneString(){
  if (notefreq1.available()) {
    if (note>400 && note <500){
      Serial.printf("%3.2f\n", note);
      noInterrupts();
      stepperSpeed = map(note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -300, 300);
      interrupts();
    }
    if ((millis()-lastSampleTime) > 250){
      stepperSpeed = 0;
    }
  }
}
