enum
{
  kAcknowledge,
  kSetTargetString
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kSetTargetString, OnSetTargetString);
  cmdMessenger.printLfCr();
}

void OnSetTargetString()
{
  String target = cmdMessenger.readStringArg();

  if (target == "E"){
    cmdMessenger.sendCmd(kAcknowledge, "Tuning E String");
    setTuningTarget(STRING_E5);
  }
  else if(target == "A"){
    cmdMessenger.sendCmd(kAcknowledge, "Tuning A String");
    setTuningTarget(STRING_A4);
  }
  else if(target == "D"){
    cmdMessenger.sendCmd(kAcknowledge, "Tuning D String");
    setTuningTarget(STRING_D4);
  }
  else if(target == "G"){
    cmdMessenger.sendCmd(kAcknowledge, "Tuning G String");
    setTuningTarget(STRING_G3);
  }else{
    cmdMessenger.sendCmd(kAcknowledge, "Unknown String");
  }
      
  
}
