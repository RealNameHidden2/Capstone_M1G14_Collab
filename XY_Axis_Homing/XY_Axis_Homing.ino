// Define pin connections
#define STEP_PIN 37
#define DIR_PIN 39
#define ENABLE_PIN 41
#define Switch1_PIN 2
#define Switch2_PIN 3
#define Switch3_PIN 4

#define STEP_PIN_X 43
#define DIR_PIN_X 45
#define ENABLE_PIN_X 47

#define STEP_PIN_Z 49
#define DIR_PIN_Z 51
#define ENABLE_PIN_Z 53

// Variables for speed control
int stepDelay = 300; // Initial delay between steps (microseconds)
bool direction1 = false; // Initial direction (true = Backward, false = Forward)
bool direction2 = false; // Initial direction (true = Backward, false = Forward)
bool direction3 = false; // Initial direction (true = Backward, false = Forward)
volatile bool switch1Pressed = false; // Interrupt flag for switch 1 press
volatile bool switch2Pressed = false; // Interrupt flag for switch 2 press
volatile bool switch3Pressed = false; // Interrupt flag for switch 3 press

const int minStepDelay = 190; // Minimum allowable step delay
const int maxStepDelay = 2000; // Maximum allowable step delay

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(Switch1_PIN, INPUT_PULLUP);
  pinMode(Switch2_PIN, INPUT_PULLUP);
  pinMode(Switch3_PIN, INPUT_PULLUP);

  pinMode(STEP_PIN_X, OUTPUT);
  pinMode(DIR_PIN_X, OUTPUT);
  pinMode(ENABLE_PIN_X, OUTPUT);

  pinMode(STEP_PIN_Z, OUTPUT);
  pinMode(DIR_PIN_Z, OUTPUT);
  pinMode(ENABLE_PIN_Z, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW); // Enable the driver (LOW is active for DM542)
  digitalWrite(ENABLE_PIN_X, LOW);
  digitalWrite(ENABLE_PIN_Z, HIGH);
  attachInterrupt(digitalPinToInterrupt(Switch1_PIN), switch1PressedISR, FALLING); // Attach interrupt for Switch1_PIN
  attachInterrupt(digitalPinToInterrupt(Switch2_PIN), switch2PressedISR, FALLING); // Attach interrupt for Switch2_PIN
  attachInterrupt(digitalPinToInterrupt(Switch3_PIN), switch3PressedISR, FALLING); // Attach interrupt for Switch3_PIN

  Serial.begin(115200);
  Serial.println("Stepper motor control ready.");
  Serial.println("Enter 'speed_up' for faster, 'slow_down' for slower, 'backward' for backward, 'forward' for forward.");

  homing(); // Trigger homing sequence at the start
}

void loop() {
  // Check if all switches are pressed
  //if (digitalRead(Switch1_PIN) == HIGH && digitalRead(Switch2_PIN) == HIGH && digitalRead(Switch3_PIN) == HIGH) {
  //  Serial.println("All switches are pressed.");
  //  digitalWrite(ENABLE_PIN, LOW);
    //digitalWrite(ENABLE_PIN_X, LOW);
  //}
  stepDelay = 100;
  // Handle switch press interrupts
  if (switch1Pressed) {
    Serial.println("Switch 1 pressed.");
    switch1Pressed = false; // Clear interrupt flag
  }
  if (switch2Pressed) {
    Serial.println("Switch 2 pressed.");
    switch2Pressed = false; // Clear interrupt flag
  }
  if (switch3Pressed) {
    Serial.println("Switch 3 pressed.");
    switch3Pressed = false; // Clear interrupt flag
  }

  // Normal motor operation
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(stepDelay);

  digitalWrite(ENABLE_PIN_X, LOW);
  digitalWrite(DIR_PIN_X, LOW);
  digitalWrite(STEP_PIN_X, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(STEP_PIN_X, LOW);
  delayMicroseconds(stepDelay);

  // Constrain step delay
  stepDelay = constrain(stepDelay, minStepDelay, maxStepDelay);

  // Check for user input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "speed_up") {
      if (stepDelay > minStepDelay) {
        stepDelay -= 10;
        Serial.println("Faster");
        Serial.println(stepDelay);
      }
    } else if (input == "slow_down") {
      if (stepDelay < maxStepDelay) {
        stepDelay += 10;
        Serial.println("Slower");
        Serial.println(stepDelay);
      }
    } else if (input == "backward") {
      direction1 = true;
      Serial.println("Direction: Backward");
    } else if (input == "forward") {
      direction1 = false;
      Serial.println("Direction: Forward");
    } else {
      Serial.println("Invalid command. Use 'speed_up', 'slow_down', 'backward', or 'forward'.");
    }
  }
}

void homing() {
  Serial.println("Homing sequence started...");
  direction1 = true; // Set direction to backward for homing
  direction2 = false; // Set direction to backward for homing
  direction3 = true; // Set direction to backward for homing



  while (!switch1Pressed) { // Keep stepping until Switch 1 is pressed
    digitalWrite(DIR_PIN, direction1 ? HIGH : LOW);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(stepDelay); //maybe put all inside here 3 axes can move together
  }
  digitalWrite(ENABLE_PIN, HIGH);

  while (!switch2Pressed) { // Keep stepping until Switch 1 is pressed
    digitalWrite(DIR_PIN_X, direction2 ? HIGH : LOW);
    digitalWrite(STEP_PIN_X, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(STEP_PIN_X, LOW);
    delayMicroseconds(stepDelay);
  }
  digitalWrite(ENABLE_PIN_X, HIGH);
  Serial.println("Homing completed. Switch 1 & 2 & 3 pressed.");
  delay(500); // Short delay to stabilize
}

void switch1PressedISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 50) { // Debounce delay
    switch1Pressed = true;
  }
  lastInterruptTime = interruptTime;
}

void switch2PressedISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 50) { // Debounce delay
    switch2Pressed = true;
  }
  lastInterruptTime = interruptTime;
}

void switch3PressedISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 50) { // Debounce delay
    switch3Pressed = true;
  }
  lastInterruptTime = interruptTime;
}