
#include <HTTPClient.h>

void HttpConnect(){
  
  client.begin(url);
  //Serial.println("[1  ] Http Connected");
  client.addHeader("Content-Type", "application/json");
}

void HttpDisConnect(){  
  client.end();
  Serial.println("[1  ] Http Disonnected");
}
