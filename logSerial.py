#! /usr/bin/python

from daemonize import daemonize

import serial
import time
import sys
import os
import select
import optparse
import ConfigParser

verbose = False
dfile = '/dev/ttyACM0'
logFile = "/var/log/remote-control.log"
elogFile = "/var/log/heaterd.log" # daemon's standard output 
configFile="/etc/remote-control"
timeout = 30 # timeout in seconds when we poll the temperature
baudRate = 38400

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
    config = {}
    if not os.access(file, os.R_OK):
    	return config
    Config = ConfigParser.ConfigParser()
    Config.read(file)
    sections = Config.sections()
    for section in sections:
	if verbose:
	    print "Section: " + section
	for (key, val) in Config.items(section):
		config[key] = val
		if verbose:
		    print "config[" + key + "] = '" + val + "'"
    return config

def init(c):
    global dfile, logFile, timeout, elogFile, baudRate
    if 'dfile' in c:
	dfile = c['dfile']
    if 'logfile' in c:
	logFile = c['logfile']
    if 'timeout' in c:
	timeout = c['timeout']
    if 'elogfile' in c:
	elogFile = c['elogfile']
    if 'baudrate' in c:
	baudRate = c['baudrate']

def main():
    p = optparse.OptionParser()
    p.add_option("--verbose", "-v", action="store_true",help="enable debugging",
         default=False)
    p.add_option("--config", "-c", action="store",help="config file", type='string',
         default=configFile)
    p.add_option("--dump", "-d", action="store_true",help="dump settings",
         default=False)
    options, args = p.parse_args()
    if options.verbose:
	global verbose
	verbose = options.verbose
	print("logging output from '" + dfile + "' to log file: " + logFile + "\n")

    init(ReadConfig(options.config))

    if options.dump:
	print("dumping settings:");
	print(" verbose=" + str(options.verbose)
	    + " config='" + str(options.config) + "' dump=" + str(options.dump))
	print(" dfile='" + str(dfile) + "' logFile='" + str(logFile)
	    + "' timeout=" + str(timeout));
	print(" elogFile='" + str(elogFile) + "' baudRate='" + str(baudRate))
	exit(0)

    daemonize(stdout=elogFile)

    line = serial.Serial(dfile, baudRate)
    line.open()
    line.flushInput()
    log = open(logFile, 'a', 0);

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
