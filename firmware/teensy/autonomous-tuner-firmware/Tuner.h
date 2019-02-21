#ifndef Tuner_h
#define Tuner_h

enum TuningTargets{STRING_G3, STRING_D4, STRING_A4, STRING_E5};

class Tuner
{
  public:
    Tuner(float* note, bool* noteAvailable, long* lastSampleTime, float* stepperSpeed);
    void setTuningTarget(TuningTargets target);
    void Compute(); 
  private:
    bool Done = false;
    float focusBand = 3;
    float FREQ_TARGET;
    bool *myNoteAvailable;
    float *myNote;
    float *myStepperSpeed;
    long *myLastSampleTime;
};


#endif
