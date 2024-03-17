 int a;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   a = 5;
  Serial.println("Ready");
  delay(3000);
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Go");
  delay(1000);
}
