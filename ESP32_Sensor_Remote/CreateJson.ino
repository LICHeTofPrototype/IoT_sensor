
#include <ArduinoJson.h>

//***********************************
//10ms, arrayNum = 1000のとき, MAX(strlen(buffer)) = 5028
int timeInterval = 10;  //測定間隔(ms)
int arrayNum = 1000;  //配列に入れる要素数
char buffer[5050];  //バッファ strlen(buffer)で確認できる 
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
