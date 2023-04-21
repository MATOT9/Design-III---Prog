#include <math.h>
#include <SPI.h>      // Library for SPI devices communication functions
#include <stdio.h>    // Library for standard I/O operations
#include "C:\Users\Maxime\Desktop\Vbias.h"  // Local library

// Define pins
const byte S0 = 11;
const byte S1 = 12;
const byte S2 = 13;
const byte CSList[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };


// Define ADC communication settings
SPISettings MCP33141(60E6, MSBFIRST, SPI_MODE0);

float matrix[9][9] = { };
float refs[2][2] = { };
char MUXs[8][8] PROGMEM = {
	{ 24, 23, 22, 35, 25, 14, 15, 16 },
	{ 44, 42, 43, 41, 45, 32, 33, 34 },
	{ 61, 62, 63, 65, 53, 52, 54, 51 },
	{ 82, -21, 83, 84, 65, 73, 72, 74 },
	{ 16, 17, 18, 46, 28, 27, -11, 26 },
	{ 58, 56, 59, 55, 57, 49, 48, 47 },
	{ 75, 78, 77, 76, 67, 68, 66, 69 },
	{ 96, 94, 93, -22, 85, 87, 86, 88 },
};


void setup(){
  SPI.begin();
  Serial.begin(2E6);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  for (byte CS : CSList) {
    pinMode(CS, OUTPUT);
    digitalWrite(CS, LOW);
    digitalWrite(CS, HIGH);
  }
}

void loop(){
  for (ushort i = 0; i < 8; i++) {
		for (ushort j = 0; j < 8; j++) {
			short val = MUXs[i][j];
			ushort ROW = extractDigit(val, 10) - 1;
			ushort COL = extractDigit(val, 1) - 1;
			// float randomf = static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / 50));
			if (val < 0) {
				// refs[ROW][COL] = randomf;
        refs[ROW][COL] = roundDec(readADC(), 1E4);
			}
			else {
				matrix[ROW][COL] = roundDec(readADC(), 1E4);
			}
		}
	}

}

float readADC() {
  // Define MUXs selection bytes
  byte bS0 = 0;
  byte bS1 = 0;
  byte bS2 = 0;

  for (byte x : CSList) {
    for (byte i = 0; i < 8; i++) {
      bS0 = i & 0x01;			      // Get value of first bit
      bS1 = (i >> 1) & 0x01;	  // Get value of second bit
      bS2 = (i >> 2) & 0x01;	  // Get value of third bit

      // Send selection bits value to MUX
      digitalWrite(S0, bS0);
      digitalWrite(S1, bS1);
      digitalWrite(S2, bS2);

      float data = fetchVal(x);
      data = roundDec(data, 1E4);

      Serial.print(data, 4);
      Serial.print(" V");
      Serial.println();
    }

    delayMicroseconds(5);
  }
}

float fetchVal(byte CS) {
  static byte MSB = 0;
  static byte LSB = 0;
  float arr[3];

  // Reads 3 times and return the average
  for (int x = 0; x < 3; x++) {
    delayMicroseconds(5);
    SPI.beginTransaction(MCP33141);
    digitalWrite(CS, LOW);
    MSB = SPI.transfer(0) & 0x03;        // Send readAddress and receive MSB data, masked to two bits
    LSB = SPI.transfer(0);               // Push junk data and get LSB byte return
    digitalWrite(CS, HIGH);
    SPI.endTransaction();
    arr[x] = (MSB << 8 | LSB) * 3.3/4096.0; // Assemble the bytes and compute the voltage
  }

  return average(arr);
}