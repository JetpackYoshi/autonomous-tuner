#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Tuner.h"

Tuner::Tuner(float* note, long* lastSampleTime, float* stepperSpeed)
{
  myNote = note;
  myStepperSpeed = stepperSpeed;
  myLastSampleTime = lastSampleTime;
}

void Tuner::setTuningTarget(TuningTargets target)
{
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

void Tuner::Compute()
{
  if(!Done){
    if (myNoteAvailable){
      if (note>FREQ_TARGET-50 && note <FREQ_TARGET+50){
        noInterrupts();
        myStepperSpeed = map(note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -300, 300);
        interrupts();
      }
    }
    if ((millis()-myLastSampleTime) > 100){
      noInterrupts();
      myStepperSpeed = 0;
      interrupts();
    }
  }
}

void Tuner::StateMachineRun()
{
  switch(tuningState)
  {
    case NOT_READY:
      break;
    case READY:
      break;
    case TUNING:
      Compute();
      if (note>FREQ_TARGET-1.0 && note<FREQ_TARGET+1){
        tuningState = DONE;
      }
      break;
    case DONE:
      break;
  }
}
