#include <SPI.h>      // Header file for SPI devices communication functions
#include <stdio.h>    // Header file for standard I/O operations
#include <Vbias.h>    // Header file for voltage bias thermistor temperature functions
#include <Arduino.h>  // Header file for Arduino functions
#include <list.h>

const unsigned short int CS1 = 1;
const unsigned short int CS2 = 2;
const unsigned short int CS3 = 3;
const unsigned short int CS4 = 4;
const unsigned short int CS5 = 5;
const unsigned short int CS6 = 6;
const unsigned short int CS7 = 7;
const unsigned short int CS8 = 8;
const unsigned short int S0 = 11;
const unsigned short int S1 = 12;
const unsigned short int S2 = 13;
SPISettings MCP3002(3E6, MSBFIRST, SPI_MODE0);

void setup(){
  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(CS3, OUTPUT);
  pinMode(CS4, OUTPUT);
  pinMode(CS5, OUTPUT);
  pinMode(CS6, OUTPUT);
  pinMode(CS7, OUTPUT);
  pinMode(CS8, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  pinMode(SCK, OUTPUT);
  pinMode(MISO, INPUT);

  digitalWrite(SS, LOW);
  digitalWrite(SS, HIGH);

  SPI.begin();
  Serial.begin(115200);
}

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

void loop(){
  Serial.print("CH0 = ");
  float data = readADC(0x40) * 5.0/1024.0;  // 0x68 for single-ended and 0x40 for differential
  Serial.print(data, 3);
  Serial.print(" V");
  Serial.println();
  delayMicroseconds(100);
}
