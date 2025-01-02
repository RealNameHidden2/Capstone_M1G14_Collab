#include <AccelStepper.h>

#define STEP_PIN 37
#define DIR_PIN 39
#define LIMIT_SWITCH_PIN 49
#define DEBOUNCE_DELAY 50

// Constants for Stepper Motor Configuration
#define MICROSTEP_RESOLUTION 2 // Microstepping resolution (e.g., 1/2)

// Stepper Instance
AccelStepper stepperX(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Configuration Constants
const float stepsPerRevolution = 200.0; // Base motor steps per revolution
const float totalStepsPerRevolution = stepsPerRevolution * MICROSTEP_RESOLUTION; // Steps per revolution with microstepping
const float leadScrewPitch = 2.0; // Lead screw pitch in mm/revolution
const float stepsPerMm = totalStepsPerRevolution / leadScrewPitch; // Steps per mm

// Desired Linear Speed and Acceleration
const float targetLinearSpeed = 5.0; // mm/s
const float targetMaxSpeed = targetLinearSpeed * stepsPerMm; // Steps per second
const float targetAcceleration = targetMaxSpeed / 2; // Steps per second squared

// Variables for Debouncing
bool lastSwitchState = HIGH;
bool switchState = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);

  // Configure the stepper motor
  stepperX.setMaxSpeed(targetMaxSpeed); // Set max speed in steps per second
  stepperX.setAcceleration(targetAcceleration); // Set acceleration in steps per second squared

  // Print calculated values
  Serial.print("Steps per mm: ");
  Serial.println(stepsPerMm);
  Serial.print("Max Speed (steps/s): ");
  Serial.println(targetMaxSpeed);
  Serial.print("Acceleration (steps/s²): ");
  Serial.println(targetAcceleration);

  // Configure the limit switch pin
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);

  // Homing Process
  homeXAxis();
}

void loop() {
  // Continuously monitor and handle additional moves if needed
  moveToMm(200); // Move to 200 mm
  stepperX.run();
}

// Function to debounce the limit switch
bool isLimitSwitchTriggered() {
  bool reading = digitalRead(LIMIT_SWITCH_PIN);

  // Check if the switch state has changed
  if (reading != lastSwitchState) {
    lastDebounceTime = millis();
  }

  // If the debounce delay has passed, confirm the switch state
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != switchState) {
      switchState = reading;
    }
  }

  lastSwitchState = reading;
  return switchState == LOW; // LOW indicates the switch is triggered
}

// Function to log motor direction
void logDirection() {
  if (stepperX.direction()) {
    Serial.println("Motor moving clockwise (CW)");
  } else {
    Serial.println("Motor moving counterclockwise (CCW)");
  }
