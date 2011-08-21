#! /usr/bin/python

import heaterDaemon

allTests = []
failed = [] # failed tests
passed = [] # passed tests

def passes(s) :
    passed.append(cTest)
    print "Passed: ", cTest

def fail(s) :
    failed.append(cTest)
    print "Failed ", cTest, s

def testConfig() :
    cTest = "Config"
    if heaterDaemon.portName != '/dev/ttyACM0' :
	fail("Invalid portName: " + heaterDaemon)
	return

def doTests() :
    testConfig()
    nP = len(passed)
    total = len(allTests)
    print nP, " out of ", total, " tests passed\n" 

doTests()
