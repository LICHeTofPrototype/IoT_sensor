
#include <ArduinoJson.h>

//***********************************
int time_interval = 10;  //測定間隔(ms)
const int array_num = 2000;  //配列に入れる要素数
//***********************************

const int str_num = 40 + array_num*4 + array_num - 1 +  3000;
char buffer[str_num];

DynamicJsonDocument root(50000);
JsonArray Beat = root.createNestedArray("beat");

int beat_array[3000];
int beat_num;
long int k = 0;
int beat_num_out;
boolean que;


void SaveBeatData(char* task){
  
  Serial.println("[ON ] Start CreateJson");
  now_time = time(NULL);
  timeinfo = localtime(&now_time);
  sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
  root["time"] = current_time;
  root["dev_id"] = device_ID;
  root["measurement_id"] = measurement_ID;
  
  while(1){
    
    if (que == true){
      int beat_array[3000];
      now_time = time(NULL);
      timeinfo = localtime(&now_time);
      sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      root["time"] = current_time;
      que = false;
      k = 0;
    } 

    signal = 0.8 * signal_pre + 0.2 * analogRead(sensor_pin);
    beat_array[k] = signal;
    signal_pre = signal;
    k += 1;
    vTaskDelay(time_interval);
   

    int sensor_power = digitalRead(onoff_pin);
    if (sensor_power == LOW){
      break;
    }
  }
}


void PostBeat(char* task){
  
  while(1){

     int sensor_power = digitalRead(onoff_pin);
     vTaskDelay(10000);
     
     if (sensor_power == HIGH ){
       Serial.println("[ON ] Start PostBeat");
 
       beat_num_out = k;
       
       for(int i=0; i <= beat_num_out; i++){
         root["beat"].add(beat_array[i]);
       }
       
       que = true;
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
       Serial.println(beat_num_out);
       
       for(int i=0; i <= beat_num_out; i++){
         root["beat"].remove(0);
       }
      
      }else if (sensor_power == LOW){
       HttpDisConnect();
       HttpConnectEnd();
       EndPost();
       break;
     }
  }
  
}
