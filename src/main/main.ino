#include <SoftwareSerial.h>

const byte BT_RX_PIN = 2;
const byte BT_TX_PIN = 3;
const byte LED_PIN = LED_BUILTIN; // LED for blink command

const byte NONE_CMD = 0;
const byte BLINK_CMD = 1;

SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);
byte Command;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter AT commands");

  // Initialize HC-06 Bluetooth module
  BTSerial.begin(9600);

  Command = NONE_CMD;
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (BTSerial.available()) {
    // TODO: read and parse into commands
    Serial.write(BTSerial.read());
  }

  // Use AT+NAME"name" to modify the name and AT+PIN"pin" to modify the pin
  // See http://www.martyncurrey.com/arduino-and-hc-06-zs-040/ for list of commands
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

// Simple command for blinking the LED on the specified pin for half a second
void blinkCmd() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
}
