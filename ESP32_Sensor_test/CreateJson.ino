
#include <ArduinoJson.h>

//***********************************
int timeInterval = 5;  //測定間隔(ms)
const int arrayNum = 2000;  //配列に入れる要素数
//***********************************

const int strNum = 40 + arrayNum*4 + arrayNum - 1 +  100;
char buffer[strNum];

void CreateJson(){

  DynamicJsonDocument root(40000);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        
  root["time"] = CurrentTime;
  root["measurement_id"] = MeasurementID;

  JsonArray Beat = root.createNestedArray("beat");
  
  for(int i=1 ; i <= arrayNum; i++){
    Signal = 0.8 * S + 0.2 * analogRead(SensorPin);
    Beat.add(Signal);
    S = Signal;
    delay(timeInterval);
  }

  int SensorPower = digitalRead(OnOffPin);
  if (SensorPower == HIGH){
    serializeJson(root, buffer, sizeof(buffer));
    int postCode = client.POST((uint8_t *) buffer, strlen(buffer));
  }else if (SensorPower == LOW){
    HttpConnectEnd();
    EndPost();
  }
 
  Serial.flush();
}
