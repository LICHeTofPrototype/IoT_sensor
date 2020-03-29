
#include <ArduinoJson.h>

//***********************************
int timeInterval = 5;  //測定間隔(ms)
const int arrayNum = 2000;  //配列に入れる要素数
//***********************************

const int strNum = 40 + arrayNum*4 + arrayNum - 1 +  3000;
char buffer[strNum];
DynamicJsonDocument root(50000);
JsonArray Beat = root.createNestedArray("beat");


void CreateJson(char* task){

  Serial.println("[ON ] Start CreateJson");
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        
  root["time"] = CurrentTime;
  root["dev_id"] = DeviceID;
  root["measurement_id"] = MeasurementID;
  
  while(1){
    Signal = 0.8 * S + 0.2 * analogRead(SensorPin);
    Beat.add(Signal);
    S = Signal;
    vTaskDelay(timeInterval);
 
    int SensorPower = digitalRead(OnOffPin);
    if (SensorPower == LOW){
      break;
    }
  }
}


void PostBeat(char* task){

  while(1){
    vTaskDelay(10000);
    Serial.println("[ON ] Start PostBeat");
    int SensorPower = digitalRead(OnOffPin);
    if (SensorPower == HIGH){

      serializeJson(root,Serial);
      Serial.println(sizeof(root));
      Serial.println(" ");
      serializeJson(root, buffer, sizeof(buffer));
    
      int postCode = client.POST((uint8_t *) buffer, strlen(buffer));
      
      Serial.println(strlen(buffer));
      int BeatNum = (strlen(buffer) - 63 + 1) / 5;
      Serial.println(BeatNum);
      
      for(int i=0; i<BeatNum; i++){
        root["beat"].remove(0);
      }
      
    }else if (SensorPower == LOW){
      HttpDisConnect();
      HttpConnectEnd();
      EndPost();
      break;
    }
  }
  
}
