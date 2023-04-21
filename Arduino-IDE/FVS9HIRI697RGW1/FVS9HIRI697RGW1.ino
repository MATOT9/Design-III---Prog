#include <math.h>

const int ROWS = 9; // number of rows in the matrix
const int COLS = 9; // number of columns in the matrix

void setup() {
  Serial.begin(2E6); // initialize serial communication at 9600 bits per second
}

void loop() {
  float matrix[ROWS][COLS]; // initialize the 2D matrix
  
  // generate random values with Gaussian distribution and add random errors
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      float value = 5.0 * exp(-(pow(i-4,4) + pow(j-4,4))/4); // Gaussian distribution
      float error = random(-10, 10) / 100.0; // random error between -0.1 and 0.1
      matrix[i][j] = max(0.0, min(5.0, value + error)); // limit the value between 0 and 5
    }
    
    // send the row to the serial monitor with tabs as separator and wait for 1 second
    for (int j = 0; j < COLS; j++) {
      Serial.print(matrix[i][j]);
      if (j < COLS-1) Serial.print("\t"); // use tab as separator
    }
    Serial.print("\t");
   }
  // Send a row of 3 values with approximately 0.5 between matrices
  for (int i = 0; i < 3; i++) {
    Serial.print(0.5);
    if (i < 2) {
      Serial.print('\t');
    }
  }
  Serial.print('\n');
  delay(1000); // wait for 1 second before sending the next matrix
}
