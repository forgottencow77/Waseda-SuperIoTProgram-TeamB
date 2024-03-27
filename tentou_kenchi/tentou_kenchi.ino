void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  Serial.begin(9600);
}


//圧力計測変数
  float A = 880.79;
  float B = 47.96;
  int aIn; // アナログ入力 A/D値
  float aInV; // アナログ入力電圧[V]
  int RM_kohm = 10.0; // 既知抵抗[kΩ]
  float fsrR_kohm; // FSR抵抗値[kΩ]
  float gram; // 推定質量 [gram]

//温度計測変数
  int tempIn;
  float thresholdTemp = 30.0;


void loop() {
  //温度の計算
  tempIn = analogRead(2);
  float tempC = tempIn * (5.0 / 1023.0) * 100.0;

  // 質量の計算
  aIn = analogRead(0); // アナログ入力(A/D変換値)
  aInV = aIn * 5.0 / 1023; // A/D変換値から FSRにかかる電圧[V]を計算
  fsrR_kohm = RM_kohm * aInV / (5.0 - aInV); // FSRの抵抗値[kΩ]
  gram = A / fsrR_kohm + B; // 質量を計算[gram](実験値)
 
  // 質量&温度観測log 
  Serial.print(aInV); // 力センサにかかる電圧の表示
  Serial.print("V"); // 単位の表示
  Serial.print("\t"); // タブ区切り
  Serial.print(fsrR_kohm); // FSR抵抗値の表示
  Serial.print(" kohm"); // 単位の表示
  Serial.print("\t"); // タブ区切り
  Serial.print(gram); // 質量の表示
  Serial.println(" g"); // 単位の表示と改行
  Serial.println(tempC); //温度

  if(ここに続き

  // インターバル
 delay (1000);

}
