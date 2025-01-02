#include <AccelStepper.h>

#define STEP_PIN 37
#define DIR_PIN 39
#define LIMIT_SWITCH_PIN 49
#define DEBOUNCE_DELAY 50

// Constants for Stepper Motor Configuration
#define MICROSTEP_RESOLUTION 2 // Microstepping resolution (e.g., 1/16)

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

// Function to home the X-axis using the limit switch
void homeXAxis() {
  Serial.println("Starting homing process...");

  // Move towards the limit switch until triggered
  stepperX.setCurrentPosition(0);
  stepperX.moveTo(-10000);

  while (!isLimitSwitchTriggered()) {
    stepperX.run();
    logDirection();
  }

  stepperX.stop();
  delay(100);

  // Move away from the limit switch
  stepperX.move(100);
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
    logDirection();
  }

  // Trigger the limit switch again for precision
  stepperX.moveTo(-10000);
  while (!isLimitSwitchTriggered()) {
    stepperX.run();
    logDirection();
  }

  stepperX.stop();
  stepperX.setCurrentPosition(0);
  Serial.println("Homing complete. Position reset to 0.");
}

// Function to move to a position in mm
void moveToMm(float distance_mm) {
  int targetSteps = static_cast<int>(distance_mm * stepsPerMm);
  stepperX.moveTo(targetSteps);
}

// Function to log motor direction
void logDirection() {
  if (stepperX.direction()) {
    Serial.println("Motor moving clockwise (CW)");
  } else {
    Serial.println("Motor moving counterclockwise (CCW)");
  }
}
