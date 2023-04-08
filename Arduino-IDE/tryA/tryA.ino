#include <SPI.h>

SPISettings MEGA(16000000, MSBFIRST, SPI_MODE1);

void setup() {
  pinMode(SS, OUTPUT); // set slave select pin as output
  SPI.begin(); // initialize SPI
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  Serial.begin(9600); // initialize serial communication
}

void loop() {
  digitalWrite(SS, LOW); // set SS low to select the device
  byte data = SPI.transfer(50); // send command to read data from pin 10
  digitalWrite(SS, HIGH); // set SS high to deselect the device
  Serial.println(data); // print the data to the serial monitor
  delay(1000); // wait for a second before reading again
}
