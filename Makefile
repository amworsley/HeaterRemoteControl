DAEMON := logSerial.py
CMDS := heaterOn heaterOff getTemp heaterActivate \
    heaterDeActivate heaterDecr heaterDump heaterIncr

# Daemon directory
DDIR := /usr/local/sbin

# Client Directory
CDIR := /usr/local/bin

install: 
	cp $(CMDS) $(CDIR)
	cp $(DAEMON) $(DDIR)
