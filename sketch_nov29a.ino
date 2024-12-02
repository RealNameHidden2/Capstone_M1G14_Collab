#include <Servo.h>  // Include the Servo library

Servo gripperServo;  // Create a Servo object for the gripper

const int gripPin = 9;          // Define the pin where the servo is connected
const int gripOpenAngle = 90;   // Define the angle for the gripper open position
const int gripCloseAngle = 180;  // Define the angle for the gripper closed position (adjust as needed)
const int itemWeight = 300;     // Define the weight of the item (300g)
const int operationCount = 6;   // Define how many times the operation should run

void setup() {
  gripperServo.attach(gripPin);  // Attach the gripper servo to the defined pin
  Serial.begin(9600);            // Start the Serial communication for debugging
  Serial.println("Gripper Control Initialized");
}

void loop() {
  for (int i = 0; i < operationCount; i++) {
    Serial.print("Operation #");
    Serial.println(i + 1);
    pickUpItem();
    delay(2000);  // Wait for 2 seconds
    releaseItem();
    delay(2000);  // Wait for 2 seconds
  }

  // After completing the operations, stop the loop
  Serial.println("Completed all operations");
  while (true) {
    // Infinite loop to stop further actions
  }
}

void pickUpItem() {
  Serial.println("Moving to open position");
  gripperServo.write(gripOpenAngle);  // Move the gripper to the open position
  delay(1000);  // Wait for 1 second to allow the gripper to open

  Serial.println("Moving to closed position to grip item");
  gripperServo.write(gripCloseAngle); // Move the gripper to the closed position
  delay(1000);  // Wait for 1 second to allow the gripper to close around the item

  // Additional code can be added here to lift the gripper if needed
  // For example, moving a second servo or motor to lift the gripper
  
  Serial.println("Item gripped successfully");
}

void releaseItem() {
  Serial.println("Moving to open position to release item");
  gripperServo.write(gripOpenAngle);  // Move the gripper to the open position
  delay(1000);  // Wait for 1 second to release the item

  Serial.println("Item released successfully");
}
