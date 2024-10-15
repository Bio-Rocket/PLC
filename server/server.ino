#include <SPI.h>
#include <Ethernet.h>
#include <P1AM.h>

byte mac[] = {
  0x60, 0x52, 0xD0, 0x07, 0x40, 0xC5
};
IPAddress ip(192, 168, 0, 70);

// Initialize the Ethernet server library
EthernetServer server(69);

uint8_t PBV1State = 0;

void setup() {
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }

  Ethernet.init(5);   // MKR ETH shield

  // Assert all modules unpowered
  P1.writeDiscrete(LOW, 5, 1);

  // Open serial communications and wait for port to open:
  // Serial.begin(9600);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

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
}

void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available() >= 2) {
        uint8_t command = client.read();
        uint8_t state = client.read();

        switch (command) {
          case 0:
            // Null command
            break;
          case 1:
            client.write(relay1State);
            break;
          case 10: // PBV1
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 1); // Turn off slot 5 channel 1
              PBV1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 1); // Turn on slot 5 channel 1
              PBV1State = 1;
            }
            break;
          default:
            client.write("Unknown command");
            break;
        }
      }
    }
    client.stop();
  }
}