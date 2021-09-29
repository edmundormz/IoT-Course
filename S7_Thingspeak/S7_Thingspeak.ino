// Libraries to include
#include <NewPing.h>
#include <DHT.h>
#include <ThingSpeak.h>
#include <WiFi.h>

// Ultrasonic distance gpio configuration
#define TRIGGER_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200

// Humidity & Temperature gpio configuration
#define DHTPIN 14
#define DHTTYPE DHT11

// Global variables
float distance;
float humidity;
float heatIndex;
float temperature;

// WiFi parameters to be configured
const char* ssid = "IoT";
const char* password = "Esp32SIM800";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "xxxxxxxxx";

// Instances creation
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);//****???
      }
      Serial.println("\nConnected.");
    }

    Serial.print("Distance: ");
    distance = sonar.ping_cm();
    Serial.print(distance);
    Serial.println("cm");

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
    
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% ,Temperature: ");
    Serial.print(temperature);
    Serial.print("°C ");
    Serial.print("Heat index: ");
    Serial.print(heatIndex);
    Serial.println("°C");

    // set the fields with the values
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, heatIndex);
    ThingSpeak.setField(4, distance);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
