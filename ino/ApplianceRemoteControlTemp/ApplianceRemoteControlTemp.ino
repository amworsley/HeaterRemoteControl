/*
 * ApplianceRemoteControl
 *
 * Waits for control values to be sent via the serial connection and
 * pulses digital outputs to trigger the buttons on an appliance remote
 * control. Outputs are pulsed for 200ms to simulate manually pressing
 * a button.
 *
 * The number of outputs can be trivially varied to suit your needs.
 *
 * This sketch is a very simple and useful building block for any
 * project where you need to use an Arduino as an intermediary to link
 * together a host computer and a physical device for automated
 * control.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * Copyright 2009 Hugh Blemings <hugh@blemings.org>
 * http://www.practicalarduino.com/projects/appliance-remote-control
 *
 * Extended to contol a heater and allow querying of it
 *
 * Commands are by u-controller on serial port are a single character - usually sent by a shell script are:
 *
 *   a = Activate Controller (controller only switches on/off heater when active (i.e. activated)
 *   d = De-Activate Controller
 *
 *   b = Dump the current internal state of the Controller
 *   t or T - Print the current temperature reading (read from Thyristor)
 *   
 *   1-8 = Single digit pulses that PIN, HEATER_ON is wired to On to HEATER_OFF to Off button
 *
 *   + = Increment the set temperature by H degrees
 *   - = Decrement the set temperature by H degrees
 * 
 *   P = Increment the buttonPressTime by BTN_PRESS_DELTA
 *   p = Decrement the buttonPressTime by BTN_PRESS_DELTA
 *
 *   R = Increment the btn_repeat by 1
 *   r = Decrement the btn_repeat by 1
 *
 *   Every INTERVAL seconds it evaluates the temperature and when active switches ON/OFF the heater
 *  based on whether the measured temperature is above / below the set temperatures. There is a
 *  hysteresis H around the set temperature that is allowed before the controller will allow the
 *  temperature to reach before switching the direction of control. e.g. Switches off the heater once
 *  the temperature has risen above set temperature + H degrees or switch on once the temperature has
 *  fallen below set temperature.
 *   
 *
 * Copyright 2012 Andrew Worsley <amworsley@gmail.com>
 */

#define VER_STR "HeaterControl v1.0a 14 June 2014"
// Use pins 5 through 12 as the digital outputs
enum D_OUTPUTs {
 D_OUTPUT1 = 5,
 D_OUTPUT2 = 6,
 D_OUTPUT3 = 7,
 D_OUTPUT4 = 8,
 D_OUTPUT5 = 9,
 D_OUTPUT6 = 10,
 D_OUTPUT7 = 11,
 D_OUTPUT8 = 12,
};

#define BTN_PRESS_DELTA 100

#define DEBUG 0
#define DFLT_PRESS_TIME 600
int buttonPressTime = DFLT_PRESS_TIME;   // Number of milliseconds to hold outputs on
unsigned int INTERVAL = 60*1000; // Number of milliseconds to wait between updating temperatures
unsigned long elapsed = 0; // time in milliseconds of last update
#define ON 1
#define OFF 0

#define HEATER_ON D_OUTPUT3 /* pulse this to turn Heater On */
#define HEATER_OFF D_OUTPUT4 /* pulse this to turn Heater Off */

#define DFLT_BTN_REPEAT	3
int btn_repeat	= DFLT_BTN_REPEAT; /* no. of times to repeat button press for reliability */

float H = .2; /* hysterias */
int active = OFF; /* heater control state */
int state = HEATER_OFF;  // Heater state
int new_state = state;
float temp = 21.0;
float set_temp = 21.0;

#include <math.h>

void updateState();
void RemoteTemperature();
void pulse(byte output, int pressTime);
void RemoteControl(byte val);

double 
Thermister(int RawADC)
{
  double Temp;
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius

  return Temp;
}

/**
 * Initial configuration
 */
void setup()
{
  // Open the serial connection to listen for commands from the host
  Serial.begin(38400);

#if DEBUG
  Serial.println("Debug is on");
  INTERVAL = 6*1000;
  set_temp = 10.0;
#endif

  // Set up the pins as outputs
  pinMode(D_OUTPUT1, OUTPUT);
  pinMode(D_OUTPUT2, OUTPUT);
  pinMode(D_OUTPUT3, OUTPUT);
  pinMode(D_OUTPUT4, OUTPUT);
  pinMode(D_OUTPUT5, OUTPUT);
  pinMode(D_OUTPUT6, OUTPUT);
  pinMode(D_OUTPUT7, OUTPUT);
  pinMode(D_OUTPUT8, OUTPUT);

  // Make sure the outputs are all set LOW initially
  digitalWrite(D_OUTPUT1, LOW);
  digitalWrite(D_OUTPUT2, LOW);
  digitalWrite(D_OUTPUT3, LOW);
  digitalWrite(D_OUTPUT4, LOW);
  digitalWrite(D_OUTPUT5, LOW);
  digitalWrite(D_OUTPUT6, LOW);
  digitalWrite(D_OUTPUT7, LOW);
  digitalWrite(D_OUTPUT8, LOW);
}

/**
 * Main program loop
 */
void loop()
{
  byte val;

  unsigned long time = millis();

  if (time < elapsed)
    elapsed = -INTERVAL; // wrap around? 

  if (elapsed + INTERVAL < time) { // time to do update
    elapsed = time;
    RemoteTemperature();
    if (active == ON) {
      updateState();
    } else if (state == HEATER_ON) {
      Serial.println("Heater On while de-activated - switching off");
      state = HEATER_OFF;
      pulse(state, buttonPressTime);
    } 
    //Serial.print("time=");
    //Serial.println(time);
  }

  // Check if a value has been sent by the host
  if(Serial.available()) {
    val = Serial.read();
    if (val == 'a') {
      Serial.println("Activating Controller");
      active = ON;
      updateState();
    } 
    else if (val == 'd') {
      Serial.println("De-activating Controller");
      active = OFF;
      state = HEATER_OFF;
      pulse(state, buttonPressTime);
    } 
    else if (val == '-') {
      Serial.println("Decrementing set point to ");
      set_temp -= H;
      Serial.println(set_temp);
      updateState();
    } 
    else if (val == '+') {
      Serial.println("Incrementing Set point to ");
      set_temp += H;
      Serial.println(set_temp);
      updateState();
    } 
    else if (val == 'b') {
      Serial.println("Current Variables:");
      Serial.print("active=");
      Serial.println(active);
      Serial.print("state=");
      Serial.print(state);
      if (state == HEATER_ON)
	  Serial.println(" (ON)");
      else if (state == HEATER_OFF)
	  Serial.println(" (OFF)");
      else
	  Serial.println(" (Unknown?)");
      Serial.println(VER_STR);
      Serial.print("temp=");
      Serial.print(temp);
      Serial.print(" set_temp=");
      Serial.print(set_temp);
      Serial.print(" Hysteresis (H)=");
      Serial.println(H);
      Serial.print("INTERVAL=");
      Serial.print(INTERVAL);
      Serial.print(" elapsed=");
      Serial.print(elapsed);
      Serial.print(" time=");
      Serial.println(time);
      Serial.print("buttonPressTime=");
      Serial.print(buttonPressTime);
      Serial.print(" btn_repeat=");
      Serial.println(btn_repeat);
    } 
    else if (val >= '1' && val <= '8')
      RemoteControl(val);
    else if (val == 't' || val == 'T')
      RemoteTemperature();
    else if (val == 'R') {
      Serial.print("Incrementing btn_repeat: ");
      btn_repeat++;
      Serial.println(btn_repeat);
    }
    else if (val == 'r') {
      if (btn_repeat > 1) {
          Serial.print("Decrementing btn_repeat: ");
          btn_repeat--;
      }
      else
          Serial.print("btn_repeat at minimum: ");
      Serial.println(btn_repeat);
    }
    else if (val == 'P') {
      Serial.print("Incrementing buttonPressTime: ");
      buttonPressTime += BTN_PRESS_DELTA;
      Serial.println(buttonPressTime);
    }
    else if (val == 'p') {
      if (buttonPressTime > BTN_PRESS_DELTA) {
          Serial.print("Decrementing buttonPressTime: ");
          buttonPressTime -= BTN_PRESS_DELTA;
      }
      else
          Serial.print("buttonPressTime at minimum: ");
      Serial.println(buttonPressTime);
    }
    else {
      Serial.print(val);
      Serial.println(": Unknown Command ignored");
    }
  }
}
void
updateState()
{
  if (temp > set_temp + H)
    new_state = HEATER_OFF;
  else if (temp < set_temp - H)
    new_state = HEATER_ON;
  if (state != new_state) {
    Serial.print("Toggling Heater to");
    if (new_state == HEATER_ON)
      Serial.println(" on");
    else
      Serial.println(" off");
    pulse(new_state, buttonPressTime);
  }     
}

void
RemoteTemperature()
{
  temp = Thermister(analogRead(0));
  Serial.print(temp);
  Serial.println(" C");
}
void
pressBtn(byte output, int pressTime)
{
  Serial.print("Pin ");
  Serial.print(output - 5 + 1);
  Serial.print(" on...");

  digitalWrite(output, HIGH);
  delay(pressTime);
  digitalWrite(output, LOW);

  Serial.println("Off");
  state = output;
}
/*
 * pulse the button repeated for reliability as we 
 * have no sensor to detect the request has worked
 * in the case of a radio remote control for example.
 */
void
pulse(byte output, int pressTime)
{
    int cnt = 0;
    while (cnt++ < btn_repeat)
    	pressBtn(output, pressTime);
}
void
RemoteControl(byte val)
{
  if(val == '1') {
    // Pulse the 1st button
    pulse(D_OUTPUT1, buttonPressTime);
  } 
  else if(val == '2') {
    // Pulse the 2nd button
    pulse(D_OUTPUT2, buttonPressTime);
  } 
  else if(val == '3') {
    // Pulse the 3rd button
    pulse(D_OUTPUT3, buttonPressTime);
  } 
  else if(val == '4') {
    // Pulse the 4th button
    pulse(D_OUTPUT4, buttonPressTime);
  } 
  else if(val == '5') {
    // Pulse the 5th button
    pulse(D_OUTPUT5, buttonPressTime);
  } 
  else if(val == '6') {
    // Pulse the 6th button
    pulse(D_OUTPUT6, buttonPressTime);
  } 
  else if(val == '7') {
    // Pulse the 7th button
    pulse(D_OUTPUT7, buttonPressTime);
  } 
  else if(val == '8') {
    // Pulse the 8th button
    pulse(D_OUTPUT8, buttonPressTime);
  }


}

    /* vim: set filetype=c : */
