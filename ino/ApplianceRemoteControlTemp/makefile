########################################################################
#
# Based on /usr/share/arduino/Arduino.mk from arduino-mk debian package
#

#
# Hopefully these will be self-explanatory but in case they're not:
#
#    ARDUINO_DIR  - Where the Arduino software has been unpacked
ARDUINO_DIR  = /usr/share/arduino
#
#    TARGET       - The basename used for the final files. Canonically
#                   this would match the .pde file, but it's not needed
#                   here: you could always set it to xx if you wanted!
TARGET       = ApplianceRemoteControlTemp
#
#    ARDUINO_LIBS - A list of any libraries used by the sketch (we
#                   assume these are in
#                   $(ARDUINO_DIR)/hardware/libraries 
ARDUINO_LIBS = WInterrupts
#
#    ARDUINO_PORT - The port where the Arduino can be found (only needed
#                   when uploading
ARDUINO_PORT = /dev/ttyACM0
#
#    BOARD_TAG    - The ard-parse-boards tag for the board e.g. uno or mega
#                   'make show_boards' shows a list
BOARD_TAG    = uno

include /usr/share/arduino/Arduino.mk

#
# Once this file has been created the typical workflow is just
#
#   $ make upload
#
# All of the object files are created in the build-cli subdirectory
# All sources should be in the current directory and can include:
#  - at most one .pde or .ino file which will be treated as C++ after
#    the standard Arduino header and footer have been affixed.
#  - any number of .c, .cpp, .s and .h files
#
# Included libraries are built in the build-cli/libs subdirectory.
#
# Besides make upload you can also
#   make             - no upload
#   make clean       - remove all our dependencies
#   make depends     - update dependencies
#   make reset       - reset the Arduino by tickling DTR on the serial port
#   make raw_upload  - upload without first resetting
#   make show_boards - list all the boards defined in boards.txt
#
