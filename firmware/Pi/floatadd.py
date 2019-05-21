from subprocess import Popen, PIPE
from time import sleep
from datetime import datetime
import threading
import PyCmdMessenger
import queue

Sync = threading.Event()
Tune = threading.Event()

#arduino = PyCmdMessenger.ArduinoBoard("COM29",baud_rate=115200,int_bytes=4)
#arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200)
arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyAMA0",baud_rate=115200,int_bytes=4)



commands = [["kAcknowledge", "s"],
            ["kError", "s"],
            ["kFloatAddition", "ff"],
            ["kFloatAdditionResult", "ff"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)
msg = c.receive()
print(msg)

def sendrcv():
    c.send("kFloatAddition",86.2,13.8)
    msg = c.receive()
    print(msg)

def main():
    print("hello")
    while True:
        sendrcv()
    print("done")

if __name__ == '__main__':
    main()
