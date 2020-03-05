
#include <HTTPClient.h>

void HttpConnect(){
  String url = "http://";
    url += host;
    url += ":";
    url += PORT;
    url += "/api/calc_pnn/2/";

  Serial.println("[1  ] Http Connected");
  Serial.print("Requesting URL = ");
  Serial.println(url);
  Serial.print("\n");
  
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpDisConnect(){  
  client.end();
  Serial.println("[1  ] Http Disonnected");
}
