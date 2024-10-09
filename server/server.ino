#include <SPI.h>
#include <Ethernet.h>
#include <P1AM.h>

byte mac[] = {
  0x60, 0x52, 0xD0, 0x07, 0x40, 0xC5
};
IPAddress ip(192, 168, 0, 70);

// Initialize the Ethernet server library
EthernetServer server(69);

int8_t PBV1State = 0;
int8_t PBV2State = 0;
int8_t PBV3State = 0;
int8_t PBV4State = 0;
int8_t PBV5State = 0;
int8_t PBV6State = 0;
int8_t PBV7State = 0;
int8_t PBV8State = 0;

int8_t PMP1State = 0;
int8_t PMP2State = 0;
int8_t PMP3State = 0;

int8_t IGN1State = 0;
int8_t IGN2State = 0;

const char P1_04THM_CONFIG_1[] = { 0x40, 0x03, 0x60, 0x03, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char P1_04THM_CONFIG_2[] = { 0x40, 0x03, 0x60, 0x03, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char P1_04THM_CONFIG_3[] = { 0x40, 0x02, 0x60, 0x05, 0x21, 0x05, 0x22, 0x09, 0x23, 0x09, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const char P1_08ADL_2_CONFIG[] = { 0x40, 0x07 };
const char P1_04ADL_2_CONFIG[] = { 0x40, 0x00 };

void setup() {

  // TODO: Assert Prefire State

  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }

  Ethernet.init(5);   // MKR ETH shield

  P1.configureModule(P1_04THM_CONFIG_1, 1)
  P1.configureModule(P1_04THM_CONFIG_2, 2)
  P1.configureModule(P1_04THM_CONFIG_3, 3)

  P1.configureModule(P1_08ADL_2_CONFIG, 4)
  P1.configureModule(P1_04ADL_2_CONFIG, 5)

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
            // Reset (doesnt exist lol)
            break;
          case 2:
            int16_t TC1 = (int)(P1.readTemperature(1, 1) * 100);
            int16_t TC2 = (int)(P1.readTemperature(1, 2) * 100);
            int16_t TC3 = (int)(P1.readTemperature(1, 3) * 100);
            int16_t TC4 = (int)(P1.readTemperature(1, 4) * 100);
            int16_t TC5 = (int)(P1.readTemperature(2, 1) * 100);
            int16_t TC6 = (int)(P1.readTemperature(2, 2) * 100);
            int16_t TC7 = (int)(P1.readTemperature(2, 3) * 100);
            int16_t TC8 = (int)(P1.readTemperature(2, 4) * 100);
            int16_t TC9 = (int)(P1.readTemperature(3, 1) * 100);

            int16_t LC1 = (int)(P1.readTemperature(3, 2) * 100);
            int16_t LC2 = (int)(P1.readTemperature(3, 3) * 100);

            int PT1_counts = P1.readAnalog(4, 1);
            int PT2_counts = P1.readAnalog(4, 2);
            int PT3_counts = P1.readAnalog(4, 3);
            int PT4_counts = P1.readAnalog(4, 4);
            int PT5_counts = P1.readAnalog(4, 5);
            int PT6_counts = P1.readAnalog(4, 6);
            int PT15_counts = P1.readAnalog(4, 7);
            int PT16_counts = P1.readAnalog(4, 8);

            int16_t PT1_pressure = (int)(((float)PT15_counts/8191.0) * 10);
            int16_t PT2_pressure = (int)(((float)PT16_counts/8191.0) * 10);
            int16_t PT3_pressure = (int)(((float)PT1_counts/8191.0) * 10);
            int16_t PT4_pressure = (int)(((float)PT2_counts/8191.0) * 10);
            int16_t PT5_pressure = (int)(((float)PT3_counts/8191.0) * 10);
            int16_t PT6_pressure = (int)(((float)PT4_counts/8191.0) * 10);
            int16_t PT15_pressure = (int)(((float)PT5_counts/8191.0) * 10);
            int16_t PT16_pressure = (int)(((float)PT6_counts/8191.0) * 10);

            int16_t TcData[] = {TC1, TC2, TC3, TC4, TC5, TC6, TC7, TC8, TC9, LC1, LC2}
            int16_t PtData[] = {PT1_pressure, PT2_pressure, PT3_pressure, PT4_pressure, PT5_pressure, PT6_pressure, PT15_pressure, PT16_pressure}
            int8_t valveData[] = {PBV1State, PBV2State, PBV3State, PBV4State, PBV5State, PBV6State, PBV7State, PBV8State,
              PMP1State, PMP2State, PMP3State,
              IGN1State, IGN2State}

            client.write((uint8_t*)TcData, sizeof(TcData));
            client.write((uint8_t*)PtData, sizeof(PtData));
            client.write((uint8_t*)valveData, sizeof(valveData));
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

          case 11: // PBV2
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 2); // Turn off slot 5 channel 2
              PBV2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 2); // Turn on slot 5 channel 2
              PBV2State = 1;
            }
            break;

          case 12: // PBV3
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 3); // Turn off slot 5 channel 3
              PBV3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 3); // Turn on slot 5 channel 3
              PBV3State = 1;
            }
            break;

          case 13: // PBV4
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 4); // Turn off slot 5 channel 4
              PBV4State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 4); // Turn on slot 5 channel 4
              PBV4State = 1;
            }
            break;

          case 14: // PBV5
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 1); // Turn off slot 6 channel 1
              PBV5State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 1); // Turn on slot 6 channel 1
              PBV5State = 1;
            }
            break;

          case 15: // PBV6
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 2); // Turn off slot 6 channel 2
              PBV6State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 2); // Turn on slot 6 channel 2
              PBV6State = 1;
            }
            break;

          case 16: // PBV7
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 3); // Turn off slot 6 channel 3
              PBV7State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 3); // Turn on slot 6 channel 3
              PBV7State = 1;
            }
            break;

          case 17: // PBV8
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 4); // Turn off slot 6 channel 4
              PBV8State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 4); // Turn on slot 6 channel 4
              PBV8State = 1;
            }
            break;

          case 18: // PMP1
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 5); // Turn off slot 6 channel 5
              PMP1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 5); // Turn on slot 6 channel 5
              PMP1State = 1;
            }
            break;

          case 19: // PMP2
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 6); // Turn off slot 6 channel 6
              PMP2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 6); // Turn on slot 6 channel 6
              PMP2State = 1;
            }
            break;

          case 20: // PMP3
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 7); // Turn off slot 6 channel 7
              PMP3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 7); // Turn on slot 6 channel 7
              PMP3State = 1;
            }
            break;

          case 21: // IGN1
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 1); // Turn off slot 7 channel 1
              IGN1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 1); // Turn on slot 7 channel 1
              IGN1State = 1;
            }
            break;

          case 22: // IGN2
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 2); // Turn off slot 7 channel 2
              IGN2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 2); // Turn on slot 7 channel 2
              IGN2State = 1;
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