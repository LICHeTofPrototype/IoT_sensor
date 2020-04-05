
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

String url = "";
String url_start = "";
String url_end = "";

time_t now_time;
struct tm *timeinfo;  //時刻を格納するオブジェクト
char current_time[10];
char current_date[30];

boolean is_measurement_stared = false;
volatile int measurement_ID;
volatile int signal;
volatile int signal_pre =1800;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.119";
#define port 8000
int device_ID = 92407;
//*********************************************
volatile int sensor_power; // センサのデータ出力があるかないかを判断するフラグ
const int array_num = 2000;  //配列に入れる要素数
const int str_num = 40 + array_num*4 + array_num - 1 +  3000;
char buffer[str_num];

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
  //SaveBeatData
  xTaskCreatePinnedToCore(TaskSave, "tasksave", 4096, NULL, 1, NULL, 0);
}

void loop() {
  sensor_power = digitalRead(onoff_pin);
  // センサーのデータ出力がある場合
  if (sensor_power == HIGH) {
    if(!is_measurement_started){
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
      is_measurement_started = true;
    }
    delay(10000); // 測定時間＝10秒
    sensor_power = digitalRead(onoff_pin);
    while(sensor_power == HIGH){
      Serial.println("[ON ] Start Post Data");
      DynamicJsonDocument root(50000);
      JsonArray Beat = root.createNestedArray("beat");
      Serial.println("[ON ] Start PostBeat");
      now_time = time(NULL);
      timeinfo = localtime(&now_time);
      sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      root["time"] = current_time;
      root["dev_id"] = device_ID;
      root["measurement_id"] = measurement_ID;
      p1 = beat_array;
      //beat_countが前回のpostの時と比較して3000を超えていない場合
      if (beat_count > last_beat_count){
        beat_num_count = beat_count - last_beat_count;
        for(int i=0; i <= beat_num_count; i++){
          root["beat"].add(*(p1+i));
        }
      }
      //beat_countが前回のpostの時と比較して3000を超えた場合
      else if(last_beat_count >= beat_count){
        beat_num_count = 3000 - last_beat_count + beat_count;
        for(int i=0; i <= beat_num_count; i++){
          if (i < 3000-last_beat_count){
            root["beat"].add(*(p1+last_beat_count+i));
          }
          else{
            root["beat"].add(*(p1+i-last_beat_count));
          }
        }
      }
      Serial.println("*************************************");
      serializeJson(root,Serial);
      Serial.println(" "); 
      serializeJson(root, buffer, sizeof(buffer));
      HttpConnect();
      int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
      Serial.print("PostCode = ");
      Serial.println(postcode);
      Serial.print("BufferSize = ");
      Serial.println(strlen(buffer));
      Serial.print("BeatNum = ");
      Serial.println(beat_num_count);  
      last_beat_count = beat_count;
      // 明示的に動的メモリの開放を行う必要あり。
      for(int i=0; i <= beat_num_count; i++){
        root["beat"].remove(0);
      }
      DynamicJsonDocument root(50000);
      JsonArray Beat = root.createNestedArray("beat");
      sensor_power = digitalRead(onoff_pin);
    }
  }
  // センサーのデータ出力がない場合はディープスリープモードで省電力化の実装
  else if (sensor_power == LOW){
    digitalWrite(led_pin, LOW);
    now_time = time(NULL);
    timeinfo = localtime(&now_time);  
    sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Serial.print("[OFF] ");
    Serial.println(current_time);
    is_measurement_started = false;  
    delay(2000);
  }
}
//      xTaskCreatePinnedToCore(TaskPost, "taskpost", 4096, NULL, 1, NULL, 1);
//      while(sensor_power == HIGH){
//        
//        if(sensor_power == LOW){
//          break;
//        }
//      }
