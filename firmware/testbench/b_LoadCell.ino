//Load Cell
#define DOUT 8
#define CLK 9

float maxWeight = 30.0;

HX711 scale(DOUT, CLK);

void cal1(){
  scale.set_scale();
  scale.tare();
}
void cal2(){
  float weight = getValue(inputString, ':', 1).toFloat();
  calibration_factor = scale.get_units(10)/weight;
  scale.set_scale(calibration_factor);
  save();
}

void calVal(){
  calibration_factor = getValue(inputString, ':', 1).toFloat();
  scale.set_scale(calibration_factor);
  save();
  Serial.println(calibration_factor);
}

void scaleSetup(){
  scale.set_scale(calibration_factor);
  //Serial.println(calibration_factor);
  scale.tare();
}

void getWeight(){
  tension = scale.get_units();
}

