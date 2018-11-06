void cmd(){
  if (stringComplete) {
    if (inputString.startsWith("TENS?",0)){
      Serial.println(tension, 8);
    }
    else if(inputString.startsWith("TARE",0)){
      scale.tare();
    }
    else if(inputString.startsWith("CAL1",0)){
      cal1();
    }
    else if(inputString.startsWith("CAL2",0)){
      cal2();
    }
    else if(inputString.startsWith("CALSET",0)){
      calVal();
    }
    else if(inputString.startsWith("CAL?",0)){
      Serial.println(calibration_factor);
    }
    else if(inputString.startsWith("POS",0)){
      stepper.runToNewPosition(getValue(inputString, ':', 1).toInt());
      Serial.println(stepper.currentPosition());
    }
    else if(inputString.startsWith("POS?",0)){
      Serial.println(stepper.currentPosition());
    }
    else if(inputString.startsWith("STOP",0)){
      stepper.stop();
    }
    
    inputString = "";
    stringComplete = false;
    
  }
}

//void CMD(){
//  if (stringComplete){
//    String command = getValue(inputString, ':', 0);
//    switch (command) {
//      case "TARE":
//        scale.tare();
//        break;
//      case "TENS":
//        Serial.println(tension, 8);
//        break;
//      case "CAL1":
//        cal1();
//        break;
//      case "CAL2":
//        cal2();
//        break;
//      case "CALSET":
//        calVal();
//        break;
//      case "CAL?":
//        Serial.println(calibration_factor);
//        break;
//    }
//    inputString = "";
//    stringComplete = false;
//  }
//}

