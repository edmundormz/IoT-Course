//Libraries
#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid     = "Your_WiFi_Name_Here";
const char* password = "Password_Goes_Here";

//Instantiate web server
WiFiServer server(80);
#define LED  13
String state = "";

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  //Begin web server
  server.begin();
  Serial.println("***Web Server Started***");
}

void loop() {
  //Checks for web client connections
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.print("New web client: ");
  Serial.println(client.remoteIP());
  // Wait until client sends request
  while (!client.available()) {
    delay(1);
  }
  // Read instructions sent by the client
  String req = client.readStringUntil('\r');
  Serial.println(req);
  // Realiza la petición del cliente.
  if (req.indexOf("on2") != -1) {
    digitalWrite(LED, HIGH);
    state = "On";
  }
  if (req.indexOf("off2") != -1) {
    digitalWrite(LED, LOW);
    state = "Off";
  }

  //////////////////////////////////////////////
  // Web Page ////////////////////////////
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  Importante.
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta charset=utf-8></head>");
  client.println("<body><center><font face='Arial'>");
  client.println("<h1>Web Server with ESP32</h1>");
  client.println("<h1>YOUR NAME HERE</h1>");
  client.println("<h2><font color='#009900'>Webserver 1.0</font></h2>");
  client.println("<h3>Web Server Workshop</h3>");
  client.println("<br><br>");
  client.println("<a href='on2'><button>LED ON</button></a>");
  client.println("<a href='off2'><button>LED OFF</button></a>");
  client.println("<br><br>");
  client.println(state);
  client.println("</font></center></body></html>");
  Serial.print("Client disconnected: ");
  Serial.println(client.remoteIP());
  client.flush();
  client.stop();
}
