#! /usr/bin/python

import serial
import time
import sys
import os
import select
import optparse
import ConfigParser

dfile = '/dev/ttyACM0'
line = serial.Serial(dfile, 38400)
line.open()
line.flushInput()

logFile = "/home/amw/log.txt"
log = open(logFile, 'a', 0);

timeout = 5 # timeout in seconds when we poll the temperature
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

def ReadConfig(file):
    Config = ConfigParser.ConfigParser()
    if not os.access(file, os.R_OK):
    	return
    Config.read(file)
    sections = Config.sections()
    for section in sections:
	print Config.items(section)
    	
def main():
    p = optparse.OptionParser()
    p.add_option("--verbose", "-v", action="store_true",help="enable debugging",
         default=False)
    p.add_option("--config", "-c", action="store_true",help="config file",
         default=".logSerial")
    options, args = p.parse_args()
    if options.verbose:
	print("logging output from '" + dfile + "' to log file: " + logFile + "\n")
    ReadConfig(options.config)
    while 1:
	a, b, c = select.select([line], [], [], timeout)
	if line in a:
	    x = CheckInput(line)
	    log.write(x + ":"
		+ time.ctime(time.time()) + "\n")
	    print("got '" + x + "'")
	else:
	    line.write('t')

if __name__ == "__main__":
	main()
