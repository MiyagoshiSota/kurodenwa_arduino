int relayPin = 8; // リレーのINピンをD8に接続
String line;      // 受信文字列
int line_len;     // 受信文字列の長さ
long num = -1;    // 受信整数

void setup()
{
    Serial.begin(9600);
    pinMode(relayPin, OUTPUT);   // リレーのピンを出力モードに設定
    digitalWrite(relayPin, LOW); // リレーをOFFにしてモーターを止める
}

void loop()
{
    if (Serial.available() > 0)
    {
        // シリアル通信で1行（改行コードまで）読み込む
        line = Serial.readStringUntil('\n');
        // 文字列の長さを取得する
        line_len = line.length();
        // 文字列の長さが1文字以上の場合
        if (line_len > 0)
        {
            // 文字列を整数に変換する
            num = line.toInt();
        }
    }

    Serial.println(num);

    if (num == 1)
    {
        digitalWrite(relayPin, HIGH); // リレーをONにしてモーターを動かす
    }
    else if (num == 0)
    {
        digitalWrite(relayPin, LOW); // リレーをOFFにしてモーターを止める
    }
}
