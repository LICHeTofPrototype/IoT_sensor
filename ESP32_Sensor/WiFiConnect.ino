
#include <WiFi.h>

void WiFiConnect(){

  while(1){
    WiFi.begin(ssid, password); 
    count = 0;

    while (WiFi.status() != WL_CONNECTED){
      count += 1;
      delay(1000);
      Serial.print(".");

      //10秒経っても接続されない場合、再度アクセスしに行く
      if(count == 10){
        Serial.print("\n");
        Serial.println("[!  ] Try Connecting WiFi again");
        break;
      }
    }
    
    Serial.print("\n");
    Serial.println("[1  ] WiFi Connected");
    Serial.print("\n");
    break;
  }
     
}


void WiFiDisConnect() {
  WiFi.disconnect();
  Serial.println("[1  ] WiFi Disconnected");
}
