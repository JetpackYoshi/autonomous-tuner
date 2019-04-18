//#include "AltoSaxVib_C4.h"
//#include "Piano_C4_Clean.h"
//#include "Guitar_C5.h"
#include "C4.h"

// Sample Frequency in Hz
float sample_freq = 10560;

int len = sizeof(rawData);
int i,k;
long sum, sum_old;
int thresh = 0;
int calFreq = 440;
float calPeriod = 1.0/440.0;
float freq_per = 0;
byte pd_state = 0;
int soundPin = PA6;
String incomingString;
int sound;
unsigned int raw[200] = {};
int len1 = (sizeof(raw) / sizeof(raw[0]));
void setup() {
  
  Serial.begin(2000000);
  
  sum = 0;
  pd_state = 0;
  int period = 0;
  for(i=0; i < len; i++)
  {
    // Autocorrelation
    sum_old = sum;
    sum = 0;
    for(k=0; k < len-i; k++) sum += (rawData[k]-128)*(rawData[k+i]-128)/256;
//    RX8 [h=43] @1Key1 @0Key1
//    Serial.print("C");
//    Serial.write((rawData[i]-128)>>8);
//    Serial.write((rawData[i]-128)&0xff); 
    
  /*  // RX8 [h=43] @1Key1 @0Key1
    Serial.print("C");
    Serial.write(sum>>8);
    Serial.write(sum&0xff); */
    
    // Peak Detect State Machine
    if (pd_state == 2 && (sum-sum_old) <=0) 
    {
      period = i;
      pd_state = 3;
    }
    if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) pd_state = 2;
    if (!i) {
      thresh = sum * 0.5;
      Serial.println(thresh);
      pd_state = 1;
    }
  }
  // Frequency identified in kHz
  freq_per = sample_freq/period;
  Serial.println(freq_per);
  Serial.println(thresh);
  }

void loop() {
if (Serial.available() > 0) {
  Serial.println("Recording");
  Serial.println(len1);
  // read the incoming byte:
  incomingString = Serial.readString();
//  Serial.println(incomingChar);
  
  if (incomingString[0] == 'c') {
    Serial.print("Calibrating to ");
    if (incomingString.length() > 1){
      calFreq = (incomingString.substring(1)).toInt();
      Serial.print(calFreq);
      Serial.println("Hz");
      calPeriod = 1.0/calFreq;
    }
    else {
      Serial.println("440Hz");
      calPeriod = 1.0/440.0;
    }
    

    for(i=0; i< len1; i++)
    {
      raw[i] = (analogRead(soundPin));
//      Serial.println(raw[i]);
    }
    sum = 0;
    pd_state = 0;
    int period = 0;
    for(i=0; i < len1; i++)
    {
      // Autocorrelation
      sum_old = sum;
      sum = 0;
      for(k=0; k < len1-i; k++) sum += (raw[k]-2048)*(raw[k+i]-2048)/4096;
      
      // Peak Detect State Machine
      if (pd_state == 2 && (sum-sum_old) <=0) 
      {
        period = i;
        pd_state = 3;
      }
      if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) pd_state = 2;
      if (!i) {
        thresh = sum * 0.5;
//        Serial.print("thresh is ");
//        Serial.println(thresh);
        pd_state = 1;
      }
    }
    // Frequency identified in Hz
    
    sample_freq = period/calPeriod;
    Serial.print("Setting samp freq to: ");
    Serial.println(sample_freq);
    Serial.print("Calibration Period: ");
    Serial.println(calPeriod, 10);
    Serial.print("Period: ");
    Serial.println(period);
  }
  else {
  Serial.print("Start Time: ");
  int sttime = millis();
  Serial.println(sttime);
  for(i=0; i<len1; i++)
    {
//      long smoothsum = 0;
//      for(int j=0; j<100; j++)
//      {
//          smoothsum += analogRead(soundPin);
//      }
//      raw[i] = (smoothsum/100);
      raw[i] = (analogRead(soundPin));
//      Serial.println(raw[i]);
    }
  sum = 0;
  pd_state = 0;
  int period = 0;
  for(i=0; i < len1; i++)
  {
    // Autocorrelation
//    Serial.println(sum);
    sum_old = sum;
//    Serial.println(sum_old);
    sum = 0;
    for(k=0; k < len1-i; k++) sum += (raw[k]-2048)*(raw[k+i]-2048)/4096;
    
    // Peak Detect State Machine
//    Serial.println(pd_state);
    if (pd_state == 2 && (sum-sum_old) <=0) 
    {
      period = i;
//      Serial.println(i);
      pd_state = 3;
    }
    if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) pd_state = 2;
    if (!i) {
      thresh = sum * 0.5;
//      Serial.print("thresh is ");
//      Serial.println(thresh);
      pd_state = 1;
    }
  }
  // Frequency identified in Hz
  Serial.println(sample_freq);
  Serial.println(period);
  freq_per = sample_freq/period;
  Serial.println(freq_per);
  Serial.print("End Time: ");
  Serial.println(millis() - sttime);
}
}
}

