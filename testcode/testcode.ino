void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(11, OUTPUT); // ピン11を出力モードに設定
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

bool isExecuted = false; // フラグの初期化
unsigned long fallenTime = 0; // 転んでいる時間を追跡するための変数

void loop() {
  //温度の計算
  tempIn = analogRead(2);
  float tempC = tempIn * (5.0 / 1023.0);
  float temp = 100 * tempC - 60;

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

  // 質量&温度観測log 
  Serial.print("Sensor ①: ");
  Serial.print(aInV0); // 力センサにかかる電圧の表示
  Serial.print("V"); // 単位の表示
  Serial.print("\t"); // タブ区切り
  Serial.print(gram0); // 質量の表示
  Serial.println(" g"); // 単位の表示と改行

  Serial.print("Sensor ②: ");
  Serial.print(aInV1); // 力センサにかかる電圧の表示
  Serial.print("V"); // 単位の表示
  Serial.print("\t"); // タブ区切り
  Serial.print(gram1); // 質量の表示
  Serial.println(" g"); // 単位の表示と改行

  Serial.println(temp); //温度

  if (temp > thresholdTemp) {

    // 状態の判断
    if (gram0 == 47.96 && gram1 == 47.96) {
      if (fallenTime == 0) { // 初めて転んだ状態を検出した場合
        fallenTime = millis(); // 現在の時間を記録
      } else if (millis() - fallenTime >= 10000) { // 転んでいる状態が10秒以上続いた場合
        Serial.println("転んでいるにょーん");
        isExecuted = false;
        tone(11, 440);
        delay(500);
        noTone(11);
        delay(500);
      }
    } else {
      fallenTime = 0; // 転んでいない状態なので、fallenTimeをリセット
      if (gram0 >= 200 && gram1 >= 150) {
        Serial.println("立っているかも");
        if(!isExecuted){
          tone(11,  349);
          delay(200);
          noTone(11);
        }
      } else if (gram0 >= 50 && gram1 >= 50) {
        Serial.println("座っているぞ");
        if(!isExecuted){
          tone(11, 349); // ファの音
          delay(200);
          noTone(11);
        }
      } else {
        Serial.println("あれ、大丈夫そ？どういう状態なん?");
      }
    }

  }

  // インターバル
  delay (3000);
}
