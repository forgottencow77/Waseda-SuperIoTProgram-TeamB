void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int a = 100;
  int b = 200;
  Serial.println(a);
  Serial.println(b);
  a = a + b;
  Serial.println(a);
  a = a + b;
  Serial.println(a);
}

void loop() {
  // put your main code here, to run repeatedly:

}
