#! /usr/bin/python
# Daemon that opens the configured port and accepts TCP/UDP
# connections on the specified Serial port and performs the given
# operation on the port and responds with the given response back to
# the requestor

import serial

portName = '/dev/ttyACM0'
portSpeed = 54600
portTimeout = 1
logFile = '/var/log/heaterLog.txt'

def config(f) :
    """
    Read in the configuration parameters from file 
    """
    pass

def setup() :
    """
    Open the serial port and validate controller
    Create socket for listening
    """

    ser = serial.Serial(portName, portSpeed, timeout=portTimeout)
    lf = open(logFile, 'wa')

    BUFLEN = 1024 # default size of input
    from socket import *

    ServerPort = 10222	# port to listen of for requests
    ServerIf = ''		# list on all interfaces

    sockObj = socket(AF_INET, SOCK_STREAM)
    sockObj.bind((ServerIf, ServerPort))
    sockObj.listen(2)

    return sockObj

def run() :
    """
    reads in the configuration file
    initialises the communications
    starts main loop
    """

    while TRUE:
	con, address = sockObj.accept()
	lf.write("From " + address + ":")
	data = con.recv(BUFLEN)
	if not data :
	    lf.write("\n")
	continue
	res = procRequest(data);
	con.send(res);
	con.close();

def procRequest(d) :
    """
    Parsed a request data. We parse it, look up the controller it's
    addressed to and call it to process the request and return the
    result as a string. The result has the form of a decimal status
    code, a space and the result string. The status code of 0 (zero)
    is successful request and -ve number of error or failure.
    """
    # parse the data into a request tuple or an error message
    # first element is a controller name
    req = parse(d)
    if type(req) == str:  # an error message
        return req

    # Look up the controller
    obj = control[req[0]]
    if obj == none :
        return("-2 " + "Unknown controller : " + str(req[0]))

    # Ask the controller to process the request
    res = obj.process(req[1:])
    return res;

def parse(s) :
    """
    Sanity check a string and parse it into a tuple with the first
    element being the name of the control object. If there is a
    problem return a string with an error message
    """

    if type(s) != str : return "-1 Bad string: " + repr(s)
    res = s.split()
    tok = res[0]
    if not tok.isalnum() or not tok[0].isalpha() :
        return "-2 : Invalid controller name '" + res[0] + "'";

    return res;

