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
 */

// Use pins 5 through 12 as the digital outputs
int output1 = 5;
int output2 = 6;
int output3 = 7;
int output4 = 8;
int output5 = 9;
int output6 = 10;
int output7 = 11;
int output8 = 12;

#define DEBUG 0
int buttonPressTime = 600;   // Number of milliseconds to hold outputs on
unsigned int INTERVAL = 60*1000; // Number of milliseconds to wait between updating temperatures
unsigned long elapsed = 0; // time in milliseconds of last update
#define ON 1
#define OFF 0

#define HEATER_ON output3 /* pulse this to turn Heater On */
#define HEATER_OFF output4 /* pulse this to turn Heater Off */

float H = .5; /* hysterias */
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
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);
  pinMode(output6, OUTPUT);
  pinMode(output7, OUTPUT);
  pinMode(output8, OUTPUT);

  // Make sure the outputs are all set LOW initially
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(output6, LOW);
  digitalWrite(output7, LOW);
  digitalWrite(output8, LOW);
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
      pulse(state, buttonPressTime);
    } 
    else if (val == '-') {
      Serial.println("Decrementing set point to ");
      set_temp -= H;
      Serial.println(set_temp);
    } 
    else if (val == '+') {
      Serial.println("Incrementing Set point to ");
      set_temp += H;
      Serial.println(set_temp);
    } 
    else if (val == 'b') {
      Serial.println("Current Variables:");
      Serial.print("active=");
      Serial.println(active);
      Serial.print("state=");
      Serial.println(state);
      Serial.print("temp=");
      Serial.println(temp);
      Serial.print("set_temp=");
      Serial.println(set_temp);
      Serial.print("INTERVAL=");
      Serial.println(INTERVAL);
      Serial.print("elapsed=");
      Serial.println(elapsed);
      Serial.print("time=");
      Serial.println(time);
    } 
    else if (val >= '1' && val <= '8')
      RemoteControl(val);
    else if (val == 't' || val == 'T')
      RemoteTemperature();
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
pulse(byte output, int pressTime)
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
void
RemoteControl(byte val)
{
  if(val == '1') {
    // Pulse the 1st button
    pulse(output1, buttonPressTime);
  } 
  else if(val == '2') {
    // Pulse the 2nd button
    pulse(output2, buttonPressTime);
  } 
  else if(val == '3') {
    // Pulse the 3rd button
    pulse(output3, buttonPressTime);
  } 
  else if(val == '4') {
    // Pulse the 4th button
    pulse(output4, buttonPressTime);
  } 
  else if(val == '5') {
    // Pulse the 5th button
    pulse(output5, buttonPressTime);
  } 
  else if(val == '6') {
    // Pulse the 6th button
    pulse(output6, buttonPressTime);
  } 
  else if(val == '7') {
    // Pulse the 7th button
    pulse(output7, buttonPressTime);
  } 
  else if(val == '8') {
    // Pulse the 8th button
    pulse(output8, buttonPressTime);
  }


}

