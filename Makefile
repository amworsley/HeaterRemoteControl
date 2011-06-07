DAEMON := hotDaemon
CMDS := heaterOn heaterOff getTemp

# Daemon directory
DDIR := /usr/local/sbin

# Client Directory
CDIR := /usr/local/bin

install: 
	cp $(CMDS) $(CDIR)
	cp $(DAEMON) $(DDIR)
