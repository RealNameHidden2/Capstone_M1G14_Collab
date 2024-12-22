#include <Servo.h>  // Include the Servo library

// Servo Variables
Servo gripperServo;
const int gripPin = 46;          // Servo signal pin (PWM capable pin on Mega2560)
const int gripOpenAngle = 0;    // Open position
const int gripCloseAngle = 270;  // Close position


// LED Indicators
const int ledGrip = 26;       // LED ON when gripping
const int ledRelease = 28;    // LED ON when releasing

// Operation Count
const int operationCount = 4; // Number of times to repeat the drill and grip sequence

void setup() {
  // Servo Setup
  gripperServo.attach(gripPin);

  // LED Setup
  pinMode(ledGrip, OUTPUT);
  pinMode(ledRelease, OUTPUT);

  Serial.begin(9600);  // For debugging
  Serial.println("System Initialized");
}

void loop() {
  for (int i = 0; i < operationCount; i++) {
    // Step 4: Grip operation
    grip();

    // Step 5: Release operation
    release();

    Serial.print("Completed operation #");
    Serial.println(i + 1);

    delay(2000);  // Wait 2 seconds before the next operation
  }

  // After completing all operations, stop further actions
  Serial.println("All operations completed.");
  while (true) {
    // Infinite loop to halt execution
  }
}

void grip() {
  Serial.println("Gripping...");

  // Turn on gripping LED
  digitalWrite(ledGrip, HIGH);

  // Move servo to closed position
  gripperServo.write(gripCloseAngle);
  delay(3000);
  /*ISR to move axis to the next pot*/
  // Turn off gripping LED
  digitalWrite(ledGrip, LOW);
  Serial.println("Grip complete.");
}

void release() {
  Serial.println("Releasing...");

  // Turn on releasing LED
  digitalWrite(ledRelease, HIGH);

  // Move servo to open position
  gripperServo.write(gripOpenAngle);
  delay(2000);
  /*ISR to move axis to the next pot*/
  // Turn off releasing LED
  digitalWrite(ledRelease, LOW);
  Serial.println("Release complete.");
}       
