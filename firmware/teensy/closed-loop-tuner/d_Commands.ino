enum
{
  kAcknowledge,
  kSetTargetString,
  kBeginTuning,
  kToggleRawStream
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kSetTargetString, OnSetTargetString);
  cmdMessenger.attach(kBeginTuning, OnBeginTuning);
  cmdMessenger.attach(kToggleRawStream, toggleRawStream);
  cmdMessenger.printLfCr();
}

void toggleRawStream(){
  stream = !stream;
  if (stream){
    cmdMessenger.sendCmd(kAcknowledge, "Streaming On");
  }else{
    cmdMessenger.sendCmd(kAcknowledge, "Streaming Off");
  }
}

void OnBeginTuning(){
  tuningState = TUNING;
  cmdMessenger.sendCmd(kAcknowledge, "Tuning Start");
}

void OnSetTargetString()
{
  String target = cmdMessenger.readStringArg();

  if (target == "E"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: E String");
    setTuningTarget(STRING_E5);
  }
  else if(target == "A"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: A String");
    setTuningTarget(STRING_A4);
  }
  else if(target == "D"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: D String");
    setTuningTarget(STRING_D4);
  }
  else if(target == "G"){
    cmdMessenger.sendCmd(kAcknowledge, "Target: G String");
    setTuningTarget(STRING_G3);
  }else{
    cmdMessenger.sendCmd(kAcknowledge, "Unknown String");
  }
      
  
}
