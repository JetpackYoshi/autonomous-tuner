import SerialHandler as handl
import PyCmdMessenger

arduino = PyCmdMessenger.ArduinoBoard("COM25",baud_rate=115200)

commands = [["Setup_Tune",""],
            ["AckSetup","siic"],
            ["SetInstr","i"],
            ["AckInstr","ii"],
            ["SetMode","i"],
            ["AckMode","ii"],
            ["SetString","c"],
            ["AckStr","ic"],
            ["SetReady",""],
            ["AckReady","si"],
            ["Error","s"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

if __name__ == "__main__":
    tune = handl.SerHandler()
    while 1:
        if tune.tx == 1:
            c.send(tune.HandleUsrState())
        else:
            tune.HandleDevState(c.recieve())
            
            
            
        
    
    

