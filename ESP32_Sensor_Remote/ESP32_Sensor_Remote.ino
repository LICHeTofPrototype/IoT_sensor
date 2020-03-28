
#include <HTTPClient.h>
#include <WiFi.h>

#define SensorPin 34
#define OnOffPin 25
#define LEDPin 26
#define WiFiPin 13

#define DeviceID DEV0092407
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

volatile int Signal;
int S = 1800;

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

  url_start = url + "/mesurement/start/";
  url_end = url + "/mesurement/end/";
  url += "/calc_pnn/";
  
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
    
    WiFiConnect();
    
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);


    //*************************************
    //測定前にDeviceIDをPOSTする
    HttpConnectStart();
    int postCode = client.POST(DeviceID);

    if( postCode == 200 ){
      Serial.print("[---] Success on sending POST: ");
      Serial.println(postCode); 
      HttpDisConnect();
    }else{
      Serial.print("[***] Error on sending POST: ");
      Serial.println(postCode);
      HttpDisConnect();
      return ; 
    }
    //*************************************


    while(1){
      int SensorPower = digitalRead(OnOffPin);
      if (SensorPower == LOW) {        
        HttpDisConnect();
        break;
      }
      HttpConnect();
      CreateJson();
    }
   
  }else if (SensorPower == LOW){
    digitalWrite(LEDPin, LOW);
    
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);  
    sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    
    delay(2000);
  }

}
