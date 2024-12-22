#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>

#define PI 3.141593

// Driver Pins
#define STEP_PIN_X 37
#define DIR_PIN_X 39
#define STEP_PIN_Y 41
#define DIR_PIN_Y 43
#define STEP_PIN_Z 45
#define DIR_PIN_Z 47

// Limit Switch Pins
#define LIMIT_SWITCH_X 49   // Limit switch for X-axis
#define LIMIT_SWITCH_Y 51   // Limit switch for Y-axis
#define LIMIT_SWITCH_Z 52   // Limit switch for Z-axis
#define START_BUTTON 40     // Start button

// Distance Limits for Axes (in mm)
#define LIMIT_X 150
#define LIMIT_Y 200
#define LIMIT_Z 80

#define Ms 16 // 1/16 microstepping total 3200 uSteps
#define DEBOUNCE_DELAY 30 // 30ms debounce delay

// Stepper Instances
AccelStepperWithDistance stepperX(AccelStepperWithDistance::DRIVER, STEP_PIN_X, DIR_PIN_X);
AccelStepperWithDistance stepperY(AccelStepperWithDistance::DRIVER, STEP_PIN_Y, DIR_PIN_Y);
AccelStepperWithDistance stepperZ(AccelStepperWithDistance::DRIVER, STEP_PIN_Z, DIR_PIN_Z);

void setup() {
  Serial.begin(9600);

  // Configure limit switches and start button
  pinMode(LIMIT_SWITCH_X, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_Y, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_Z, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);

  // Configure stepper motors
  configureStepper(stepperX, 500, 250); // X-axis
  configureStepper(stepperY, 600, 300); // Y-axis
  configureStepper(stepperZ, 400, 200); // Z-axis

  // Perform homing for all axes
  homeAxis(stepperX, LIMIT_SWITCH_X, LIMIT_X);
  homeAxis(stepperY, LIMIT_SWITCH_Y, LIMIT_Y);
  homeAxis(stepperZ, LIMIT_SWITCH_Z, LIMIT_Z);

  // Wait for the start button to begin operation
  waitForStart();
}

void loop() {
  // Move each axis to a specified target distance
  moveAxisWithinLimit(stepperX, LIMIT_X, 100); // X axis moves to 100mm
  moveAxisWithinLimit(stepperY, LIMIT_Y, 150); // Y axis moves to 150mm
  moveAxisWithinLimit(stepperZ, LIMIT_Z, 50);  // Z axis moves to 50mm
}

// Function to configure a stepper motor with custom speed and acceleration
void configureStepper(AccelStepperWithDistance &stepper, float maxSpeed, float acceleration) {
  stepper.setMaxSpeed(maxSpeed);       // Maximum speed
  stepper.setAcceleration(acceleration); // Acceleration
  stepper.setStepsPerRotation(200);    // 1.8° stepper motor
  stepper.setMicroStep(Ms);            // Microstepping
  stepper.setDistancePerRotation(8 * PI); // Shaft circumference
  stepper.setAnglePerRotation(360);    // Standard 360° per rotation
}

// Function to home an axis by triggering the limit switch twice
void homeAxis(AccelStepperWithDistance &stepper, int limitSwitchPin, float limit) {
  Serial.println("Homing axis...");

  // Move towards the limit switch slowly for the first trigger
  stepper.setMaxSpeed(100); // Slow speed for homing
  stepper.setAcceleration(50);

  while (digitalRead(limitSwitchPin) == HIGH) {
    // Move towards the limit switch
    stepper.moveToDistance(-1);
    stepper.run();

    // Check if the limit switch is triggered
    if (digitalRead(limitSwitchPin) == LOW) {
      delay(DEBOUNCE_DELAY); // 30ms debounce delay

      // Verify if the limit switch is still triggered after debounce
      if (digitalRead(limitSwitchPin) == LOW) {
        Serial.println("First trigger detected. Moving back slightly.");

        // Move slightly back
        stepper.moveToDistance(10);
        while (stepper.distanceToGo() != 0) {
          stepper.run();
        }
        delay(DEBOUNCE_DELAY); // 30ms debounce delay

        // Check for the second trigger
        while (digitalRead(limitSwitchPin) == HIGH) {
          stepper.moveToDistance(-1);
          stepper.run();

          if (digitalRead(limitSwitchPin) == LOW) {
            delay(DEBOUNCE_DELAY); // Debounce for the second trigger

            if (digitalRead(limitSwitchPin) == LOW) {
              // Limit switch triggered the second time
              stepper.setCurrentPosition(0);
              Serial.println("Homing complete. Position set to 0.");
              return;
            }
          }
        }
      }
    }
  }
}

// Function to move an axis to a specific target distance within its limit
void moveAxisWithinLimit(AccelStepperWithDistance &stepper, float limit, float targetDistance) {
  // Ensure the target distance is within the defined limit
  if (targetDistance > limit) {
    Serial.print("Target distance exceeds limit! Adjusting to limit: ");
    Serial.println(limit);
    targetDistance = limit;
  }

  if (stepper.getCurrentPositionDistance() < targetDistance) {
    stepper.moveToDistance(targetDistance); // Move to the target distance
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    Serial.print("Axis reached target: ");
    Serial.println(targetDistance);
  }

  // Move the axis back to 0 if it exceeds the target distance
  if (stepper.getCurrentPositionDistance() >= targetDistance) {
    stepper.moveToDistance(0); // Move back to 0
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    Serial.println("Axis returned to home position.");
  }
}

// Wait for the start button to be pressed
void waitForStart() {
  Serial.println("Waiting for start button...");
  while (digitalRead(START_BUTTON) == HIGH) {
    // Do nothing, wait for button press
  }
  Serial.println("Start button pressed. Resuming operation.");
}
