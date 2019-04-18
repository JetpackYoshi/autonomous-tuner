import sys

InstLookup = {
        0 : "violin",
        1 : "mayonnaise"
        }

class SerHandler: 
    
    #The "MAKENULL" function, will generate empty hash table of specified length
    def __init__(self, mode = 0):
        if mode == 0:
            self.tx = 1
            self.UsrStr = 'A'
            self.DevStr = []
            
            self.UsrMode = 0
            self.DevMode = []
            
            self.UsrInstr = 0
            self.DevInstr = []
            
            self.UsrState = "idle"
            self.DevState = []
            
            self.UsrCmd = "ask"
            
            self.StrAck = 0
            self.ModeAck = 0
            self.InstrAck = 0
            self.StateAck = 0
    
    def GetInstrument(self, InstIdx):
        return InstLookup.get(InstIdx)
        
    def HandleUsrState(self):
        while self.UsrState == "idle":
            if self.UsrCmd == "tune":
                self.UsrState = "Setup"
                self.tx = 0
                return "Setup_Tune"
        
        if self.UsrState == "Setup":
            if self.StateAck:
                if self.InstrAck:
                    if self.ModeAck:
                        if self.StrAck:
                            self.UsrState = "Ready2Tune"
                            self.tx = 0
                            return "SetReady"
                        else:
                            self.tx = 0
                            return "SetString", self.UsrStr
                    else:
                        self.tx = 0
                        return "SetMode", self.UsrMode
                else:
                    self.tx = 0
                    return "SetInstr", self.UsrInstr
            else:
                print("whut did you even do")
            
            
    def HandleDevState(self, msg):
        if msg[0] == "AckSetup":
            self.DevState = (msg[1])[0]
            self.DevInstr = (msg[1])[1]
            self.DevMode = (msg[1])[2]
            self.DevStr = (msg[1])[3]
            
            if self.StateStr == self.DevStr:
                self.StateAck = 1
            if self.InstrStr == self.DevStr:
                self.InstrAck = 1
            if self.ModeStr == self.DevStr:
                self.ModeAck = 1
            if self.UsrStr == self.DevStr:
                self.StrAck = 1
            self.tx = 1
            
        elif msg[0] == "AckInst":
            self.InstrAck = (msg[1])[0]
            self.DevInstr = (msg[1])[1]
            self.tx = 1
            
        elif msg[0] == "AckMode":
            self.ModeAck = (msg[1])[0]
            self.DevMode = (msg[1])[1]
            self.tx = 1
        
        elif msg[0] == "AckStr":
            self.StrAck = (msg[1])[0]
            self.DevStr = (msg[1])[1]
            self.tx = 1
        
        elif msg[0] == "AckReady":
            self.DevState = "tuning"
            self.UsrState = "plotting"
            self.tx = 0
            
        
        elif msg[0] == "Error":
            print("Error from device: " + (msg[1])[0])
        
        else:
            print("Error: Command from device not recognized")
        
        
            
        
    
    