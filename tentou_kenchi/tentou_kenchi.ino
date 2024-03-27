void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);
}

//圧力計測変数
float A = 880.79;
float B = 47.96;
int aIn0, aIn1; // アナログ入力 A/D値 for A0 and A1
float aInV0, aInV1; // アナログ入力電圧[V] for A0 and A1
int RM_kohm = 10.0; // 既知抵抗[kΩ]
float fsrR_kohm0, fsrR_kohm1; // FSR抵抗値[kΩ] for A0 and A1
float gram0, gram1; // 推定質量 [gram] for A0 and A1

//温度計測変数
int tempIn;
float thresholdTemp = 25.0;

void loop() {
  //温度の計算
  tempIn = analogRead(2);
  float tempC = tempIn * (5.0 / 1023.0);
  float temp = 100 * tempC - 60;

  if (temp > thresholdTemp) {
    // 質量の計算 for A0
    aIn0 = analogRead(0); // アナログ入力(A/D変換値)
    aInV0 = aIn0 * 5.0 / 1023; // A/D変換値から FSRにかかる電圧[V]を計算
    fsrR_kohm0 = RM_kohm * aInV0 / (5.0 - aInV0); // FSRの抵抗値[kΩ]
    gram0 = A / fsrR_kohm0 + B; // 質量を計算gram

    // 質量の計算 for A1
    aIn1 = analogRead(1); // アナログ入力(A/D変換値)
    aInV1 = aIn1 * 5.0 / 1023; // A/D変換値から FSRにかかる電圧[V]を計算
    fsrR_kohm1 = RM_kohm * aInV1 / (5.0 - aInV1); // FSRの抵抗値[kΩ]
    gram1 = A / fsrR_kohm1 + B; // 質量を計算gram

    // 状態の判断
    if (gram0 == 47.96 && gram1 == 47.96) {
      Serial.println("転んでいる");
      tone(11,440);
    } else if (gram0 >= 200 && gram1 >= 150) {
      Serial.println("立っている");
      tone(11,440);
      delay(1500)
    } else if (gram0 >= 50 && gram1 >= 50) {
      Serial.println("座っている");
      tone(11,440);
      delay(1500)
    }

    // 質量&温度観測log 
    Serial.print("Sensor ①: ");
    Serial.print(aInV0); // 力センサにかかる電圧の表示
    Serial.print("V"); // 単位の表示
    Serial.print("\t"); // タブ区切り
    //Serial.print(fsrR_kohm0); // FSR抵抗値の表示
    //Serial.print(" kohm"); // 単位の表示
    //Serial.print("\t"); // タブ区切り
    Serial.print(gram0); // 質量の表示
    Serial.println(" g"); // 単位の表示と改行

    Serial.print("Sensor ②: ");
    Serial.print(aInV1); // 力センサにかかる電圧の表示
    Serial.print("V"); // 単位の表示
    Serial.print("\t"); // タブ区切り
    //Serial.print(fsrR_kohm1); // FSR抵抗値の表示
    //Serial.print(" kohm"); // 単位の表示
    //Serial.print("\t"); // タブ区切り
    Serial.print(gram1); // 質量の表示
    Serial.println(" g"); // 単位の表示と改行

    Serial.println(temp); //温度
  }

  // インターバル
  delay (1000);
}