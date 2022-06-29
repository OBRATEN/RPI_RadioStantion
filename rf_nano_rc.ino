#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9); // CE, CSN

#define PloadSize  32  // 32 unsigned chars TX payload
byte newdata;
unsigned char rx_buf[PloadSize] = {0};

struct txData {
  uint8_t prefix = 0;
  int16_t aX = 0;
  int16_t aY = 0;
  int16_t aZ = 0;
  uint16_t DStemp = 0;
  uint16_t BMPtemp = 0;
  uint32_t press = 0;
  uint16_t alt = 0;
  uint32_t packageID = 0;
  uint8_t voltage = 0;
  uint32_t gTime = 0;
  uint8_t status = 0;
} tx_data;

struct gpsData {
  uint8_t prefix = 1;
  char lat[12];
  char lon[12];
  char alt[6];
} gps_data;

void setup() {
  radio.begin();
  Serial.begin(115200);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(0x5C);
  radio.openReadingPipe(0, 0x7878787878LL);
  radio.startListening();
  delay(10);
}

void loop() {
  if (radio.available()) {
    radio.read(rx_buf, 32);
    if (rx_buf[0] == 0) {
      memcpy(&tx_data, rx_buf, sizeof(tx_data));
      Serial.print("Sensors:");
      Serial.print(((double)tx_data.aX) / 100);
      Serial.print('|');
      Serial.print(((double)tx_data.aY) / 100);
      Serial.print('|');
      Serial.print(((double)tx_data.aZ) / 100);
      Serial.print('|');
      Serial.print(((double)tx_data.DStemp) / 100);
      Serial.print('|');
      Serial.print(((double)tx_data.BMPtemp) / 100);
      Serial.print('|');
      Serial.print((tx_data.press));
      Serial.print('|');
      Serial.print(((double)tx_data.alt) / 100);
      Serial.print('|');
      Serial.print(tx_data.voltage);
      Serial.print('|');
      Serial.print(tx_data.gTime);
      Serial.print('|');
      Serial.print(tx_data.packageID);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 7) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 6) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 5) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 4) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 3) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 2) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 1) ? 1 : 0);
      Serial.print('|');
      Serial.print(tx_data.status & (1 << 0) ? 1 : 0);
      Serial.print(";\n");
    } else {
      memcpy(&gps_data, rx_buf, sizeof(gps_data));
      Serial.print("GPS:");
      for (uint8_t i = 0; i < 12; i++) Serial.print(gps_data.lat[i]);
      Serial.print('|');
      for (uint8_t i = 0; i < 12; i++) Serial.print(gps_data.lon[i]);
      Serial.print('|');
      for (uint8_t i = 0; i < 6; i++) Serial.print(gps_data.alt[i]);
      Serial.print(";\n");
    }
    
    /*
    for (int i = 0; i < 32; i++)
    Serial.print(rx_buf[i]);
    Serial.print('\n');
    */
  }
}
