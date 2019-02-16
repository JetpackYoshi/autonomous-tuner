#define NOTE_E5 659.3
#define NOTE_A4 440.0
#define NOTE_D4 293.7
#define NOTE_G3 196.0

float FREQ_TARGET = NOTE_A4;
float focusBand = 3;

AudioInputAnalog              adc1; //adc(A2);
AudioAnalyzeNoteFrequency     notefreq1;
AudioConnection               patchCord1(adc1,0, notefreq1,0);

long lastSampleTime = 0;

void audioSetup(){
  AudioMemory(30);
  notefreq1.begin(.15);
}
void detectPitch(){
  //Serial.println("TEST");
  if (notefreq1.available()) {
    
    noInterrupts();
    lastSampleTime = millis();
    note = notefreq1.read();
    prob = notefreq1.probability();
    //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);

    
    if (note>400 && note <500){
      Serial.printf("%3.2f\n", note);
      stepperSpeed = map(note, FREQ_TARGET-focusBand, FREQ_TARGET+focusBand, -300, 300);
    }
    interrupts();
  }

  if ((millis()-lastSampleTime) > 250){
   stepperSpeed = 0;
  }
}
