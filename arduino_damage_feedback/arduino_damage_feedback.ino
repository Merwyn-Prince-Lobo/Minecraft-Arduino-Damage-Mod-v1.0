/*
  arduino_damage_feedback.ino
  ----------------------------
  Receives a '1' byte over Serial from the Python bridge and
  triggers a relay for TRIGGER_MS milliseconds.
  
  The relay switches power to BOTH the buzzer and haptic motor.
  
  Wiring:
    Pin 7  → Relay IN
    Relay COM  → + power rail (5V from Arduino or external supply)
    Relay NO   → Buzzer (+) AND Haptic motor (+) [connect both in parallel]
    Buzzer (–) → GND
    Haptic (–) → GND

  Note: If your buzzer/haptic motor together draw more than ~300mA,
        use an external 5V supply switched by the relay instead of
        Arduino's onboard 5V pin.
*/

const int RELAY_PIN   = 7;
const int TRIGGER_MS  = 500;  // how long relay stays on per hit (ms)

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // relay off at startup
  Serial.println("Arduino Damage Feedback ready.");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '1') {
      // Trigger relay → buzzer + haptic ON
      digitalWrite(RELAY_PIN, HIGH);
      delay(TRIGGER_MS);
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}
