#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

bool isInit = true;
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int count = 0;
int old_d5_state = 0;
int old_d8_state = 0;
int old_pls = 0;
unsigned long waiteTime = 0;

void setup()
{
    Serial.begin(9600);
    mySoftwareSerial.begin(9600);

    if (!myDFPlayer.begin(mySoftwareSerial))
    {
        Serial.println("DFPlayerMiniERR");
        isInit = false;
    }

    pinMode(6, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
}

void loop()
{
    if (isInit)
    {
        if (Serial.available() > 0)
        {
            String input = ""; // 入力を保存するための文字列バッファ

            // 受信した全ての文字をバッファに追加する
            while (Serial.available() > 0)
            {
                char ch = Serial.read();
                if (isDigit(ch))
                { // 受け取った文字が数字であるか確認
                    input += ch;
                }
                delay(10); // 少し待ってから次の文字を読み取る（データの受信が完了するのを待つため）
            }

            // バッファが空でない場合に処理
            if (input.length() > 0)
            {
                int num = input.toInt(); // 文字列を整数に変換
                myDFPlayer.volume(20);   // ボリュームをセット

                if (num != 999)
                {
                    myDFPlayer.playMp3Folder(num); // 音声を再生
                    if (num != -38)
                    {
                        delay(10);
                    }
                }
            }
        }
    }

    int d6 = digitalRead(6);
    int d5 = digitalRead(5);
    int d8 = digitalRead(8);

    if (d8 == LOW && d5 == HIGH && old_d5_state == LOW)
    {
        count++;
        waiteTime = millis();
    }

    if (old_d8_state == HIGH && d8 == LOW)
    {
        count = 0;
    }

    if ((millis() - waiteTime) > 300)
    {
        count = 0;
    }

    if (old_pls > 0 && count == 0)
    {
        Serial.print(d6);
        Serial.print(",");
        Serial.println(old_pls);
    }
    else
    {
        Serial.print(d6);
        Serial.print(",");
        Serial.println(0);
    }

    old_d5_state = d5;
    old_d8_state = d8;
    old_pls = count;
}