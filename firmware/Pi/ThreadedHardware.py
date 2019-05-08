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

lcd_rs = digitalio.DigitalInOut(board.D22)
lcd_en = digitalio.DigitalInOut(board.D17)
lcd_d4 = digitalio.DigitalInOut(board.D25)
lcd_d5 = digitalio.DigitalInOut(board.D24)
lcd_d6 = digitalio.DigitalInOut(board.D23)
lcd_d7 = digitalio.DigitalInOut(board.D18)


lcd = characterlcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, lcd_columns, lcd_rows)

lcd.clear()

GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.IN, pull_up_down=GPIO.PUD_UP)  
GPIO.setup(5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(13, GPIO.IN, pull_up_down=GPIO.PUD_UP)

Up_E = threading.Event()
Down_E = threading.Event()
Select_E = threading.Event()
Back_E = threading.Event()

arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200)
#arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200)

commands = [["GetStat",""],
            ["RcvStat","si"],
            ["GetPitch",""],
            ["RcvPitch","siii"],
            ["SetTarget","ii"],
            ["AckTarget","siii"],
            ["InitTune",""],
            ["AckTune","sis"],
            ["StopTune",""],
            ["AckStop","sis"],
            ["Calibrate","ii"],
            ["AckCal","siii"],
            ["Stream",""],
            ["AckStream","sis"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

class Update(threading.Thread):
    def __init__(self, Sync, Tune, Freq, Params, Back):
        threading.Thread.__init__(self)
        
        self.tune = Tune
        self.sync = Sync
        self.freq = Freq
        self.params = Params
        self.back = Back
        self.msg = []
        
        
    def run(self):
        while True:
            self.msg = c.receive()
            
#            with self.slock:
#                self.state = (self.msg[1])[0]
                
            if self.tune.is_set:
                if ((self.msg[1])[0]) == "READY":
                    c.send("InitTune")
                elif ((self.msg[1])[0]) == "TUNING":
                    if self.msg[0] == "RcvPitch":
                        self.freq.put((self.msg[1])[2])
                    c.send("GetPitch")
                elif ((self.msg[1])[0]) == "DONE":
                    self.tune.clear()
                    c.send("ReqSettings")
                    
            elif self.sync.is_set():
                if self.msg[0] == "AckSync":
                    self.sync.clear()
                    self.tune.set()
                    c.send("ReqSettings")
                else:
                    c.send("Sync", self.params) 
                    
            else:
                c.send("ReqSettings")
                        
            
class LCD(threading.Thread):
    def __init__(self, Freq, Up, Down, Select, Back):
       threading.Thread.__init__(self)
       
       self.Up = Up
       self.Down = Down
       self.Select = Select
       self.Back = Back
       
       self.MenuNum = 0
       self.Menu1IDX = 0
       self.Menu2IDX = 0
       self.idx = 0
       self.idx2 = 0
       self.Menu =  [("Violin", [("E", 659.3), ("A", 440.0), ("D", 293.7), ("G", 196.0)]),
                     ("Viola", [("A", 440.0), ("D", 293.7), ("G", 196.0), ("C", 130.8)]),
                     ("Cello", [("A", 220.0), ("D", 146.8), ("G", 98.0), ("C", 65.41)]),
                     ("Upright Bass", [("E", 41.2), ("A", 55.0), ("D", 73.4), ("G", 98.0)])]
#       self.UpdateF = UpdateF
       self.UpdateScreen = True
       self.Target = 440.0
       self.Tolerance = 0.1
       self.freq = Freq
       
       
    def run(self):   
        lcd_line_1 = "Starting"
        lcd_line_2 = "Threads"
        lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
        sleep(1)
        while True:
            if self.MenuNum == 2:
                while not self.Back.is_set():
                    if not self.freq.empty():
                        lcd_line_1 = "Tuning: " + self.Target
                        lcd_line_2 = (self.freq.get()) + " Hz"
                        lcd.message = lcd_line_1.ljust(16)  + "\n" + lcd_line_2.ljust(16)
                self.MenuNum = 1
                self.Back.clear()
                    
                    
                
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
                        print(self.Menu[self.idx])
                        lcd_line_1 = (((self.Menu[self.idx]))[1][(self.Menu2IDX % len(self.Menu[self.idx]))])[0] + "*"
                        lcd_line_2 = (((self.Menu[self.idx]))[1][((self.Menu2IDX + 1) % len(self.Menu[self.idx]))])[0]
                        
                    else:
                        self.idx2 = (self.Menu2IDX % len(self.Menu[self.idx]))
                        self.Target = (((self.Menu[self.idx]))[1][self.idx2])[1]
                        self.Range = int(round(self.Target * self.Tolerance))
                        self.Params = [self.Target, self.Range]
                        lcd_line_1 = "Tuning..."
                        lcd_line_2 = "Target: "
                    print(self.idx)
                    print(self.idx2)
                    print(lcd_line_1)
                    print(self.MenuNum)
                    lcd.message = lcd_line_1.ljust(16) + "\n" + lcd_line_2.ljust(16)
                    self.UpdateScreen = False
            

def Up(channel):
    Up_E.set()
    
def Down(channel):
    Down_E.set()
    
def Select(channel):
    Select_E.set()
    
def Back(channel):
    Back_E.set()
    

def main():
    lcd_line_1 = "Running"
    lcd_line_2 = "Program"
    
    lcd.message = lcd_line_1 + lcd_line_2
    
#    Up_E = threading.Event()
#    Down_E = threading.Event()
#    Select_E = threading.Event()
#    Back_E = threading.Event()
    
    Sync = threading.Event()
    Tune = threading.Event()
#    UpdateFreq = threading.Condition()
#    UpdateGUI = threading.Condition()
#    StateLock = threading.Lock()
    
    Freq = queue.Queue()
    Params = []
    
    GPIO.add_event_detect(4, GPIO.FALLING, callback=Up, bouncetime=300)
    GPIO.add_event_detect(5, GPIO.FALLING, callback=Down, bouncetime=300)
    GPIO.add_event_detect(6, GPIO.FALLING, callback=Select, bouncetime=300)
    GPIO.add_event_detect(13, GPIO.FALLING, callback=Back, bouncetime=300)
    
    Operate = Update(Sync, Tune, Freq, Params, Back)
    GUI = LCD(Freq, Up_E, Down_E, Select_E, Back_E)
    Operate.start()
    GUI.start()
    
    
    
    Operate.join()
    GUI.join()

if __name__ == '__main__':
    main()