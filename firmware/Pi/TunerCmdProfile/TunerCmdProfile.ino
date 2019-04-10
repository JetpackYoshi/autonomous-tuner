#include <CmdMessenger.h>  // CmdMessenger

CmdMessenger cmdMessenger = CmdMessenger(Serial);


bool Setup = false;
bool Ready = false;
bool Tune = false;

enum
{
  InitTune,
  kInitTune,
  SetInst,
  kSetInst,
  SetMod,
  kSetMod,
  SetString,
  kSetString,
  SetReady,
  kSetReady,
  kError // Command to report errors
};

bool Setup = false;
int inst = 0;
int mode = 0;
char str = 'A';



void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(InitTune, OnInitTune);
  cmdMessenger.attach(SetInst, OnSetInst);
  cmdMessenger.attach(SetMod, OnSetMod);
  cmdMessenger.attach(SetStr, OnSetStr);
  cmdMessenger.attach(SetReady, OnSetReady);
}


void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError,"Command without attached callback");
}

void OnInitTune()
{
  Setup = true;
  cmdMessenger.sendCmdStart(kInitTune);
  cmdMessenger.sendCmdArg("Setup");
  cmdMessenger.sendCmdBinArg(inst);
  cmdMessenger.sendCmdBinArg(mode);
  cmdMessenger.sendCmdBinArg(str);
  cmdMessenger.sendCmdEnd();
}

void OnSetInst()
{
  inst = cmdMessenger.readBinArg();
  cmdMessenger.sendCmdStart(kSetInst);
  cmdMessenger.sendCmdBinArg(1);
  cmdMessenger.sendCmdBinArg(inst);
  cmdMessenger.sendCmdEnd();
}

void OnSetMod()
{
  mode = cmdMessenger.readBinArg();
  cmdMessenger.sendCmdStart(kSetMod);
  cmdMessenger.sendCmdBinArg(1);
  cmdMessenger.sendCmdBinArg(mode);
  cmdMessenger.sendCmdEnd();
}

void OnSetStr()
{
  str = cmdMessenger.readCharArg();
  cmdMessenger.sendCmdStart(kSetStr);
  cmdMessenger.sendCmdBinArg(1);
  cmdMessenger.sendCmdArg(str);
  cmdMessenger.sendCmdEnd();
}

void OnSetReady()
{
  Setup = false;
  Ready = true;
  cmdMessenger.sendCmdStart(kSetReady);
  cmdMessenger.sendCmdBinArg(1);
  cmdMessenger.sendCmdArg(Tuning);
  cmdMessenger.sendCmdEnd();
}

void setup() 
{
  // Listen on serial connection for messages from the pc
  Serial.begin(115200); 

  // Adds newline to every command
  cmdMessenger.printLfCr();   

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();

  // Send the status to the PC that says the Arduino has booted
  cmdMessenger.sendCmd(kAcknowledge,"Arduino has started!");
}

void loop() {
  cmdMessenger.feedinSerialData(); 

}
