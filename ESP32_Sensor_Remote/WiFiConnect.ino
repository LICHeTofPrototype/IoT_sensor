
#include <WiFi.h>

void WiFiConnect(){

  while(1){
    WiFi.begin(ssid, password); 
    count = 0;

    while (WiFi.status() != WL_CONNECTED){
      if(count == 5){
        break;
      }else if (count % 2 == 0){
        digitalWrite(WiFiPin, LOW);
      }else {
        digitalWrite(WiFiPin, HIGH);
      }
      delay(1000);
      count += 1;    
    }

    if (WiFi.status() == WL_CONNECTED){
      digitalWrite(WiFiPin, HIGH);
      configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
      break;
    }
  }   
}


void WiFiDisConnect() {
  WiFi.disconnect();
  digitalWrite(WiFiPin, LOW);
}
