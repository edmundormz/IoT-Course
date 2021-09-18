//Libraries to include
#include "WiFi.h"

//Global variables
const int ledPin =  13;                  // the number of the LED pin
int ledState = LOW;                      // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;                    // interval at which to blink (milliseconds)

// WiFi parameters to be configured
const char* ssid = "Your_WiFi_Name_Here";
const char* password = "Password_Goes_Here";

void setup(void)
{
  pinMode(ledPin, OUTPUT);    // set the digital pin as output:
  
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi, my IP address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;                // save the last time you blinked the LED
    if (ledState == LOW) {                         // if the LED is off turn it on and vice-versa:
      ledState = HIGH;
    } else {
      ledState = LOW;
    }    
    digitalWrite(ledPin, ledState);                // set the LED with the ledState of the variable:
  }
}
