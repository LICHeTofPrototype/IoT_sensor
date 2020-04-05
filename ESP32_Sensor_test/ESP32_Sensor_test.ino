
#include "freertos/task.h"
#include <WiFi.h>
#include <HTTPClient.h>
HTTPClient client;

#define sensor_pin 34
#define onoff_pin 25
#define led_pin 26
#define wifi_pin 13

#define JST 3600* 9

volatile int sample_count = 0;
volatile int count = 0;
volatile int calibration = 0;

time_t now_time;
struct tm *timeinfo;  //時刻を格納するオブジェクト
char current_time[10];
char current_date[30];

boolean is_measurement_start = false;
boolean is_measurement_end = false;
int measurement_ID;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.119";
String url = ""; //calc_dataのurl
String url_start = ""; // measurement_startのurl
String url_end = ""; // measurement_endのurl
#define port 8000
int device_ID = 92407;
//*********************************************

int sensor_power; // センサのデータ出力があるかないかを判断するフラグ

void setup() {
  Serial.begin(115200); 
  while (!Serial);
  
  pinMode(sensor_pin, INPUT);
  pinMode(onoff_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(wifi_pin, OUTPUT);

  url = "http://";
  url += host;
  url += ":";
  url += port;
  url += "/v1/api";
  
  url_start = url + "/measurement/start/";
  url_end = url + "/measurement/end/";
  url += "/calc_data/";
  WiFiDisConnect();
  WiFiConnect();
}

void loop() {
  //Serial.print("LOOP");
  sensor_power = digitalRead(onoff_pin);
  // センサーのデータ出力がある場合
  if (sensor_power == HIGH){
    if (!is_measurement_start) {
      Serial.println("[ON ] Start Calibulation");
      Serial.print("[ON ] ");
      // 6秒間のキャリブレーション時間
      while (calibration <=  6){
        if (calibration % 2 == 0){
          digitalWrite(led_pin, LOW);
        } else {
          digitalWrite(led_pin, HIGH);
        }
        delay(1000);
        Serial.print("◼");
        calibration += 1 ;
      }
      calibration = 0;
      digitalWrite(led_pin, HIGH);
      Serial.println(" ");
      Serial.println("[ON ] Success Calibulation");
      now_time = time(NULL);
      timeinfo = localtime(&now_time);
      HttpConnectStart(); // MeasurementStartAPIのコール
      StartPost();
      HttpDisConnect();
      is_measurement_start = true;
      is_measurement_end = false;
    }
  }
  sensor_power = digitalRead(onoff_pin);
  if (sensor_power == HIGH){
    //Serial.println("[ON ] Start MultiTask");
    xTaskCreatePinnedToCore(TaskSave, "tasksave", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TaskPost, "taskpost", 4096, NULL, 1, NULL, 1);
//    while(sensor_power == HIGH){
//      if(sensor_power == LOW){
//        break;
//      }
//    }
  }
  
  // センサーのデータ出力がない場合はディープスリープモードで省電力化の実装
  else if (sensor_power == LOW){
    digitalWrite(led_pin, LOW);
    now_time = time(NULL);
    timeinfo = localtime(&now_time);  
    sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Serial.print("[OFF] ");
    Serial.println(current_time);
    if (!is_measurement_end){
      HttpConnectStart(); // MeasurementStartAPIのコール
      EndPost();
      HttpDisConnect();
      is_measurement_start = false;
      is_measurement_end = true;
    }
    delay(2000);
  }
}
