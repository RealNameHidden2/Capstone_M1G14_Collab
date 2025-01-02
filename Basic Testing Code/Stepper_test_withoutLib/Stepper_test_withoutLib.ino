/*
  Stepper Motor Test
  stepper-test01.ino
  Uses MA860H or similar Stepper Driver Unit
  Has speed control & reverse switch
  
  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Define pins
int reverseSwitch = 41;  // Push button for reverse
int driverPUL = 37;      // PUL- pin
int driverDIR = 39;      // DIR- pin

// Variables
const int pd = 500;     // Pulse Delay period (adjust for speed)
boolean setdir = LOW;   // Set Direction

// Interrupt Handler
void revmotor() {
  setdir = !setdir;     // Toggle direction
}

void setup() {
  // Set motor driver pins as outputs
  pinMode(driverPUL, OUTPUT);
  pinMode(driverDIR, OUTPUT);

  // Set reverseSwitch as INPUT_PULLUP
  pinMode(reverseSwitch, INPUT_PULLUP);

  // Attach interrupt to reverseSwitch pin
  attachInterrupt(digitalPinToInterrupt(reverseSwitch), revmotor, FALLING);
}

void loop() {
  // Set motor direction
  digitalWrite(driverDIR, setdir);

  // Generate pulses for the stepper motor
  digitalWrite(driverPUL, HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPUL, LOW);
  delayMicroseconds(pd);
}
