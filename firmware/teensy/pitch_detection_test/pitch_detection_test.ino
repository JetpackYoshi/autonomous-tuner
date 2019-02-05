// Test by Yoshin Govender
// To use, test with a MIC circuit or a pre-built IC, and a teensy 3.5
// Signal range should be 0 to 1.2V
//MIC               TEENSY
//VCC <------------> 3.3V
//GND <------------> GND
//OUT <------------> D16 (A2)

#include <Audio.h>
#include <SPI.h>
#include <Wire.h>

#define kSerialSpeed 115200

AudioInputAnalog              adc1; //adc(A2);
AudioAnalyzeNoteFrequency     notefreq1;

AudioConnection               patchCord1(adc1,0, notefreq1,0);

void setup() {
  AudioMemory(30);
  notefreq1.begin(.15);
  Serial.begin(kSerialSpeed);
}

void loop() {

  if (notefreq1.available()) {
    float note = notefreq1.read();
    float prob = notefreq1.probability();
    Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
  }
//  else{
//    Serial.println("no data");
//    delay(500);
//  }
  

}
