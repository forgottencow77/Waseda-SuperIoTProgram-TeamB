void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val_x = analogRead(0);
  int val_y = analogRead(1);
  int val_z = analogRead(2);

  float V_x = (float)val_x / 1023 * 5;
  float V_y = (float)val_y / 1023 * 5;
  float V_z = (float)val_z / 1023 * 5;

  float a_x = (V_x - 1.65) / 0.66;
  float a_y = (V_y - 1.65) / 0.66;
  float a_z = (V_z - 1.65) / 0.66;

  Serial.print(millis());
  Serial.print(",");

  Serial.print(a_x);
  Serial.print(",");
  Serial.print(a_y);
  Serial.print(",");
  Serial.print(a_z);
  delay(1000);
}
