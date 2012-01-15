DAEMON := logSerial.py
CMDS := heaterOn heaterOff getTemp heaterActivate \
    heaterDeActivate heaterDecr heaterDump heaterIncr
MODULES := daemonize.py

# Daemon directory
DDIR := /usr/local/sbin

# Client Directory
CDIR := /usr/local/bin

# Module Directory
MDIR := /usr/local/sbin

# Configuration Directory
EDIR := /etc

install: 
	cp $(CMDS) $(CDIR)
	cp $(DAEMON) $(DDIR)
	cp $(MODULES) $(MDIR)
	cp rc-heater /etc/init.d
	update-rc.d rc-heater defaults
	cp remote-control.cfg $(EDIR)
