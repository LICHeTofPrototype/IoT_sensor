
#include <HTTPClient.h>
#include <WiFi.h>

#define SensorPin 34
#define ButtonPin 35
#define LEDPin 25

volatile int SampleCount = 0;
volatile int count = 0;
String url = "";

HTTPClient client;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.111";
#define PORT 8000
//*********************************************

void setup() {  
  Serial.begin(115200); 
  while (!Serial);
  
  pinMode(SensorPin, INPUT);
  pinMode(ButtonPin, INPUT);
  pinMode(LEDPin, OUTPUT);

  url = "http://";
  url += host;
  url += ":";
  url += PORT;
  url += "/api/calc_pnn/2/";

  Serial.print("Requesting URL = ");
  Serial.println(url);
  Serial.print("\n");

  HttpDisConnect();
  WiFiDisConnect();
}

void loop() {
  
  int ButtonStatus = digitalRead(ButtonPin);
  SampleCount = 0;
  
  if (ButtonStatus == HIGH) {
    Serial.println("[ON ] Start Mesurement");
    digitalWrite(LEDPin, HIGH);

    WiFiConnect();
    
    while(1){
      int ButtonStatus = digitalRead(ButtonPin);
      if (ButtonStatus == LOW) {
        HttpDisConnect();
        WiFiDisConnect();
        break;
      }
      HttpConnect();
      CreateJson();
      //delay(1000);   //１件ごとの時間間隔を設定 (ms)
    }
   
  }else if (ButtonStatus == LOW){
    digitalWrite(LEDPin, LOW);
    Serial.println("[OFF] Stop Mesurement");
    delay(5000);
  }
  
}
