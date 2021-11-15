#include <SoftwareSerial.h>

const byte BT_RX_PIN = 2;
const byte BT_TX_PIN = 3;
const byte LED_PIN = LED_BUILTIN; // LED for blink command

SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);
String BTBuffer;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter AT commands");

  // Initialize HC-06 Bluetooth module
  BTSerial.begin(9600);

  BTBuffer = "";
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Use AT+NAME"name" to modify the name and AT+PIN"pin" to modify the pin
  // See http://www.martyncurrey.com/arduino-and-hc-06-zs-040/ for list of commands
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }

  readFromBT();
}

// Read from HC-06 Bluetooth module
void readFromBT() {
  if (BTSerial.available()) {

    // Start reading and store in buffer
    while (BTSerial.available()) {
      BTBuffer += (char)BTSerial.read();
      delay(10); // Delay to let the arduino process the incoming bytes
    }

    // Parse and execute based on BT buffer
    parseCommand();

    // Print the buffer and clear
    Serial.println(BTBuffer);
    BTBuffer = "";
  }
}

// Execute the appropriate function based on the given Command value
void parseCommand() {
  if (BTBuffer == "OK") {
    blinkCmd();
  }
}

// Simple command for blinking the LED on the specified pin for half a second
void blinkCmd() {
  digitalWrite(LED_PIN, HIGH);
  delay(5000);
  digitalWrite(LED_PIN, LOW);
}
