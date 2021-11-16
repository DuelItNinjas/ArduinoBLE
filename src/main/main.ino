#include <SoftwareSerial.h>
#define IR_SEND_PIN 8
#include <IRremote.h>

#define BT_RX_PIN 2
#define BT_TX_PIN 3

#define LED_PIN LED_BUILTIN // LED for blink command

// See https://gist.github.com/francis2110/8f69843dd57ae07dce80 for IR codes for LG
#define IR_POWER    0x20DF10EF
#define IR_SETTINGS 0x20DFC23D
#define IR_CONFIRM  0x20DF22DD
#define IR_CANCEL   0x20DF14EB
#define IR_UP       0x20DF02FD
#define IR_DOWN     0x20DF827D
#define IR_LEFT     0x20DFE01F
#define IR_RIGHT    0x20DF609F

SoftwareSerial BTSerial(2, 3);
String BTBuffer;

void setup() {
  Serial.begin(9600);

  // Initialize HC-06 Bluetooth module
  BTSerial.begin(9600);
  BTBuffer = "";

  // Initialize IR transmitter
//  IrSender.begin(IR_SEND_PIN, false, 0);

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Enter AT commands");
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
    Serial.print("BT Buffer: ");
    Serial.println(BTBuffer);
    BTBuffer = "";
  }
}

// Execute the appropriate function based on the given Command value
void parseCommand() {
  // TODO: temporary
  if (BTBuffer == "OK") {
    irConfirm();
  }
}

// Blink the LED on the specified pin for a second
void blinkCmd() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

// Transmit 'confirm' IR code
void irConfirm() {
  IrSender.sendNECMSB(IR_SETTINGS, 32, false);
}
