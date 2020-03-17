
#include <ArduinoJson.h>


//***********************************
//10ms, arrayNum = 1000のとき, MAX(strlen(buffer)) = 4028
int timeInterval = 10;  //測定間隔(ms)
int arrayNum = 1000;  //配列に入れる要素数
char buffer[5050];  //バッファ strlen(buffer)で確認できる 
//unsigned char* buffer;
//***********************************
//int = arrayNum + 1;

void CreateJson(){

  //StaticJsonDocument<JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(1)> root;
  //StaticJsonDocument<6002> root;
  DynamicJsonDocument root(20000);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  root["time"] = CurrentTime;
  //root["delta"] = timeInterval;

  JsonArray Beat = root.createNestedArray("beat");
  Serial.println("----------------------------------------------");
  
  for(int i=1 ; i <= arrayNum; i++){
    int Signal = analogRead(SensorPin);  
    Beat.add(Signal);
    delay(timeInterval);
  }

  serializeJson(root, Serial); //シリアルモニタにjsonを表示
  Serial.print("\n");
  serializeJson(root, buffer, sizeof(buffer)); //バッファにjsonを格納
  Serial.println(strlen(buffer));
  
  //POSTはここでしてる
  int postCode = client.POST((uint8_t *) buffer, strlen(buffer));
  
  if( postCode == 201 ){
    Serial.print("[---] Success on sending POST: ");
    Serial.println(postCode); 
  }else{
    Serial.print("[***] Error on sending POST: ");
    Serial.println(postCode); 
  }

  Serial.flush();
}
