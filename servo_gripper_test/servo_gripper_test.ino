#include <Servo.h>  // Include the Servo library

// Servo Variables
Servo gripperServo;
const int gripPin = 46;          // Servo signal pin (PWM capable pin on Mega2560)
const int gripOpenAngle = 90;    // Open position
const int gripCloseAngle = 180;  // Close position

// Motor Variables
const int pwmA = 5;     // PWM pin to control motor speed
const int in1A = 30;     // Direction pin 1
const int in2A = 32;    // Direction pin 2
const int stby = 34;     // Standby pin for TB6612 motor driver

const int MotorSpeedDrill = 250;   // Speed when drilling
const int MotorSpeedRetract = 200; // Speed when retracting

// LED Indicators
const int ledDrilling = 22;   // LED ON while drilling
const int ledRetract = 24;    // LED ON when retracting
const int ledGrip = 26;       // LED ON when gripping
const int ledRelease = 28;    // LED ON when releasing

// Operation Count
const int operationCount = 4; // Number of times to repeat the drill and grip sequence

void setup() {
  // Servo Setup
  gripperServo.attach(gripPin);

  // Motor Setup
  pinMode(pwmA, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(stby, OUTPUT);

  // LED Setup
  pinMode(ledDrilling, OUTPUT);
  pinMode(ledRetract, OUTPUT);
  pinMode(ledGrip, OUTPUT);
  pinMode(ledRelease, OUTPUT);

  Serial.begin(9600);  // For debugging
  Serial.println("System Initialized");
}

void loop() {
  for (int i = 0; i < operationCount; i++) {
    Serial.print("Starting operation #");
    Serial.println(i + 1);

    // Step 1: Drill operation (CW)
    drill();

    // Step 2: Retract operation (CCW)
    retract();

    // Step 3: Set STBY low (back to home position)
    stopMotor();
  }
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

void drill() {
  // Enable H-bridge and set motor to clockwise direction
  digitalWrite(stby, HIGH);
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);

  // Turn on drilling LED
  digitalWrite(ledDrilling, HIGH);
  Serial.println("Drilling...");

  // Set motor speed for drilling
  analogWrite(pwmA, MotorSpeedDrill);

  // Run motor for 5 seconds
  delay(3000);

  // Turn off drilling LED
  digitalWrite(ledDrilling, LOW);
  Serial.println("Drilling done.");
}

void retract() {
  // Enable H-bridge and set motor to counterclockwise direction
  digitalWrite(stby, HIGH);
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, HIGH);

  // Turn on retracting LED
  digitalWrite(ledRetract, HIGH);
  Serial.println("Retracting...");

  // Set motor speed for retracting
  analogWrite(pwmA, MotorSpeedRetract);

  // Run motor for 3 seconds
  delay(2000);

  // Turn off retracting LED
  digitalWrite(ledRetract, LOW);
  Serial.println("Retracting done.");
}

void stopMotor() {
  // Disable the motor driver (STBY low)
  digitalWrite(stby, LOW); //disable H bridge
  analogWrite(pwmA, 0);  // Ensure motor stops
  /*ISR to move axis to the next pot*/
  Serial.println("Motor stopped (back to home position).");
  delay(2000);  // Wait 2 seconds to simulate home position
}

void grip() {
  Serial.println("Gripping...");

  // Turn on gripping LED
  digitalWrite(ledGrip, HIGH);

  // Move servo to closed position
  gripperServo.write(gripCloseAngle);
  delay(1000);
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
  delay(1000);
  /*ISR to move axis to the next pot*/
  // Turn off releasing LED
  digitalWrite(ledRelease, LOW);
  Serial.println("Release complete.");
}       
