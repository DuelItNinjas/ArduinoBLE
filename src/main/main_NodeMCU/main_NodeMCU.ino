#include "DHT.h"

#define DHT_TYPE DHT22

// Sensor pins
uint8_t DHT_PIN = D6;

// Global variables
DHT dht(DHT_PIN, DHT_TYPE);
float temperature;
float humidity;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(DHT_PIN, INPUT);
  dht.begin();
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  delay(1000);
  Serial.print("T: ");
  Serial.print(temperature);
  Serial.print(".C, H: ");
  Serial.print(humidity);
  Serial.println("%");
}
