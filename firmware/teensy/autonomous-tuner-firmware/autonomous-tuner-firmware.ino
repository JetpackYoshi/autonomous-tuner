#include <AccelStepper.h>
#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define NOTE_E5 659.3
#define NOTE_A4 440.0
#define NOTE_D4 293.7
#define NOTE_G3 196.0

float tuningTarget = NOTE_A4;

//Serial Communication
#define kSerialSpeed 115200
AudioInputAnalog              adc1; //adc(A2);
AudioAnalyzeNoteFrequency     notefreq1;
AudioConnection               patchCord1(adc1,0, notefreq1,0);

IntervalTimer myTimer;

void setup() {
  AudioMemory(30);
  notefreq1.begin(.15);
  
  stepper.setMaxSpeed(1000);
  Serial.begin(kSerialSpeed);
  myTimer.begin(runStepper, 10);
}

void loop() {
  if (notefreq1.available()) {
    noInterrupts();
    float note = notefreq1.read();
    float prob = notefreq1.probability();
    Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    int val = map(note, NOTE_A4-5, NOTE_A4+5, -500, 500);
    stepper.setSpeed(val);
    interrupts();
  }
}

void runStepper(){
  stepper.runSpeed();
}
