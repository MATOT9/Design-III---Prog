#include <SPI.h>      // Header file for SPI devices communication functions
#include <stdio.h>    // Header file for standard I/O operations

SPISettings MCP3002(3E6, MSBFIRST, SPI_MODE0);

unsigned short int readADC(byte address){
  byte MSB = 0;
  byte LSB = 0;
  byte JUNK = 0;

  SPI.beginTransaction(MCP3002);
  digitalWrite(SS, LOW);
  MSB = SPI.transfer(address) & 0x03;  // Send readAddress and receive MSB data, masked to two bits
  LSB = SPI.transfer(JUNK);            // Push junk data and get LSB byte return
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
  Serial.print("CH0 = ");
  float data = readADC(0x40) * 5.0/1024.0;  // 0x68 for single-ended and 0x40 for differential
  Serial.print(data, 3);
  Serial.print(" V");
  Serial.println();
  delayMicroseconds(100);
}
