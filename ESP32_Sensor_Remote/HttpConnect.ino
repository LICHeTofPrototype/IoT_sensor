
#include <HTTPClient.h>

void HttpConnect(){
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpConnectStart(){
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpConnectEnd(){
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpDisConnect(){  
  client.end();
}
