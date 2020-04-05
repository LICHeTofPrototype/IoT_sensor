
#include <ArduinoJson.h>

//***********************************
int time_interval = 10;  //測定間隔(ms)
//***********************************
volatile int beat_array[3000];
int *p1;
volatile long int beat_count = 0;
volatile long int last_beat_count = 0; //最後にデータを送信した時のbeat_arrayの
int beat_num_count; // 最後にデータを送信した時から現在データを送信する時までにbeat_arrayにデータが保存された回数
//boolean que;

void SaveBeatData(void *pvParameters){
  Serial.println("[ON ] Start SaveBeatData");
  while(1){
    sensor_power = digitalRead(onoff_pin);
//    if (que == true){
//      now_time = time(NULL);
//      timeinfo = localtime(&now_time);
//      sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
//      root["time"] = current_time;
//      que = false;
//    }
    if (sensor_power == HIGH) {
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
      vTaskDelay(time_interval);      
    }
    //TODO センサーのデータ出力がない場合にbeat_countがどのような状態で保存されているか要検討
    if(sensor_power == LOW){
      break;
    }
  }
}
//void PostBeat(char* task){
//  while(1){
//     vTaskDelay(10000);
//     int sensor_power = digitalRead(onoff_pin);
//     
//     if (sensor_power == HIGH ){
//       Serial.println("[ON ] Start PostBeat");
//       
//       if (beat_count > last_beat_count){
//        beat_num_count = beat_count - last_beat_count;
//        for(int i=0; i <= beat_num_count; i++){
//         root["beat"].add(*(p1+i));
//        }
//       }
//       else if(last_beat_count >= beat_count){
//        beat_num_count = 3000 - last_beat_count + beat_count;
//        for(int i=0; i <= beat_num_count; i++){
//          if (i < 3000-last_beat_count){
//            root["beat"].add(*(p1+last_beat_count+i));
//          }
//          else{
//            root["beat"].add(*(p1+i-last_beat_count));
//          }
//        }
//       }
//       que = true;
//       Serial.println("*************************************");
//       serializeJson(root,Serial);
//       Serial.println(" "); 
//       serializeJson(root, buffer, sizeof(buffer));
//
//       HttpConnect();
//       int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
//       Serial.print("PostCode = ");
//       Serial.println(postcode);
//       
//       Serial.print("BufferSize = ");
//       Serial.println(strlen(buffer));
//       Serial.print("BeatNum = ");
//       Serial.println(beat_num_count);
//       
//       last_beat_count = beat_count;
//       
//       for(int i=0; i <= beat_num_count; i++){
//         root["beat"].remove(0);
//       }
//       DynamicJsonDocument root(50000);
//       JsonArray Beat = root.createNestedArray("beat");
//     }
//     else if (sensor_power == LOW){
//       HttpDisConnect();
//       HttpConnectEnd();
//       EndPost();
//       break;
//     }
//  }
//  
//}
