// External Libraries 
#include <Arduino.h>
#include <Servo.h>

// Custom Libraries
#include <ControlRC.hpp>
#include <SlewRateLimiter.hpp>


/** 
 * General Reminders:
 *   1. Unplug the reciever from pin 0 (RX) when uploading code
 *   2. Run the serial monitor at a baudrate of 115200 to work properly
 * 
 * For the GitHub page, go to https://github.com/Nyx3815/Adjustable-Runway
**/ 


const double sampleRate = 5; // Number of receiver samples per second
ControlRC rcTest;

// Note >> Currently, mapped for motor control using an esc
const int joysitckMap[2] = {0, 180};    // Maps the standard joysticks
const int throttleMap[2] = {0, 180};    // Maps the throttle joystick
const int switchMap[2] = {0, 180};      // Maps the basic switches
const int cSwitchMap[3] = {0, 90, 180}; // Maps the SWC switch
const int knobMap[2] = {0, 180};        // Maps the knobs

const int motorPin = 3; 
int motorSpeed = 0;
bool enableMotor = false;
ChannelRC enableChannel = ChannelRC::SWD;

double currentTime;
double lastSample;
double lastBlink;

const int ledPin = 2;     // LED pin
const double ledFreq = 1; // Blinks per second
bool ledState = false;

const double motorChangeLimit = 30;
bool isRateLimited = true;
SlewRateLimiter rateLimit(motorChangeLimit); // Creates a SlewRateLimiter with a maximum change limit defined above 
ChannelRC limiterChannel = ChannelRC::SWA;

Servo esc;
ChannelRC testChannel = ChannelRC::SWC;


/**
 * @brief One time setup code
 */
void setup() {
  Serial.begin(ControlRC::iBusBaudrate);
  while (!Serial) { delay(20); } // Wait for the Serial port to open 

  // Set up the esc and set the initial speed to 0
  esc.attach(motorPin, 1000, 2000);
  esc.write(motorSpeed);

  // Set up the LED 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Set the mapping arrays for the values
  rcTest.setMapping(joysitckMap, ControlRC::mapType::JOYSTICK);
  rcTest.setMapping(throttleMap, ControlRC::mapType::THROTTLE);
  rcTest.setMapping(switchMap, ControlRC::mapType::SWITCH);
  rcTest.setMapping(cSwitchMap, ControlRC::mapType::C_SWITCH);
  rcTest.setMapping(knobMap, ControlRC::mapType::KNOB);

  // Set up the time variables 
  currentTime = millis();
  lastBlink = currentTime;
}


/**
 * @brief Main code loop
 */
void loop() {
  // Updates the current time in since start 
  currentTime = millis();

  // Updates the values of each channel at the start of the loop 
  if ((currentTime - lastSample) >= (1000 * (1 / sampleRate))) {
    rcTest.update();
    lastSample = currentTime;
  }

  // Updates the motor enable state and rate limiter state
  enableMotor = rcTest.getChannelValue(enableChannel, ControlRC::mapSwitches);
  isRateLimited = !rcTest.getChannelValue(limiterChannel, ControlRC::mapSwitches);
  
  // Writes to the motor using the SWD switch as enable and SWC switch as velocities
  if (enableMotor) {
    motorSpeed = isRateLimited ? rateLimit.calculate(rcTest.getChannelValue(testChannel)) : rcTest.getChannelValue(testChannel);

    Serial.print("Motor Output - ");
    Serial.print(motorSpeed);
    Serial.print(" units\t| ");
    Serial.print(map(motorSpeed, joysitckMap[0], joysitckMap[1], 0, 100));
    Serial.println("%");
  } else {
    motorSpeed = isRateLimited ? rateLimit.calculate(0) : 0;
  }

  // Set the LED state 
  if (enableMotor) { // Blinks the LED if the motor is enabled
    if ((currentTime - lastBlink) >= (500 * (1 / ledFreq))) {
      ledState = !ledState;
      lastBlink = currentTime;
    }
  } else { // Keeps the LED solid if the motor isn't enabled 
    ledState = true;
  }

  // Sets the LED and motor to their current states 
  digitalWrite(ledPin, ledState);
  esc.write(constrain(motorSpeed, 0, 180)); // Constrains the motor speed incase of weird errors

  // Delays the code to not overload the Arduino
  delay(20);
}