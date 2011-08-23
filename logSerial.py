#! /usr/bin/python

import serial
import time
import sys

line = serial.Serial('/dev/ttyACM0', 38400)
line.open()
line.flushInput()

logFile = "log.txt"
log = open(logFile, 'a', 0);
def CheckInput(line) :
    if line.inWaiting() <= 0:
	return ""
    x = line.readline()
    x = x.rstrip('\n\r')
    return x


def CheckStdIn():
    if sys.stdin.inWaiting() <= 0:
	return None
    i = sys.stdin.readline()
    i = i[0]
    if i == 't':
	line.write('t')
    	
while 1:
    time.sleep(.1)
    x = CheckInput(line)
    if len(x) > 1:
	log.write(x + "\n")
#    x = CheckInput(sys.stdin)
#    if len(x) > 1:
#	log.write(x + "\n")
#	line.write(x)
