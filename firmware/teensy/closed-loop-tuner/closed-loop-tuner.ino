#include <AccelStepper.h>
#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define NOTE_E5 659.3
#define NOTE_A4 440.0
#define NOTE_D4 293.7
#define NOTE_G3 196.0

#define ANALOG_IN 0
#define kSerialSpeed 115200

float pitch;
float setPoint;

#define PITCH_BUFFER_LENGTH 30
bool signalStates[PITCH_BUFFER_LENGTH];
bool pitchReady = false;
int put_idx = 0;

AccelStepper stepper(4, 7, 8, 9, 10);

AudioInputAnalog              adc1; //adc(A2);
AudioAnalyzeNoteFrequency     notefreq1;
AudioConnection               patchCord1(adc1,0, notefreq1,0);

IntervalTimer myTimer;

long lastSampleTime = 0;

void setup() {
  AudioMemory(30);
  notefreq1.begin(.15);
  
  stepper.setMaxSpeed(50);
  Serial.begin(kSerialSpeed);
  myTimer.begin(runStepper, 10);

  for (int i=0; i<PITCH_BUFFER_LENGTH; i++){
    signalStates[i] = false;
  }
}

void loop() {
  
  pitchReady = false;
  
  if (notefreq1.available()) {
    noInterrupts();
    lastSampleTime = millis();
    float note = notefreq1.read();
    float prob = notefreq1.probability();
    Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    int val = map(note, NOTE_A4-5, NOTE_A4+5, -50, 50);
    stepper.setSpeed(val);
    pitchReady = true;
    //Serial.println("PITCH IS PLAYING!");
    interrupts();
  }

  if ((millis()-lastSampleTime) > 500){
    //Serial.println("no pitch is playing!");
    stepper.setSpeed(0);
  }

  
  //stepper.setSpeed(800);
  //stepper.runSpeed();
  //Serial.println(micros()-startTime);
}

void runStepper(){
  //if (notefreq1.available()){
    stepper.runSpeed();
  //}
}
