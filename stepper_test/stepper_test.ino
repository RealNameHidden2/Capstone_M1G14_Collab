#include <AccelStepper.h>
#include <AccelStepperWithDistance.h>

#define STEP_PIN 2
#define DIR_PIN 3

AccelStepperWithDistance stepperz(AccelStepperWithDistance::DRIVER, STEP_PIN, DIR_PIN);
AccelStepperWithDistance stepper1(AccelStepperWithDistance::DRIVER, STEP_PIN, DIR_PIN);
AccelStepperWithDistance stepper2(AccelStepperWithDistance::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(9600);
  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  stepper.setStepsPerRotation(200);   // 1.8° stepper motor
  stepper.setMicroStep(16);           // 1/16 microstepping
  stepper.setDistancePerRotation(8);  // 8mm per rotation
  stepper.setAnglePerRotation(360);   // Standard 360° per rotation
  
  // Move to 50mm
  stepper.runToNewDistance(50);
  Serial.print("Current position: ");
  Serial.println(stepper.getCurrentPositionDistance());
  
  // Move relatively by -20mm
  stepper.runRelative(-20);
  Serial.print("New position after relative move: ");
  Serial.println(stepper.getCurrentPositionDistance());
  
  // Move to 90° angle
  stepper.runToNewAngle(90);
  Serial.print("Position after moving to 90°: ");
  Serial.println(stepper.getCurrentPositionDistance());
  
  // Set up a move to 100mm (but don't execute it yet)
  stepper.moveToDistance(100);
}

void loop() {
  // Execute the move set up in setup()
  if (stepper.distanceToGo() != 0) {
    stepper.run();
  }
}
