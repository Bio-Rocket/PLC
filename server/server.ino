#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0x60, 0x52, 0xD0, 0x07, 0x40, 0xC5
};
IPAddress ip(192, 168, 0, 70);

// Initialize the Ethernet server library
EthernetServer server(69);

uint8_t relay1State = 0;
uint8_t relay2State = 0;

void setup() {
  uint8_t P1.init();
  Ethernet.init(5);   // MKR ETH shield

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available() == 2) {
        uint8_t command = client.read();
        uint8_t state = client.read();

        switch (command) {
          case 0:
            //null command
            break;
          case 1:
            //reset
            break;
          case 2:
            client.write(relay1State);
            break;
          case 10:
            if (state == 0) {
              P1.writeDiscrete(LOW,5,1); // Turn off slot 5 channel 1
              relay1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH,5,1); // Turn on slot 5 channel 1
              relay1State = 1;
            }
            break;
          default:
            Serial.println("Unknown command");
            break;
        }
    client.stop();
    Serial.println("client disconnected");
  }
}