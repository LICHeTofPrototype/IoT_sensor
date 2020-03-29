
#include <ArduinoJson.h>

char start_buffer[128];
char end_buffer[128];

void StartPost(){
  DynamicJsonDocument root(400);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentDate, "%04d-%02d-%02d %02d:%02d:%02d",timeInfo->tm_year+1900, timeInfo->tm_mon+1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
 
  root["start_time"] = CurrentDate;
  root["dev_id"] = DeviceID;

  serializeJson(root,Serial);
  Serial.println(" ");
  serializeJson(root, start_buffer, sizeof(start_buffer));
  int postCode = client.POST((uint8_t *) start_buffer, strlen(start_buffer));

  String response = client.getString();
  StaticJsonDocument<200> doc;
  JsonObject object = doc.as<JsonObject>();
  deserializeJson(doc, response);
  MeasurementID = doc["id"];

}



void EndPost(){
  DynamicJsonDocument root(400);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentDate, "%04d-%02d-%02d %02d:%02d:%02d",timeInfo->tm_year+1900, timeInfo->tm_mon+1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  
  root["end_time"] = CurrentDate;
  root["dev_id"] = DeviceID;
  root["measurement_id"] = MeasurementID;

  serializeJson(root,Serial);
  Serial.println(" ");
  serializeJson(root, end_buffer, sizeof(end_buffer));
  int postCode = client.POST((uint8_t *) end_buffer, strlen(end_buffer));
}
