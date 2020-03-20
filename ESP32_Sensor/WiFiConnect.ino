
#include <WiFi.h>

void WiFiConnect(){

  while(1){
    WiFi.begin(ssid, password); 
    count = 0;

    while (WiFi.status() != WL_CONNECTED){

      if(count == 5){
        Serial.print("\n");
        Serial.println("[!!!] Try Connecting WiFi again");
        break;
      }else if (count % 2 == 0){
        digitalWrite(WiFiPin, LOW);
      }else {
        digitalWrite(WiFiPin, HIGH);
      }
      
      Serial.print(".");
      delay(1000);
      count += 1;
      
    }

    if (WiFi.status() == WL_CONNECTED){
      digitalWrite(WiFiPin, HIGH);
      Serial.print("\n");
      Serial.println("[---] WiFi Connected");
      configTime( JST, 0, "ntp.nict.jp");
      break;
    }
  }
     
}


void WiFiDisConnect() {
  WiFi.disconnect();
  digitalWrite(WiFiPin, LOW);
  Serial.println("[---] WiFi Disconnected");
}
