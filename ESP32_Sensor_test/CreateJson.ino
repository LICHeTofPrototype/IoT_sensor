
#include <ArduinoJson.h>

//***********************************
int time_interval = 10;  //測定間隔(ms)
//***********************************
int signal;
int signal_pre =1800;
int beat_array[3000];
//int *p1;
volatile int beat_count = 0;
volatile int last_beat_count = 0; //最後にデータを送信した時のbeat_arrayの個数
// int beat_num_count; // 最後にデータを送信した時から現在データを送信する時までにbeat_arrayにデータが保存された回数

const int array_num = 2000;  //配列に入れる要素数
const int str_num = 40 + array_num*4 + array_num - 1 +  3000;
char buffer[str_num];

void SaveBeatData(char* task){
//void SaveBeatData(void *pvParameters){
  //Serial.println("[ON ] Start SaveBeatData");
  while(1){
    //if (is_measurement_start){
      sensor_power = digitalRead(onoff_pin);
      //Serial.print("beat_count = "); 
      //Serial.println(beat_count); 
      if (sensor_power == HIGH) {
        //Serial.println("A"); 
        signal = 0.8 * signal_pre + 0.2 * analogRead(sensor_pin);
        if(beat_count < 3000){
          beat_array[beat_count] = signal;
          signal_pre = signal;
          beat_count += 1;
        } else {
          beat_count = 0;
          beat_array[beat_count] = signal;
          signal_pre = signal;
          beat_count += 1;
        }      
      }
      //TODO センサーのデータ出力がない場合にbeat_countがどのような状態で保存されているか要検討 
      else if(sensor_power == LOW){
        break;
      }
//    } else {
//      break;
//    }
    vTaskDelay(time_interval);
    //delay(time_interval);
  }
}

void PostBeat(char* task){
  while (1){
    vTaskDelay(10000); // 測定時間＝10秒
    Serial.println("testß");
//    //delay(10000);
//    DynamicJsonDocument root(50000);
//    JsonArray Beat = root.createNestedArray("beat");
//    //if (is_measurement_start){
//    sensor_power = digitalRead(onoff_pin);
//      if (sensor_power == HIGH) {
//        //beat_countが前回のpostの時と比較して3000を超えていない場合
//        if (beat_count > last_beat_count){
//        
//          Serial.println("B"); 
//          int beat_num_count = beat_count - last_beat_count;
//          Serial.print("BeatNum = ");
//          Serial.println(beat_num_count);
//          
//          if (beat_num_count > 1000){
//            Serial.println("C");
//            //vTaskDelay(10000); // 測定時間＝10秒
//            //StaticJsonDocument<str_num> root;
//             //Serial.println("[ON ] Start Post Data");
//            now_time = time(NULL);
//            timeinfo = localtime(&now_time);
//            sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
//            root["time"] = current_time;
//            root["dev_id"] = device_ID;
//            root["measurement_id"] = measurement_ID;
//            for(int i=0; i <= beat_num_count; i++){
//              root["beat"].add(*(beat_array+i));
//            }
//            //Serial.println("*************************************");
//            serializeJson(root,Serial);
//            //Serial.println(" "); 
//            serializeJson(root, buffer, sizeof(buffer));
//            HttpConnect();
//            int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
//            HttpDisConnect();
////            Serial.print("PostCode = ");
////            Serial.println(postcode);
//            Serial.print("BufferSize = ");
//            Serial.println(strlen(buffer));
////            Serial.print("BeatNum = ");
////            Serial.println(beat_num_count);
//            last_beat_count = beat_count;
//            root.clear();
//          }
//        }
//        //beat_countが前回のpostの時と比較して3000を超えた場合
//        else if(last_beat_count >= beat_count){
//          Serial.println("D");
//          int beat_num_count = 3000 - last_beat_count + beat_count;
//          
//          if (beat_num_count > 1000){
//            Serial.println("E");
//            //StaticJsonDocument<str_num> root;
//            //Serial.println("[ON ] Start Post Data");
//            now_time = time(NULL);
//            timeinfo = localtime(&now_time);
//            sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
//            root["time"] = current_time;
//            root["dev_id"] = device_ID;
//            root["measurement_id"] = measurement_ID;
//            
//            for(int i=0; i <= beat_num_count; i++){
//              if (i < 3000-last_beat_count){
//                root["beat"].add(*(beat_array+last_beat_count+i));
//              }
//              else{
//                root["beat"].add(*(beat_array+i-last_beat_count));
//              }
//            }
//            //Serial.println("*************************************");
//            serializeJson(root,Serial);
//            //Serial.println(" "); 
//            serializeJson(root, buffer, sizeof(buffer));
//            HttpConnect();
//            int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
//            HttpDisConnect();
////            Serial.print("PostCode = ");
////            Serial.println(postcode);
//            Serial.print("BufferSize = ");
//            Serial.println(strlen(buffer));
//            Serial.print("BeatNum = ");
//            Serial.println(beat_num_count);
//            last_beat_count = beat_count;
//            root.clear();
//          }
//        }
//      }
//      else if(sensor_power == LOW){
//        Serial.println("E");
//        break;
//      }
////    } else {
////      break;
////    }
//    root.clear();
  }
}
