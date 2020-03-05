*************************************************
概要
*************************************************
ESP32 + PulseSensor により脈拍をアナログデータをデジタルデータとして読み込み
サーバ側にデータをPOSTするためのスクリプトだ！

*************************************************
ツール
*************************************************
・ESPr Developer 32
https://www.switch-science.com/catalog/3210/

・PulseSensor
https://www.switch-science.com/catalog/1135/

・æ IDE (1.8.10)
https://www.arduino.cc/en/main/software

・Library to install
*ArduinoJson ( by Benoit Blanchon) 6.14

*************************************************
内容
*************************************************
*現在はテスト用に作成しているため随時変更する

メイン関数は setup() ( ESP32_Sensor.ino )
シリアル通信を確認後, 
WiFiとの接続 WiFiConnect() ( WiFiConnect.ino ), 
サーバとの接続 HttpConnet() (HttpConnect.ino ), 
センサデータをJson形式で生成しサーバへPOST CreateJson() ( CreateJson.ino ),
測定終了後, 各接続を解除.

(*本番では, loop() をメイン関数として実装する予定)


*************************************************
使い方
*************************************************

(1)ESP32の, 3.3V, GND, 25pinにセンサピンを挿して, シリアル通信を行うためにPCと接続.

(2)ESP32とマイコン間で通信をするためにBoardManagerから「esp32 by Espressif Systems 1.0.4」をインストール

(3)ボートを「ESP32 Dev Module」シリアルポートを「***usb**」に設定

(4)WiFi環境・サーバに合わせて以下を変更
・SSID
・Password
・IPアドレス
・PORT番号

(5)１件で送りたいデータ数をCreateJson.inoで変更
・timeInterval (測定間隔)
・arrayNum (配列に入れる要素数)
・JSON_ARRAY_SIZE (配列の大きさ)
     (変数で置きたいけど、なぜかできないから手動で）
     
(6)シリアルモニタを開き、コンパイルし実行




