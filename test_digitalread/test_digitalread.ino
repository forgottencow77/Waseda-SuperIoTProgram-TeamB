void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(9, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(9) == HIGH){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13,  LOW);
  }
  Serial.println(digitalRead(13));
}
