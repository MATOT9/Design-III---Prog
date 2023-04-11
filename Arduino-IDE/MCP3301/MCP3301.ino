#include <SPI.h>      // Header file for SPI devices communication functions

SPISettings MCP3002(3000000, MSBFIRST, SPI_MODE0);

unsigned short int readADC(){
  byte MSB = 0;
  byte LSB = 0;
  byte JUNK = 0;

  SPI.beginTransaction(MCP3002);
  digitalWrite(SS, LOW);
  MSB = SPI.transfer(JUNK) & 0x0F;  // Send readAddress and receive MSB data, masked to 4 bits
  LSB = SPI.transfer(JUNK);         // Push junk data and get LSB byte return
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
  return MSB << 8 | LSB;
}

void setup(){
  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MISO, INPUT);

  digitalWrite(SS, LOW);
  digitalWrite(SS, HIGH);

  SPI.begin();
  Serial.begin(115200);
}

void loop(){
  float data = readADC() * 3.3/8192.0;
  Serial.print(data, 4);
  Serial.print(" V");
  Serial.println();
  delayMicroseconds(100);
}
