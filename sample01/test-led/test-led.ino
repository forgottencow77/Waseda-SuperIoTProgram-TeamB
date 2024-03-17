int led_pin = 13;
int time = 1000;

void setup() {
  pinMode(led_pin, OUTPUT);
}

void loop() {
  digitalWrite(led_pin, HIGH);
  delay(time);
  digitalWrite(led_pin, LOW);
  delay(time);
  Serial.println(time);
  time -= 100;

  if (time == 0) {
    time = 1000;
  }
}
