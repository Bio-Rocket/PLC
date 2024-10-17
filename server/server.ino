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

int8_t HeaterState = 0;

int PT1_counts, PT2_counts, PT3_counts, PT4_counts, PT5_counts, PT6_counts, PT15_counts, PT16_counts;
int16_t PT1_pressure, PT2_pressure, PT3_pressure, PT4_pressure, PT5_pressure, PT6_pressure, PT15_pressure, PT16_pressure;
int16_t thermocouple1, thermocouple2, thermocouple3, thermocouple4, thermocouple5, thermocouple6, thermocouple7, thermocouple8, thermocouple9 loadCell1, leadCell2;
int16_t PtData[8];
int16_t TcData[11];
int8_t valveData[14];

const char P1_04THM_CONFIG_1[] = { 0x40, 0x03, 0x60, 0x03, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char P1_04THM_CONFIG_2[] = { 0x40, 0x03, 0x60, 0x03, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char P1_04THM_CONFIG_3[] = { 0x40, 0x02, 0x60, 0x05, 0x21, 0x05, 0x22, 0x09, 0x23, 0x09, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const char P1_08ADL_2_CONFIG[] = { 0x40, 0x07 };
const char P1_04ADL_2_CONFIG[] = { 0x40, 0x00 };

void setup() {
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }

  Ethernet.init(5);   // MKR ETH shield

  P1.configureModule(P1_04THM_CONFIG_1, 1);
  P1.configureModule(P1_04THM_CONFIG_2, 2);
  P1.configureModule(P1_04THM_CONFIG_3, 3);

  P1.configureModule(P1_08ADL_2_CONFIG, 4);
  P1.configureModule(P1_04ADL_2_CONFIG, 5);

  // Assert all modules unpowered
  P1.writeDiscrete(LOW, 6, 1);
  P1.writeDiscrete(LOW, 6, 2);
  P1.writeDiscrete(LOW, 6, 3);
  P1.writeDiscrete(LOW, 6, 4);
  P1.writeDiscrete(LOW, 7, 1);
  P1.writeDiscrete(LOW, 7, 2);
  P1.writeDiscrete(LOW, 7, 3);
  P1.writeDiscrete(LOW, 7, 4);
  P1.writeDiscrete(LOW, 7, 5);
  P1.writeDiscrete(LOW, 7, 6);
  P1.writeDiscrete(LOW, 7, 7);
  P1.writeDiscrete(LOW, 8, 1);
  P1.writeDiscrete(LOW, 8, 2);
  P1.writeDiscrete(LOW, 8, 3);

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
            thermocouple1 = (int)(P1.readTemperature(1, 1) * 100);
            thermocouple2 = (int)(P1.readTemperature(1, 2) * 100);
            thermocouple3 = (int)(P1.readTemperature(1, 3) * 100);
            thermocouple4 = (int)(P1.readTemperature(1, 4) * 100);
            thermocouple5 = (int)(P1.readTemperature(2, 1) * 100);
            thermocouple6 = (int)(P1.readTemperature(2, 2) * 100);
            thermocouple7 = (int)(P1.readTemperature(2, 3) * 100);
            thermocouple8 = (int)(P1.readTemperature(2, 4) * 100);
            thermocouple9 = (int)(P1.readTemperature(3, 1) * 100);

            loadCell1 = (int)(P1.readTemperature(3, 2) * 100);
            loadCell2 = (int)(P1.readTemperature(3, 3) * 100);

            PT1_counts = P1.readAnalog(4, 1);
            PT2_counts = P1.readAnalog(4, 2);
            PT3_counts = P1.readAnalog(4, 3);
            PT4_counts = P1.readAnalog(4, 4);
            PT5_counts = P1.readAnalog(4, 5);
            PT6_counts = P1.readAnalog(4, 6);
            PT15_counts = P1.readAnalog(4, 7);
            PT16_counts = P1.readAnalog(4, 8);

            PT1_pressure = (int)(((float)PT15_counts/8191.0) * 10);
            PT2_pressure = (int)(((float)PT16_counts/8191.0) * 10);
            PT3_pressure = (int)(((float)PT1_counts/8191.0) * 10);
            PT4_pressure = (int)(((float)PT2_counts/8191.0) * 10);
            PT5_pressure = (int)(((float)PT3_counts/8191.0) * 10);
            PT6_pressure = (int)(((float)PT4_counts/8191.0) * 10);
            PT15_pressure = (int)(((float)PT5_counts/8191.0) * 10);
            PT16_pressure = (int)(((float)PT6_counts/8191.0) * 10);

            PtData[0] = PT1_pressure;
            PtData[1] = PT2_pressure;
            PtData[2] = PT3_pressure;
            PtData[3] = PT4_pressure;
            PtData[4] = PT5_pressure;
            PtData[5] = PT6_pressure;
            PtData[6] = PT15_pressure;
            PtData[7] = PT16_pressure;

            TcData[0] = thermocouple1;
            TcData[1] = thermocouple2;
            TcData[2] = thermocouple3;
            TcData[3] = thermocouple4;
            TcData[4] = thermocouple5;
            TcData[5] = thermocouple6;
            TcData[6] = thermocouple7;
            TcData[7] = thermocouple8;
            TcData[8] = thermocouple9;
            TcData[9] = loadCell1;
            TcData[10] = loadCell2;

            valveData[0] = PBV1State;
            valveData[1] = PBV2State;
            valveData[2] = PBV3State;
            valveData[3] = PBV4State;
            valveData[4] = PBV5State;
            valveData[5] = PBV6State;
            valveData[6] = PBV7State;
            valveData[7] = PBV8State;
            valveData[8] = PMP1State;
            valveData[9] = PMP2State;
            valveData[10] = PMP3State;
            valveData[11] = IGN1State;
            valveData[12] = IGN2State;
            valveData[13] = HeaterState;

            client.write((uint8_t*)TcData, sizeof(TcData));
            client.write((uint8_t*)PtData, sizeof(PtData));
            client.write((uint8_t*)valveData, sizeof(valveData));
            break;

          case 10: // PBV1
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 1); // Turn off slot 6 channel 1
              PBV1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 1); // Turn on slot 6 channel 1
              PBV1State = 1;
            }
            break;

          case 11: // PBV2
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 2); // Turn off slot 6 channel 2
              PBV2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 2); // Turn on slot 6 channel 2
              PBV2State = 1;
            }
            break;

          case 12: // PBV3
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 3); // Turn off slot 6 channel 3
              PBV3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 3); // Turn on slot 6 channel 3
              PBV3State = 1;
            }
            break;

          case 13: // PBV4
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 4); // Turn off slot 6 channel 4
              PBV4State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 4); // Turn on slot 6 channel 4
              PBV4State = 1;
            }
            break;

          case 14: // PBV5
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 1); // Turn off slot 7 channel 1
              PBV5State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 1); // Turn on slot 7 channel 1
              PBV5State = 1;
            }
            break;

          case 15: // PBV6
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 2); // Turn off slot 7 channel 2
              PBV6State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 2); // Turn on slot 7 channel 2
              PBV6State = 1;
            }
            break;

          case 16: // PBV7
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 3); // Turn off slot 7 channel 3
              PBV7State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 3); // Turn on slot 7 channel 3
              PBV7State = 1;
            }
            break;

          case 17: // PBV8
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 4); // Turn off slot 7 channel 4
              PBV8State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 4); // Turn on slot 7 channel 4
              PBV8State = 1;
            }
            break;

          case 18: // PMP1
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 5); // Turn off slot 7 channel 5
              PMP1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 5); // Turn on slot 7 channel 5
              PMP1State = 1;
            }
            break;

          case 19: // PMP2
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 6); // Turn off slot 7 channel 6
              PMP2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 6); // Turn on slot 7 channel 6
              PMP2State = 1;
            }
            break;

          case 20: // PMP3
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 7); // Turn off slot 7 channel 7
              PMP3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 7); // Turn on slot 7 channel 7
              PMP3State = 1;
            }
            break;

          case 21: // IGN1
            if (state == 0) {
              P1.writeDiscrete(LOW, 8, 1); // Turn off slot 8 channel 1
              IGN1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 8, 1); // Turn on slot 8 channel 1
              IGN1State = 1;
            }
            break;

          case 22: // IGN2
            if (state == 0) {
              P1.writeDiscrete(LOW, 8, 2); // Turn off slot 8 channel 2
              IGN2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 8, 2); // Turn on slot 8 channel 2
              IGN2State = 1;
            }
            break;

          case 23: // Heater
            if (state == 0) {
              P1.writeDiscrete(LOW, 8, 3); // Turn off slot 8 channel 3
              HeaterState = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 8, 3); // Turn on slot 8 channel 3
              HeaterState = 1;
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