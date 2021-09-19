// Libraries
#include <WiFi.h>
#include <WiFiClient.h>

#define RELAY 15 //Pin to be used to connect the relay module

// Set these to your desired credentials.
const char *ssid = "The Little WiFi";
const char *password = "ThePassword";

// Web server instance and port
WiFiServer server(80);

void setup() {

  pinMode(RELAY, OUTPUT);     //Set the relay GPIO as an output
  digitalWrite(RELAY, HIGH);  //Set this GPIO in HIGH state

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // Configure the WiFi Acces Point
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("The access point IP address is: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("--- Acces Point Started ---");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/L\">here</a> to turn ON the RELAY.<br>");
            client.print("Click <a href=\"/H\">here</a> to turn OFF the RELAY.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(RELAY, HIGH);               // GET /H turns the RELAY off
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(RELAY, LOW);                // GET /L turns the RELAY on
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected");
  }
}
