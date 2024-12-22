#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>

#define STEP_PIN 2
#define DIR_PIN 3
#define PI 3.141593

AccelStepperWithDistance stepperZ(AccelStepperWithDistance::DRIVER, STEP_PIN, DIR_PIN);


void setup() {
  Serial.begin(9600);
  
  stepperZ.setMaxSpeed(500); //increase gradually
  stepperZ.setAcceleration(250); //increase gradually
  stepperZ.setStepsPerRotation(200);   // 1.8° stepper motor
  stepperZ.setMicroStep(16);           // 1/16 microstepping total 3200 uSteps
  stepperZ.setDistancePerRotation(8 * PI);  // 8mm per rotation
  stepperZ.setAnglePerRotation(360);   // Standard 360° per rotation
  
  // Move to 50mm
  stepperZ.runToNewDistance(50);
  Serial.print("Current position: ");
  Serial.println(stepper.getCurrentPositionDistance());
  
  // Move relatively by -20mm
  stepperZ.runRelative(-20);
  Serial.print("New position after relative move: ");
  Serial.println(stepper.getCurrentPositionDistance());
  
  // Move to 90° angle
  // stepperZ.runToNewAngle(90);
  // Serial.print("Position after moving to 90°: ");
  // Serial.println(stepper.getCurrentPositionDistance());
  
  // Set up a move to 100mm (but don't execute it yet)
  stepperZ.moveToDistance(100);
}

void loop() {
  // Execute the move set up in setup()
  if (stepperZ.distanceToGo() != 0) {
    stepperZ.run();
  }
}
