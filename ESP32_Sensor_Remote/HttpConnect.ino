
#include <HTTPClient.h>

void HttpConnect(){
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpDisConnect(){  
  client.end();
}
