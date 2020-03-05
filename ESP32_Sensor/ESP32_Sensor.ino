
#include <HTTPClient.h>
#include <WiFi.h>

#define SensorPin 34
#define ButtonPin 35
//#define LEDPin 26
//#define SensorPower 25

volatile int SampleCount = 0;
volatile int count = 0;
char buffer[3000];

HTTPClient client;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
//const String host ="8e522fd7.ngrok.io";
const String host ="192.168.2.105";
#define PORT 8000
//*********************************************


void setup() {  
  Serial.begin(115200); 
  while (!Serial);
  
  pinMode(SensorPin, INPUT);
  pinMode(ButtonPin, INPUT);
  //pinMode(LEDPin, OUTPUT);
  //pinMode(SensorPower, OUTPUT);
  Serial.print("\n");

  HttpDisConnect();
  WiFiDisConnect();
}
 
void loop() {
  
  int ButtonStatus = digitalRead(ButtonPin);
  SampleCount = 0;
  
  if (ButtonStatus == HIGH) {
    //digitalWrite(LEDPin, HIGH);
    //digitalWrite(SensorPower, HIGH);
    Serial.println("[ON ] Start Mesurement");
    //Serial.println("Calibrating . . .");
    //delay(30000);

    WiFiConnect();
    HttpConnect();
    
    while(1){
      int ButtonStatus = digitalRead(ButtonPin);
      if (ButtonStatus == LOW) {
        HttpDisConnect();
        WiFiDisConnect();
        break;
      }
      CreateJson();
      delay(1000);
    }
   
  }else if (ButtonStatus == LOW){
    //digitalWrite(LEDPin, LOW);
    //digitalWrite(SensorPower, LOW);
    Serial.println("[OFF] Stop Mesurement");
    delay(5000);
  }
  
}
