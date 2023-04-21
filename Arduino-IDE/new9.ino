const int ROWS = 9;
const int COLS = 9;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      float val = random(500) / 100.0; // generate a random number between 0 and 5
      float err = random(100) / 100.0; // generate a random error between 0 and 1
      float gauss = val + err; // add error to value
      Serial.print(gauss, 2); // print value with 2 decimal places
      if (j < COLS - 1) {
        Serial.print("\t"); // use tab as separator between columns
      }
      delay(10);
    }
    Serial.print("\t"); // use newline character to separate rows
  }
  Serial.println("0.5\t0.5\t0.5"); // print the 3 extra values after every matrix
  delay(1000); // wait 1 second before generating the next matrix
}
