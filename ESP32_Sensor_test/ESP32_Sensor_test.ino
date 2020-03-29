
#include "freertos/task.h"
#include <WiFi.h>
#include <HTTPClient.h>
HTTPClient client;

#define SensorPin 34
#define OnOffPin 25
#define LEDPin 26
#define WiFiPin 13

#define JST 3600* 9

volatile int SampleCount = 0;
volatile int count = 0;
volatile int calibration = 0;

String url = "";
String url_start = "";
String url_end = "";

time_t NowTime;
struct tm *timeInfo;  //時刻を格納するオブジェクト
char CurrentTime[10];
char CurrentDate[30];

volatile int MeasurementID;
volatile int Signal;
volatile int S =1800;


//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.119";
#define PORT 8000
int DeviceID = 92407;
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

    Serial.println("[ON ] Start Calibulation");
    Serial.print("[ON ] ");
    while (calibration <=  6){
      if (calibration % 2 == 0){
        digitalWrite(LEDPin, LOW);
      }else {
        digitalWrite(LEDPin, HIGH);
      }
      delay(1000);
      Serial.print("◼");
      calibration += 1 ;
    }
    calibration = 0;
    digitalWrite(LEDPin, HIGH);
    Serial.println(" ");
    Serial.println("[ON ] Success Calibulation");
    
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);

    HttpConnectStart();
    StartPost();
    HttpDisConnect();
    HttpConnect();

    int SensorPower = digitalRead(OnOffPin);
    if (SensorPower == HIGH){
      Serial.println("[ON ] Start MultiTask");
      xTaskCreatePinnedToCore(TaskSave, "tasksave", 4096, NULL, 1, NULL, 0);
      xTaskCreatePinnedToCore(TaskPost, "taskpost", 4096, NULL, 1, NULL, 1);

      while(SensorPower == HIGH){
        
        if(SensorPower == LOW){
          break;
        }
      }
    }
       
  }else if (SensorPower == LOW){
    digitalWrite(LEDPin, LOW);
    
    NowTime = time(NULL);
    timeInfo = localtime(&NowTime);  
    sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    Serial.print("[OFF] ");
    Serial.println(CurrentTime);
    
    delay(2000);
  }

}
