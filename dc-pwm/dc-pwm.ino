int count = 0;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  Serial.begin(9600);  // 9600bps for serial communication
  Serial.println("start");
}

void loop() {
  if (digitalRead(4) == LOW) {
    delay(100);
    count = (count % 4) + 1;  // Increment count (1 to 4) and wrap around
    Serial.print("count:");
    Serial.println(count);
    while (digitalRead(4) == LOW) {
      delay(100);
    }
    delay(100);
  }

  // Rest of your code...
  // Adjust analogWrite values based on the updated count.
  if (count == 1) {
    analogWrite(2, 0);
  } else if (count == 2) {
    analogWrite(2, 24);
  } else if (count == 3) {
    analogWrite(2, 96);
  } else if (count == 4) {
    analogWrite(2, 255);
  }
}
