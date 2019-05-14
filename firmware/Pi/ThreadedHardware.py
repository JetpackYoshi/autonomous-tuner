from subprocess import Popen, PIPE
from time import sleep
from datetime import datetime
import board
import digitalio
import adafruit_character_lcd.character_lcd as characterlcd
import threading
import PyCmdMessenger
import queue
import RPi.GPIO as GPIO

lcd_columns = 16
lcd_rows = 2

#lcd_rs = digitalio.DigitalInOut(board.D22)
#lcd_en = digitalio.DigitalInOut(board.D17)
#lcd_d4 = digitalio.DigitalInOut(board.D25)
#lcd_d5 = digitalio.DigitalInOut(board.D24)
#lcd_d6 = digitalio.DigitalInOut(board.D23)
#lcd_d7 = digitalio.DigitalInOut(board.D18)

lcd_rs = digitalio.DigitalInOut(board.D20)
lcd_en = digitalio.DigitalInOut(board.D16)
lcd_d4 = digitalio.DigitalInOut(board.D7)
lcd_d5 = digitalio.DigitalInOut(board.D8)
lcd_d6 = digitalio.DigitalInOut(board.D25)
lcd_d7 = digitalio.DigitalInOut(board.D24)


lcd = characterlcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, lcd_columns, lcd_rows)

lcd.clear()

lcd_line_1 = "Initializing "
lcd_line_2 = "Program"

lcd.message = lcd_line_1 + "\n" + lcd_line_2

GPIO.setmode(GPIO.BCM)
#GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_UP)  
#GPIO.setup(5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
#GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_UP)
#GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(19, GPIO.IN, pull_up_down=GPIO.PUD_UP)  
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(5, GPIO.IN, pull_up_down=GPIO.PUD_UP)

Up_E = threading.Event()
Down_E = threading.Event()
Select_E = threading.Event()
Back_E = threading.Event()

Sync = threading.Event()
Tune = threading.Event()
Stahp = threading.Event()
Done = threading.Event()

arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyAMA0",baud_rate=115200,int_bytes=4)
#arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200,int_bytes=4)

commands = [["GetStat",""],
            ["RcvStat","ii"],
            ["GetPitch",""],
            ["RcvPitch","iiff"],
            ["SetTarget","ff"],
            ["AckTarget","iiff"],
            ["InitTune",""],
            ["AckTune","iis"],
            ["StopTune",""],
            ["AckStop","iis"],
            ["Calibrate","ii"],
            ["AckCal","iiii"],
            ["Stream",""],
            ["AckStream","iis"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

class Update(threading.Thread):
    def __init__(self, Sync, Tune, Freq, Params, Back, Stahp, Done):
        threading.Thread.__init__(self)
        
        self.tune = Tune
        self.sync = Sync
        self.freq = Freq
        self.Params = Params
        self.params = []
        self.back = Back
        self.Stahp = Stahp
        self.Done = Done
        self.msg = []
        
        
    def run(self):
        c.send("GetStat")
        while True:
#            sleep(1)
            self.msg = c.receive()
            print(self.msg)
            
#            with self.slock:
#                self.state = (self.msg[1])[0]
                
            if self.tune.is_set():
                if self.Stahp.is_set():
                    self.tune.clear()
                    self.Stahp.clear()
                    c.send("StopTune")
                    print("sent StopTune Cancelled")
                else:  
                    if ((self.msg[1])[0]) == 1:
                        print("tune raised")
                        c.send("InitTune")
                        print("sent InitTune")
                    elif ((self.msg[1])[0]) == 2:
                        if self.msg[0] == "RcvPitch":
                            if not self.freq.qsize():
                                self.freq.put(((self.msg[1])[3]))
                            print((self.msg[1])[2])
                        c.send("GetPitch")
                        print("sent GetPitch")
                    elif ((self.msg[1])[0]) == 3:
                        self.tune.clear()
                        self.Done.set()
                        print("done")
                        c.send("StopTune")
                        print("sent StopTune Done")
                    
            elif self.sync.is_set():
                if self.msg[0] == "AckTarget":
                    self.sync.clear()
                    self.tune.set()
                    c.send("GetStat")
                    print("sent GetStat")
                else:
                    print("sync raised")
                    self.params = self.Params.get()
                    print(self.params)
                    print("sent SetTarget " + str(self.params[0]) + ", " + str(self.params[1]))
                    c.send("SetTarget", self.params[0], self.params[1])
#                    print("sent SetTarget " + self.params)
                    
            else:
                c.send("GetStat")
                print("sent GetStat")
                        
            
class LCD(threading.Thread):
    def __init__(self, Freq, Params, Sync, Up, Down, Select, Back, Stahp, Done):
       threading.Thread.__init__(self)
       
       self.Sync = Sync
       self.Up = Up
       self.Down = Down
       self.Select = Select
       self.Back = Back
       self.Stahp = Stahp
       self.Done = Done
       
       self.MenuNum = 0
       self.Menu1IDX = 0
       self.Menu2IDX = 0
       self.idx = 0
       self.idx2 = 0
       self.Menu =  [("Violin", [("E", 659.3, 80.0), ("A", 440.0, 50.0), ("D", 293.7, 50.0), ("G", 196.0, 50.0)]),
                     ("Viola", [("A", 440.0, 50.0), ("D", 293.7, 50.0), ("G", 196.0, 50.0), ("C", 130.8, 50.0)]),
                     ("Cello", [("A", 220.0, 50.0), ("D", 146.8, 50.0), ("G", 98.0, 50.0), ("C", 65.41, 50.0)]),
                     ("Upright Bass", [("E", 41.2, 50.0), ("A", 55.0, 50.0), ("D", 73.4, 50.0), ("G", 98.0, 50.0)])]
       self.UpdateScreen = True
       self.Target = 440.0
       self.Tolerance = 0.1
       self.freq = Freq
       self.Params = Params
       
       
    def run(self):   
        lcd_line_1 = "Starting"
        lcd_line_2 = "Threads"
        lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
#        sleep(1)
        while True:
            if self.MenuNum == 2:
                while (not self.Back.is_set()) and (not self.Done.is_set()):
                    if not self.freq.empty():
                        lcd_line_1 = "Tuning: " + str(self.Target)
                        lcd_line_2 = "{:.2f} Hz".format(self.freq.get())
                        lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
                        print("Update")
                if self.Back.is_set():
                    self.Stahp.set()
                    self.Back.clear()
                    lcd_line_1 = "Cancelled"
                    lcd_line_2 = ""
                    lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
                    sleep(1)
                else:
                    self.Done.clear()
                    lcd_line_1 = "Successfully"
                    lcd_line_2 = "Tuned :)"
                    lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
                    sleep(1)
                
                self.MenuNum = 1
                self.UpdateScreen = True
                    
                    
                
            else:
                if self.Up.is_set():
                    if self.MenuNum == 0:
                        self.Menu1IDX -= 1
                    else:
                        self.Menu2IDX -= 1
                    self.UpdateScreen = True
                    self.Up.clear()
                        
                if self.Down.is_set():
                    if self.MenuNum == 0:
                        self.Menu1IDX += 1
                    else:
                        self.Menu2IDX += 1
                    self.UpdateScreen = True
                    self.Down.clear()    
                    
                if self.Select.is_set():
                    if self.MenuNum == 0:
                        self.MenuNum = 1
                    else:
                        self.MenuNum = 2
                    self.UpdateScreen = True
                    self.Select.clear()    
                    
                if self.Back.is_set():
                    if self.MenuNum == 1:
                        self.MenuNum = 0
                    self.UpdateScreen = True
                    self.Back.clear()
                        
                if self.UpdateScreen:
                    if self.MenuNum == 0:
                        lcd_line_1 = (self.Menu[(self.Menu1IDX % len(self.Menu))])[0] + "*"
                        lcd_line_2 = (self.Menu[((self.Menu1IDX + 1) % len(self.Menu))])[0]
                        
                    elif self.MenuNum == 1:
                        self.idx = (self.Menu1IDX % len(self.Menu))
                        lcd_line_1 = (((self.Menu[self.idx]))[1][(self.Menu2IDX % len((self.Menu[self.idx])[1]))])[0] + "*"
                        lcd_line_2 = (((self.Menu[self.idx]))[1][((self.Menu2IDX + 1) % len((self.Menu[self.idx])[1]))])[0]
#                        print(self.Menu[self.idx])
                        
                    else:
                        self.idx2 = (self.Menu2IDX % len((self.Menu[self.idx])[1]))
                        self.Target = (((self.Menu[self.idx]))[1][self.idx2])[1]
                        self.Range = (((self.Menu[self.idx]))[1][self.idx2])[2]
#                        self.Range = 80.0
                        self.Params.put([self.Target, self.Range])
                        lcd_line_1 = "Tuning..."
                        lcd_line_2 = "Target: " + str(self.Target)
                        self.Sync.set()
                        
#                    print(self.idx)
#                    print(self.idx2)
                    print(lcd_line_1)
                    print(self.MenuNum)
                    lcd.message = lcd_line_1.ljust(16) + "\n" + lcd_line_2.ljust(16)
                    self.UpdateScreen = False
            

def Up(channel):
    Up_E.set()
    print("UP SET")
    
def Down(channel):
    Down_E.set()
    print("DOWN SET")
    
def Select(channel):
    Select_E.set()
    print("SELECT SET")
    
def Back(channel):
    Back_E.set()
    print("BACK SET")
    

def main():
    
#    Up_E = threading.Event()
#    Down_E = threading.Event()
#    Select_E = threading.Event()
#    Back_E = threading.Event()
    
#    Sync = threading.Event()
#    Tune = threading.Event()
#    UpdateFreq = threading.Condition()
#    UpdateGUI = threading.Condition()
#    StateLock = threading.Lock()   

    Freq = queue.Queue()
    Params = queue.Queue()
    
#    GPIO.add_event_detect(4, GPIO.FALLING, callback=Up, bouncetime=300)
#    GPIO.add_event_detect(5, GPIO.FALLING, callback=Down, bouncetime=300)
#    GPIO.add_event_detect(6, GPIO.FALLING, callback=Select, bouncetime=300)
#    GPIO.add_event_detect(13, GPIO.FALLING, callback=Back, bouncetime=300)
    
    GPIO.add_event_detect(19, GPIO.FALLING, callback=Up, bouncetime=300)
    GPIO.add_event_detect(13, GPIO.FALLING, callback=Down, bouncetime=300)
    GPIO.add_event_detect(6, GPIO.FALLING, callback=Select, bouncetime=300)
    GPIO.add_event_detect(5, GPIO.FALLING, callback=Back, bouncetime=300)
        
    Operate = Update(Sync, Tune, Freq, Params, Back, Stahp, Done)
    GUI = LCD(Freq, Params, Sync, Up_E, Down_E, Select_E, Back_E, Stahp, Done)
    Operate.start()
    GUI.start()
    
    
    
    Operate.join()
    GUI.join()

if __name__ == '__main__':
    main()