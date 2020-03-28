
#include <ArduinoJson.h>

char start_buffer[100];
char end_buffer[100];

void StartPost(){
  DynamicJsonDocument root(400);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  //sprintf(CurrentDate, "%04d-%02d-%02d %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
  
  root["start_time"] = CurrentTime;
  root["dev_id"] = DeviceID;

  serializeJson(root, start_buffer, sizeof(start_buffer));
  int postCode = client.POST((uint8_t *) start_buffer, strlen(start_buffer));
  Serial.println(postCode);

  String response = client.getString();
  Serial.println(response);
  MeasurementID = response.toInt();

  if( postCode == 201 ){
      Serial.print("[---] Success on sending POST: ");
      Serial.println(postCode); 
      HttpDisConnect();
    }else{
      Serial.print("[***] Error on sending POST: ");
      Serial.println(postCode);
      HttpDisConnect();
      return ; 
    }
}



void EndPost(){
  DynamicJsonDocument root(400);
  NowTime = time(NULL);
  timeInfo = localtime(&NowTime);
  sprintf(CurrentTime, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
   //sprintf(CurrentDate, "%04d-%02d-%02d %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
  root["endtime"] = CurrentTime;
  
  serializeJson(root, end_buffer, sizeof(end_buffer));
  int postCode = client.POST((uint8_t *) end_buffer, strlen(end_buffer));
  Serial.println(postCode);
}
