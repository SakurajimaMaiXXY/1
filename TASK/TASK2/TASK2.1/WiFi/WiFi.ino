#include <WiFi.h>
void setup() {
  Serial.begin(115200);
  WiFi.begin("ASUKA","12345678");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
  }
  Serial.print("IP");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
