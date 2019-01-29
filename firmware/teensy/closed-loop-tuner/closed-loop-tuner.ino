#include <AccelStepper.h>
#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define ANALOG_IN 0
#define kSerialSpeed 115200

float pitch;
float setPoint;

AccelStepper stepper(4, 7, 8, 9, 10);

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
  long startTime = micros();
  //int reading = analogRead(ANALOG_IN);
  //Serial.println(reading);
  //int val = map(reading, 0, 1023, -1000, 1000);
  if (notefreq1.available()) {
    noInterrupts();
    float note = notefreq1.read();
    float prob = notefreq1.probability();
    Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    int val = map(note, 100, 700, 100, 800);
    stepper.setSpeed(val);
    interrupts();
  }
  //stepper.setSpeed(800);
  //stepper.runSpeed();
  //Serial.println(micros()-startTime);
}

void runStepper(){
  stepper.runSpeed();
}
