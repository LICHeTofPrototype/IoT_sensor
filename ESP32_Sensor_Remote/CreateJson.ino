
#include <ArduinoJson.h>

//***********************************
int timeInterval = 10;  //測定間隔(ms)
const int arrayNum = 1000;  //配列に入れる要素数
const int strNum = 29 + arrayNum*4 + arrayNum - 1 +  100;
char buffer[strNum];
//***********************************

void CreateJson(){

  DynamicJsonDocument root(20000);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  root["time"] = CurrentTime;

  JsonArray Beat = root.createNestedArray("beat");
  
  for(int i=1 ; i <= arrayNum; i++){
    int Signal = analogRead(SensorPin);  
    Beat.add(Signal);
    delay(timeInterval);
  }

  serializeJson(root, buffer, sizeof(buffer)); //バッファにjsonを格納
  int postCode = client.POST((uint8_t *) buffer, strlen(buffer));

  Serial.flush();
}
