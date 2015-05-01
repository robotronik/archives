#!/usr/bin/python
# -*- coding: utf-8 -*-

import Tkinter
import serial
import struct
import sys
import time
import matplotlib
import matplotlib.pyplot

class GUIquad :
    #attention, les var de Get() sont des strings
    def fct(self):
        self.sendAll();

    def fct2(self, value):
        self.sendAll();

    def sendAll(self):
        if time.time() - self.lastSend > 0.01 : #10 millisecondes
            toSend = struct.pack('!ffff',float(self.throttle_scale.get()),float(self.coef_P.get()),float(self.coef_I.get()), float(self.coef_D.get()))
            self.ser.write(toSend)
            self.lastSend = time.time()



    def __init__(self) :   
        TIMEOUT = 0.001

        self.lastSend = time.time()
        #com serie
        try :
            self.ser = serial.Serial('/dev/ttyACM0', 115200, timeout=TIMEOUT)
        except serial.SerialException :
            try : 
                print "error connecting ttyACM0, trying ttyACM1"
                self.ser = serial.Serial('/dev/ttyACM1', 1152000, timeout=TIMEOUT)
            except serial.SerialException :
                try : 
                    print "error connecting ttyACM1, trying ttyUSB0"
                    self.ser = serial.Serial('/dev/ttyUSB0', 1152000, timeout=TIMEOUT)
                except serial.SerialException :
                    try : 
                        print "error connecting ttyUSB0, trying ttyUSB1"
                        self.ser = serial.Serial('/dev/ttyUSB1', 1152000, timeout=TIMEOUT)
                    except serial.SerialException :
                        print "Erreur lors de l'établissement de la liaison série"
                        sys.exit()  #fin prématurée du programme

        self.top = Tkinter.Tk()


        #coeffs PID
        text1 = Tkinter.StringVar()
        text1.set("P coefficient")
        label1 = Tkinter.Label(self.top, textvariable=text1)
        label1.pack()
        self.coef_P = Tkinter.Spinbox(self.top, from_=-100, to=100, increment=0.01, command=self.fct)
        self.coef_P.pack()

        text2 = Tkinter.StringVar()
        text2.set("I coefficient")
        label2 = Tkinter.Label(self.top, textvariable=text2)
        label2.pack()
        self.coef_I = Tkinter.Spinbox(self.top, from_=-100, to=100, increment=0.01, command=self.fct)
        self.coef_I.pack()

        text3 = Tkinter.StringVar()
        text3.set("D coefficient")
        label3 = Tkinter.Label(self.top, textvariable=text3)
        label3.pack()
        self.coef_D = Tkinter.Spinbox(self.top, from_=-100, to=100, increment=0.01, command=self.fct)
        self.coef_D.pack()


        #throttle
        throttle = Tkinter.IntVar()
        self.throttle_scale = Tkinter.Scale(self.top, variable=throttle, from_=3999, to=0, command=self.fct2, label = "throttle")
        self.throttle_scale.pack()

        self.top.after(100, self.readSerial) #lance la tâche readSerial après 100ms

        self.initPlotter()

        self.top.mainloop()
        
    def initPlotter(self):
        self.LPitch = range(-500,500,1) #liste des valeurs de Pitch 

        fig = matplotlib.pyplot.figure(1)
        ax = fig.add_subplot(111)
        ax.grid(True)
        ax.set_title("Realtime plot")
        ax.set_xlabel("time")
        ax.set_ylabel("amplitude")
        ax.axis([0,1000,-50,50])
        self.line1 = ax.plot(range(0,1000,1), self.LPitch, '+')
        self.canvas = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(fig, master = self.top)
        self.canvas.show()
        self.canvas.get_tk_widget().pack(side=Tkinter.TOP, fill=Tkinter.BOTH, expand=1)

        self.canvas._tkcanvas.pack(side=Tkinter.TOP, fill=Tkinter.BOTH, expand=1)


    
    def updatePlot(self):
        while len(self.LPitch) > 1000 : #10 secondes d'affichage
            self.LPitch.remove(self.LPitch[0]) #pas trop allonger la liste
        self.line1[0].set_data(range(0,1000,1),self.LPitch)
        self.canvas.draw()
    

    def readSerial(self):
        buffNotEmpty = True
        while buffNotEmpty : #permet de vider le buffer
            mess = self.ser.readline()
            mess = mess.split(" ")
            if mess[0] == '': #buffer is empty
                buffNotEmpty = False
            else :
                if mess[0] == 'r' :
                    #print "                     " , mess
                    try :
                        self.LPitch.append(float(mess[1]))
                    except ValueError :
                        print "oops, ValueError"
                        self.LPitch.append(self.LPitch[-1]) #concerve le temps
                    except IndexError :
                        print "oops, IndexError"
                        self.LPitch.append(self.LPitch[-1]) #concerve le temps

                else :
                    print mess
        self.updatePlot()
        self.top.after(1, self.readSerial) #relance la tâche readSerial après 100ms
 
if __name__ == '__main__' : 
    a = GUIquad()

        
    
