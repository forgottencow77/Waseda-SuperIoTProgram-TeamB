void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

int time = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(time % 3 == 0){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  }
  else{
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  }
  delay(1000);
  time += 1;
  Serial.println(time);
}
