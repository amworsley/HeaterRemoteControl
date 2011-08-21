#! /usr/bin/python

import serial
import time
import sys

line = serial.Serial('/dev/ttyACM0', 38400)
line.open()
line.flushInput()
def CheckArduino() :
    time.sleep(.1)
    while line.inWaiting() > 0:
	x = line.readline()
	x = x.rstrip('\n\r')
	print(x)


def CheckStdIn():
    time.sleep(.1)
    i = sys.stdin.readline()
    i = i[0]
    if i == 't':
	line.write('t')
    	
while 1:
    CheckArduino()
    #CheckStdIn()
