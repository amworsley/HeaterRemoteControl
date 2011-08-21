#! /usr/bin/python

import serial
import time
import sys

line = serial.Serial('/dev/ttyACM0', 115200)
line.open()
line.flushInput()
time.sleep(1)
while line.inWaiting() > 0:
    x = line.readline()
    x = x.rstrip('\n\r')
    print(x)
    time.sleep(1)
    i = sys.stdin.readline()
    i = i[0]
    if i == 't':
	line.write('t')
    	
