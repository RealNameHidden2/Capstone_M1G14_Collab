const int relayPin = 7; // Pin connected to the relay

void setup() {
  pinMode(relayPin, OUTPUT); // Set relay pin as output
  digitalWrite(relayPin, LOW); // Ensure relay is off initially

  // Turn relay ON for 5 seconds
  digitalWrite(relayPin, HIGH);
  delay(5000); // Wait for 5 seconds
  digitalWrite(relayPin, LOW); // Turn relay OFF
}

void loop() {
  // Enter an infinite loop and wait for the reset button to be pressed
  while (true) {
    // Do nothing, wait for reset
  }
}
