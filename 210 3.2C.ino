#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// WiFi credentials
char ssid[] = "Arjun";
char pass[] = "123456781";

WiFiClient client;
BH1750 lightMeter;

// IFTTT Configuration
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME  = "/trigger/lightsensor/with/key/pEXkMrzYLEzJv7mrOcg1_j6Dm2GT3vk10zDzXBdWke_";
String queryString = "?value1=57&value2=25"; // Initial query parameters

void setup() {
  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);

  WiFi.begin(ssid, pass);
  Wire.begin();

  // Connect to WiFi and IFTTT server
  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      break;
    } else {
      Serial.println("Connection failed");
    }
    delay(500);
  }
  lightMeter.begin();
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel();

  // Update the query parameters with the current light reading
  queryString = "?value1=" + String(lux);
  Serial.println(queryString);

  if (lux > 600) {
    // Send an HTTP GET request to IFTTT
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }

  // Reset the queryString and wait before the next reading
  queryString = "";
  delay(3000);
}
