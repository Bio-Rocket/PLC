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
  int8_t PBV9State = 0;
  int8_t PBV10State = 0;
  int8_t PBV11State = 0;

  int8_t SOL1State = 0;
  int8_t SOL2State = 0;
  int8_t SOL3State = 0;
  int8_t SOL4State = 0;
  int8_t SOL5State = 0;
  int8_t SOL6State = 0;
  int8_t SOL7State = 0;
  int8_t SOL8State = 0;
  int8_t SOL9State = 0;

  int8_t HeaterState = 0;

  int8_t PMP3State = 0;

  int8_t IGN1State = 0;
  int8_t IGN2State = 0;

  int16_t LC1, LC2, LC7;
  int16_t PT1_counts, PT2_counts, PT3_counts, PT4_counts, PT5_counts, PT13_counts, PT14_counts;
  int16_t PT1_pressure, PT2_pressure, PT3_pressure, PT4_pressure, PT5_pressure, PT13_pressure, PT14_pressure;
  int16_t TC1_data, TC2_data, TC3_data, TC4_data, TC5_data, TC6_data, TC7_data, TC8_data, TC9_data;
  int16_t PtData[7];
  int16_t TcData[9];
  int16_t LcData[3];
  int8_t valveData[24];

  const char P1_04THM_CONFIG_1[] = { 0x40, 0x03, 0x60, 0x01, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  const char P1_04THM_CONFIG_2[] = { 0x40, 0x03, 0x60, 0x01, 0x21, 0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  const char P1_04THM_CONFIG_3[] = { 0x40, 0x03, 0x60, 0x01, 0x21, 0x01, 0x22, 0x0a, 0x23, 0x0a, 0x24, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  const char P1_08ADL_2_CONFIG[] = { 0x40, 0x06 };

void setup() {
  while (!P1.init()){ 
    ; //Wait for Modules to Sign on   
  }

  Ethernet.init(5);   // MKR ETH shield

  P1.configureModule(P1_04THM_CONFIG_1, 1);
  P1.configureModule(P1_04THM_CONFIG_2, 2);
  P1.configureModule(P1_04THM_CONFIG_3, 3);
  P1.configureModule(P1_08ADL_2_CONFIG, 4);

  // Assert all modules unpowered
  P1.writeDiscrete(LOW, 5, 1);
  P1.writeDiscrete(LOW, 5, 2);
  P1.writeDiscrete(LOW, 5, 3);
  P1.writeDiscrete(LOW, 5, 4);
  P1.writeDiscrete(LOW, 5, 5);
  P1.writeDiscrete(LOW, 5, 6);
  P1.writeDiscrete(LOW, 5, 7);
  P1.writeDiscrete(LOW, 5, 8);
  P1.writeDiscrete(LOW, 6, 1);
  P1.writeDiscrete(LOW, 6, 2);
  P1.writeDiscrete(LOW, 6, 3);
  P1.writeDiscrete(LOW, 6, 4);
  P1.writeDiscrete(LOW, 6, 5);
  P1.writeDiscrete(LOW, 6, 6);
  P1.writeDiscrete(LOW, 6, 7);
  P1.writeDiscrete(LOW, 6, 8);
  P1.writeDiscrete(LOW, 7, 1);
  P1.writeDiscrete(LOW, 7, 2);
  P1.writeDiscrete(LOW, 7, 3);
  P1.writeDiscrete(LOW, 7, 4);
  P1.writeDiscrete(LOW, 7, 5);
  P1.writeDiscrete(LOW, 7, 6);
  P1.writeDiscrete(LOW, 7, 7);
  P1.writeDiscrete(LOW, 7, 8);

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
            TC1_data = (int)(P1.readTemperature(1, 1) * 100);
            TC2_data = (int)(P1.readTemperature(1, 2) * 100);
            TC3_data = (int)(P1.readTemperature(1, 3) * 100);
            TC4_data = (int)(P1.readTemperature(1, 4) * 100);
            TC5_data = (int)(P1.readTemperature(2, 1) * 100);
            TC6_data = (int)(P1.readTemperature(2, 2) * 100);
            TC7_data = (int)(P1.readTemperature(2, 3) * 100);
            TC8_data = (int)(P1.readTemperature(2, 4) * 100);
            TC9_data = (int)(P1.readTemperature(3, 1) * 100);

            LC1 = (int)(P1.readTemperature(3, 2) * 100);
            LC2 = (int)(P1.readTemperature(3, 3) * 100);
            LC7 = (int)(P1.readTemperature(3, 4) * 100);

            PT1_counts = P1.readAnalog(4, 1);
            PT2_counts = P1.readAnalog(4, 2);
            PT3_counts = P1.readAnalog(4, 3);
            PT4_counts = P1.readAnalog(4, 4);
            PT5_counts = P1.readAnalog(4, 5);
            PT13_counts = P1.readAnalog(4, 6);
            PT14_counts = P1.readAnalog(4, 7);

            PT1_pressure = (int)(((float)PT1_counts/8191.0) * 10);
            PT2_pressure = (int)(((float)PT2_counts/8191.0) * 10);
            PT3_pressure = (int)(((float)PT3_counts/8191.0) * 10);
            PT4_pressure = (int)(((float)PT4_counts/8191.0) * 10);
            PT5_pressure = (int)(((float)PT5_counts/8191.0) * 10);
            PT13_pressure = (int)(((float)PT13_counts/8191.0) * 10);
            PT14_pressure = (int)(((float)PT14_counts/8191.0) * 10);

            TcData[0] = TC1_data;
            TcData[1] = TC2_data;
            TcData[2] = TC3_data;
            TcData[3] = TC4_data;
            TcData[4] = TC5_data;
            TcData[5] = TC6_data;
            TcData[6] = TC7_data;
            TcData[7] = TC8_data;
            TcData[8] = TC9_data;

            LcData[0] = LC1;
            LcData[1] = LC2;
            LcData[2] = LC7;

            PtData[0] = PT1_pressure;
            PtData[1] = PT2_pressure;
            PtData[2] = PT3_pressure;
            PtData[3] = PT4_pressure;
            PtData[4] = PT5_pressure;
            PtData[5] = PT13_pressure;
            PtData[6] = PT14_pressure;

            valveData[0] = PBV1State;
            valveData[1] = PBV2State;
            valveData[2] = PBV3State;
            valveData[3] = PBV4State;
            valveData[4] = PBV5State;
            valveData[5] = PBV6State;
            valveData[6] = PBV7State;
            valveData[7] = PBV8State;
            valveData[8] = PBV9State;
            valveData[9] = PBV10State;
            valveData[10] = PBV11State;
            valveData[11] = SOL1State;
            valveData[12] = SOL2State;
            valveData[13] = SOL3State;
            valveData[14] = SOL4State;
            valveData[15] = SOL5State;
            valveData[16] = SOL6State;
            valveData[17] = SOL7State;
            valveData[18] = SOL8State;
            valveData[19] = SOL9State;
            valveData[20] = HeaterState;
            valveData[21] = PMP3State;
            valveData[22] = IGN1State;
            valveData[23] = IGN2State;

            client.write((uint8_t*)TcData, sizeof(TcData));
            client.write((uint8_t*)LcData, sizeof(LcData));
            client.write((uint8_t*)PtData, sizeof(PtData));
            client.write((uint8_t*)valveData, sizeof(valveData));
            break;

          case 10: // PBV1
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 1);
              PBV1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 1);
              PBV1State = 1;
            }
            break;

          case 11: // PBV2
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 2);
              PBV2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 2);
              PBV2State = 1;
            }
            break;

          case 12: // PBV3
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 3);
              PBV3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 3);
              PBV3State = 1;
            }
            break;

          case 13: // PBV4
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 4);
              PBV4State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 4);
              PBV4State = 1;
            }
            break;

          case 14: // PBV5
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 5);
              PBV5State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 5);
              PBV5State = 1;
            }
            break;

          case 15: // PBV6
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 6);
              PBV6State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 6);
              PBV6State = 1;
            }
            break;

          case 16: // PBV7
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 7);
              PBV7State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 7);
              PBV7State = 1;
            }
            break;

          case 17: // PBV8
            if (state == 0) {
              P1.writeDiscrete(LOW, 5, 8);
              PBV8State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 5, 8);
              PBV8State = 1;
            }
            break;

          case 18: // PBV9
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 1);
              PBV9State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 1);
              PBV9State = 1;
            }
            break;

          case 19: // PBV10
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 2);
              PBV10State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 2);
              PBV10State = 1;
            }
            break;

          case 20: // PBV11
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 3);
              PBV11State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 3);
              PBV11State = 1;
            }
            break;

          case 21: // SOL1
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 4);
              SOL1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 4);
              SOL1State = 1;
            }
            break;

          case 22: // SOL2
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 5);
              SOL2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 5);
              SOL2State = 1;
            }
            break;

          case 23: // SOL3
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 6);
              SOL3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 6);
              SOL3State = 1;
            }
            break;
          
          case 24: // SOL4
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 7);
              SOL4State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 7);
              SOL4State = 1;
            }
            break;

          case 25: // SOL5
            if (state == 0) {
              P1.writeDiscrete(LOW, 6, 8);
              SOL5State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 6, 8);
              SOL5State = 1;
            }
            break;

          case 26: // SOL6
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 1);
              SOL6State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 1);
              SOL6State = 1;
            }
            break;

          case 27: // SOL7
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 2);
              SOL7State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 2);
              SOL7State = 1;
            }
            break;

          case 28: // SOL8
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 3);
              SOL8State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 3);
              SOL8State = 1;
            }
            break;  

          case 29: // SOL9
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 4);
              SOL9State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 4);
              SOL9State = 1;
            }
            break;

          case 30: // Heater
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 5);
              HeaterState = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 5);
              HeaterState = 1;
            }
            break;

          case 31: // PMP3
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 6);
              PMP3State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 6);
              PMP3State = 1;
            }
            break;

          case 32: // IGN1
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 7);
              IGN1State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 7);
              IGN1State = 1;
            }
            break;

          case 33: // IGN2
            if (state == 0) {
              P1.writeDiscrete(LOW, 7, 8);
              IGN2State = 0;
            } else if (state == 1) {
              P1.writeDiscrete(HIGH, 7, 8);
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