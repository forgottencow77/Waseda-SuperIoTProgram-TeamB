void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value =  analogRead(A0);
  float v = (float)value / 1023 * 5;
  float temp =  100 *  v - 60 ;
  Serial.println(temp);
  delay(1000);
}
