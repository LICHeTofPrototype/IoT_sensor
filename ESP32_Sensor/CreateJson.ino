
#include <ArduinoJson.h>

//***********************************
int timeInterval = 50;  //ms
int arrayNum = 200;  //配列に入れる要素数
//arrayNum * 2 以上の数値をJSON_ARRAY_SIZE(**)に入れてね
//***********************************

void CreateJson(){
  StaticJsonDocument<JSON_ARRAY_SIZE(400) + JSON_OBJECT_SIZE(2)> root;
  JsonArray Time = root.createNestedArray("time");
  JsonArray Beat = root.createNestedArray("beat");
  
  for(int i=1 ; i <= arrayNum; i++){
    SampleCount += timeInterval;
    int Signal = analogRead(SensorPin);  
      
    Time.add(SampleCount);
    Beat.add(Signal);
    
    delay(timeInterval);
  }
  
  Serial.println("****************************");
  Serial.flush();
  
  serializeJson(root, Serial);
  Serial.print("\n");
  serializeJson(root, buffer, sizeof(buffer));

  int postCode = client.POST((uint8_t *) buffer, strlen(buffer));
  
  if( postCode == 201 ){
    Stream* resp = client.getStreamPtr();

    DynamicJsonDocument json_response(255);
    deserializeJson(json_response, *resp);
    
    serializeJson(json_response, Serial);
    Serial.print("\n");
  }else{
    Serial.print("[0  ] Error on sending POST: ");
    Serial.println(postCode); 
  }
  
  int getCode = client.GET();
  
  if(getCode > 0){
      String httpResponse = client.getString();                   
      Serial.printf("Response: %d", getCode);  
      Serial.println(httpResponse);  
    }else{
      Serial.print("[0  ] Error on sending GET: ");
      Serial.println(getCode);  
    }
  
  Serial.println("****************************");
}
