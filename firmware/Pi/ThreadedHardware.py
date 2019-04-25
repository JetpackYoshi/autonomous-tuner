import threading
import PyCmdMessenger
from Queue import Queue


arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200)

"""
commands = [["ReqSettings",""],
            ["RcvSettings","ii"],
            ["Sync","i"],
            ["AckSync","ii"],
            ["PlotReady",""],
            ["AckPlotReady","ii"],
            ["Plot",""],
            ["AckPlot","ii"],
            ["AckPlotD","iii"],
            ["TuneDone","ii"],
            ["Error","s"]]
"""
commands = [["GetStat",""],
            ["RcvStat","si"],
            ["GetPitch",""],
            ["RcvPitch","siii"],
            ["SetPitch","ii"],
            ["InitTune",""],
            ["AckTune","sii"],
            ["StopTune",""],
            ["AckStop","sii"],
            ["Calibrate","ii"],
            ["AckCal","siii"]]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

class Update(threading.Thread):
    def __init__(self, slock, plock, sync, plot, stats, params, freq, fcond):
        threading.Thread.__init__(self)
        self.slock = slock
        self.plock = plock
        self.sync = sync
        self.plot = plot
        self.stats = stats
        self.params = params
        self.freq = freq
        self.fcond = fcond
        self.msg = []
        
        
    def run(self):
        while True:
            self.msg = c.receive()
            
            with self.slock:
                self.stats = self.msg[1]
                
            if self.plot.is_set:
                if self.msg[0] == "RcvSettings":
                    c.send("PlotReady")
                elif self.msg[0] == "AckPlotReady":
                    c.send("Plot")
                elif self.msg[0] == "AckPlot":
                    c.send("Plot")
                elif self.msg[0] == "AckPlotD":
                    self.fcond.acquire()
                    self.freq.append((self.msg[1])[2])
                    self.fcond.notify()
                    self.fcond.release()
                    c.send("Plot")
                elif self.msg[0] == "TuneDone":
                    self.plot.clear()
                    c.send("ReqSettings")
                    
            elif self.sync.is_set():
                if self.msg[0] == "RcvSettings":
                    with self.plock:
                        c.send("Sync", self.params)
                elif self.msg[0] == "AckSync":
                    self.sync.clear()
                    self.plot.set()
                    c.send("ReqSettings")  
                    
            else:
                c.send("ReqSettings")
                        
            
class Plot(threading.Thread):
    def __init__(self, freq, fcond):
       threading.Thread.__init__(self)
       self.freq = freq
       self.fcond = fcond
       self.val = []
       
    def run(self):   
        while True:
            self.condition.acquire()
            while True:
                if self.freq:
                    self.val = self.freq.pop()
                    print(self.val)
                    break
                self.fcond.wait()
            self.fcond.release()
            

def main():
    statlock = threading.Lock()
    paramlock = threading.Lock()
    syncparam = threading.Event()
    plot = threading.Event()
    freqcond = threading.Condition()
    
    Freq = [] 
    DevStats = []
    Parameters = []
    
    Operate = Update(statlock, paramlock, syncparam, plot, DevStats, Parameters, Freq, freqcond)
    Plotter = Plot(Freq, freqcond)
    Operate.start()
    Plotter.start()
    
    
    
    Operate.join()
    Plotter.join()

if __name__ == '__main__':
    main()