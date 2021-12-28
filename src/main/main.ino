#include <SoftwareSerial.h>
#include <DHT.h>
#define IR_SEND_PIN 8
#include <IRremote.h>

#define BT_RX_PIN 2
#define BT_TX_PIN 3
#define DHT_TYPE  DHT22 // AM2302
#define DHT_PIN   4 // Temperature and Humidity
#define LED_PIN   5 // LED for blink command

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
DHT dht(DHT_PIN, DHT_TYPE);
float temp;
float hum;

void setup() {
  Serial.begin(9600);

  // Initialize HC-06 Bluetooth module
  BTSerial.begin(9600);
  BTBuffer = "";

  // Initialize Temp/Hum sensor
  dht.begin();

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

  // Read from HC-06 Bluetooth module
  if (BTSerial.available()) {

    // Start reading and store in buffer
    while (BTSerial.available()) {

      char temp = (char)BTSerial.read();

      // Ignore newline or carriage return characters for ease of parsing command
      if (temp != '\n' && temp != '\r') {
        BTBuffer += temp;
      }
      delay(10); // Delay to let the arduino process the incoming bytes
    }

    // Parse and execute based on BT buffer
    parseCommand();

    // Print the buffer and clear
    Serial.print("BT Buffer: ");
    Serial.println(BTBuffer);
    BTBuffer = "";
    BTSerial.println("OK");
  }
}

// Execute the appropriate function based on the given Command value
void parseCommand() {
  if (BTBuffer == "ok") {
  } else if (BTBuffer == "blnk") {
    blinkCmd();
  } else if (BTBuffer == "tmp") {
    readTemp();
    BTSerial.print("Humidity: ");
    BTSerial.print(hum);
    BTSerial.print("%  Temperature: ");
    BTSerial.println(temp);
  } else if (BTBuffer == "irpwr") {
    IrSender.sendNECMSB(IR_POWER, 32, false);
  } else if (BTBuffer == "irsttng") {
    IrSender.sendNECMSB(IR_SETTINGS, 32, false);
  } else if (BTBuffer == "ircnfrm") {
    IrSender.sendNECMSB(IR_CONFIRM, 32, false);
  } else if (BTBuffer == "ircncl") {
    IrSender.sendNECMSB(IR_CANCEL, 32, false);
  } else if (BTBuffer == "iru") {
    IrSender.sendNECMSB(IR_UP, 32, false);
  } else if (BTBuffer == "ird") {
    IrSender.sendNECMSB(IR_DOWN, 32, false);
  } else if (BTBuffer == "irl") {
    IrSender.sendNECMSB(IR_LEFT, 32, false);
  } else if (BTBuffer == "irr") {
    IrSender.sendNECMSB(IR_RIGHT, 32, false);
  } else {
    BTSerial.print("Could not recognize command: ");
    BTSerial.println(BTBuffer);
  }
}

// Blink the LED on the specified pin for a second
void blinkCmd() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

void readTemp() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  delay(1000); // It takes a while to read from these sensors
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%  Temperature: ");
  Serial.println(temp);
}
