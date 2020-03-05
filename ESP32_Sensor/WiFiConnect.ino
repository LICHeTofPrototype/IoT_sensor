
#include <WiFi.h>
//WiFiClient wifiClient;

void WiFiConnect(){
  WiFi.begin(ssid, password); 
  count = 0;
  
  while(WiFi.status() != WL_CONNECTED){
    count += 1;
    delay(500);
    Serial.print(".");
    
    if(count = 10){
      Serial.print("\n");
      Serial.println("[!  ] Try Connecting WiFi again");
      break;
    }
  }
   Serial.print("\n");
   Serial.println("[1  ] WiFi Connected");
   Serial.print("\n");
}


void WiFiDisConnect() {
  WiFi.disconnect();
  Serial.println("[1  ] WiFi Disconnected");
}
