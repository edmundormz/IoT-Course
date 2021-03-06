// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800       // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024   // Set RX buffer to 1Kb
#define SMS_TARGET  "+523331323435" // The number should be +52 (for Mexico) and 10 digit number

// Libraries
#include <TinyGsmClient.h>

// Configure TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT  Serial1

// Create modem object
TinyGsm modem(SerialAT);

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  SerialMon.println("Initializing modem...");
  //modem.restart();
  modem.init();
  delay(3000);
  String smsMessage = "Your_Name_Here";
  if(modem.sendSMS(SMS_TARGET, smsMessage)){
    SerialMon.print("SMS Sent: ");
    SerialMon.println(smsMessage);
  }
  else{
    SerialMon.println("SMS failed to send");
  }
}

void loop() {
  delay(1);
}
