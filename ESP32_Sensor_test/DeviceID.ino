
#include <ArduinoJson.h>

char start_buffer[128];
char end_buffer[128];

void StartPost(){
  DynamicJsonDocument start_obj(400);
  now_time = time(NULL);
  timeinfo = localtime(&now_time);
  sprintf(current_date, "%04d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
 
  start_obj["start_time"] = current_date;
  start_obj["dev_id"] = device_ID;

  serializeJson(start_obj,Serial);
  Serial.println(" ");
  serializeJson(start_obj, start_buffer, sizeof(start_buffer));
  int postcode = client.POST((uint8_t *) start_buffer, strlen(start_buffer));
  
  Serial.print("PostCode = ");
  Serial.println(postcode);
  
  String response = client.getString();
  StaticJsonDocument<200> doc;
  JsonObject object = doc.as<JsonObject>();
  deserializeJson(doc, response);
  measurement_ID = doc["id"];

}

void EndPost(){
  DynamicJsonDocument end_obj(400);
  now_time = time(NULL);
  timeinfo = localtime(&now_time);
  sprintf(current_date, "%04d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  
  end_obj["end_time"] = current_date;
  end_obj["dev_id"] = device_ID;
  end_obj["measurement_id"] = measurement_ID;

  serializeJson(end_obj,Serial);
  Serial.println(" ");
  serializeJson(end_obj, end_buffer, sizeof(end_buffer));
  int postcode = client.POST((uint8_t *) end_buffer, strlen(end_buffer));
  Serial.print("PostCode = ");
  Serial.println(postcode);
}
