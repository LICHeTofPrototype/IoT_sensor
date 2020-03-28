
#include <HTTPClient.h>
#include <WiFi.h>

#define SensorPin 34
#define OnOffPin 25
#define LEDPin 26
#define WiFiPin 13

#define JST 3600* 9

volatile int SampleCount = 0;
volatile int count = 0;
volatile int cal = 0;

String url = "";
String url_start = "";
String url_end = "";

time_t NowTime;
struct tm *timeInfo;  //時刻を格納するオブジェクト
char CurrentTime[10];
char CurrentDate[30];

volatile int Signal;
volatile int S =1800;

volatile int MeasurementID;
volatile int DeviceID = 92407 ;

HTTPClient client;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.119";
#define PORT 8000
//*********************************************

void setup() {  
  Serial.begin(115200); 
  while (!Serial);
  
  pinMode(SensorPin, INPUT);
  pinMode(OnOffPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(WiFiPin, OUTPUT);

  url = "http://";
  url += host;
  url += ":";
  url += PORT;
  url += "/v1/api";

  url_start = url + "/measurement/start/";
  url_end = url + "/measurement/end/";
  url += "/calc_data/";
  
  WiFiDisConnect();
  WiFiConnect();
}

void loop() {
  int SensorPower = digitalRead(OnOffPin);
  if (SensorPower == HIGH) {
    
    while (cal <= 20 ){
      if (cal % 2 == 0){
        digitalWrite(LEDPin, LOW);
      }else {
        digitalWrite(LEDPin, HIGH);
      }
      delay(1000);
      cal += 1 ;
    }
    cal = 0;
    digitalWrite(LEDPin, HIGH);
        
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);

    HttpConnectStart();
    StartPost();
    HttpDisConnect();

    while(1){
      int SensorPower = digitalRead(OnOffPin);
      
      if (SensorPower == LOW) {     
        HttpDisConnect();
        break;
      }else if (SensorPower == HIGH){
        HttpConnect();
        CreateJson();
      }
    }
   
  }else if (SensorPower == LOW){
    digitalWrite(LEDPin, LOW);
    
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);  
    sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    
    delay(2000);
  }

}