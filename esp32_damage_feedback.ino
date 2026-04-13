/*
  esp32_damage_feedback.ino
  -------------------------
  Listens for a trigger byte ('1') over Serial from the Python bridge,
  then pulses a relay (and buzzer) for 500ms on GPIO pin 7.

  Wiring:
    ESP32 3.3V  →  Relay VCC   (use 5V pin if your relay needs 5V)
    ESP32 GND   →  Relay GND
    ESP32 GPIO7 →  Relay IN
    Relay COM   →  Buzzer +
    Buzzer -    →  GND

  Notes:
    - ESP32 GPIO pins are 3.3V logic. Most relay modules accept this,
      but double-check your specific module.
    - If your relay is active-LOW, swap HIGH/LOW in triggerRelay().
    - Baud rate must match arduino_bridge.py (default: 9600).
*/

// ── CONFIG ────────────────────────────────────────────────────────────────────

#define RELAY_PIN     7       // GPIO pin connected to relay IN
#define RELAY_ON      HIGH    // set to LOW if your relay module is active-LOW
#define RELAY_OFF     LOW
#define TRIGGER_MS    500     // how long the relay stays on (milliseconds)
#define BAUD_RATE     9600    // must match arduino_bridge.py

// ─────────────────────────────────────────────────────────────────────────────

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);   // make sure relay starts in OFF state

  Serial.begin(BAUD_RATE);

  // small delay to let serial settle — ESP32 bootloader can spit noise
  delay(500);
  Serial.println("[ESP32] Ready. Waiting for trigger...");
}

void triggerRelay() {
  digitalWrite(RELAY_PIN, RELAY_ON);
  delay(TRIGGER_MS);
  digitalWrite(RELAY_PIN, RELAY_OFF);
}

void loop() {
  if (Serial.available() > 0) {
    char incoming = Serial.read();

    if (incoming == '1') {
      Serial.println("[ESP32] Trigger received — activating relay!");
      triggerRelay();
    }
  }
}
