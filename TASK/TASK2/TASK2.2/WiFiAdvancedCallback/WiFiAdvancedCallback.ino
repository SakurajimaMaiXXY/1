
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
  #include <WiFi.h>
#elif defined(ARDUINO_PORTENTA_C33)
  #include <WiFiC3.h>
#elif defined(ARDUINO_UNOR4_WIFI)
  #include <WiFiS3.h>
#endif
#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"   ////A client library for MQTT messaging.
#include <ArduinoJson.h>

#include "arduino_secrets.h"

#define LED 1
#define DHTPIN 4  // Defines pin number to which the sensor is connected

char ssid[] = "ASUKA";    // your network SSID (name)
char pass[] = "12345678910";    // your network password (use for WPA, or use as key for WEP)


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

PubSubClient  client(wifiClient);

const char broker[]    = "iot-06z00a5v13wqciu.mqtt.iothub.aliyuncs.com";
int        port        = 1883;
const char inTopic[]   = "/sys/k28ycH54tFN/ESP32-CAM/thing/event/property/post";
const char outTopic[]  = "/sys/k28ycH54tFN/ESP32-CAM/thing/service/property/set";

const long interval = 10000;
unsigned long previousMillis = 0;

int count = 0;

/*void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);   // 打印主题信息
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // 打印主题内容
  }
  Serial.println();

 DynamicJsonDocument doc(1024);  //创建了一个名为 doc 的动态 JSON 文档
 deserializeJson(doc, String((char *)payload));  //从一个名为 payload 的数据中解析 JSON 数据并将其填充到 doc 中

// DynamicJsonDocument params = doc["params"];

if(doc["params"].containsKey("灯"))
{
  Serial.println("GOT DENG CMD"); 
  digitalWrite(LED, doc["params"]["灯"]);
}

}
*/
void setup() {
  pinMode(LED, OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(500);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  mqttClient.setId("k28ycH54tFN.ESP32-CAM|securemode=2,signmethod=hmacsha256,timestamp=1736843240291|");
  mqttClient.setUsernamePassword("ESP32-CAM&k28ycH54tFN","4271ba61e7030b4b181f67263e8fc0c2613872c938585a5c7ec7d489eac21a40");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  /*client.setCallback(callback);
  digitalWrite(LED,LOW);*/

  Serial.print("Subscribing to topic: ");
  Serial.println(inTopic);
  Serial.println();

  // subscribe to a topic
  // the second parameter sets the QoS of the subscription,
  // the the library supports subscribing at QoS 0, 1, or 2
  int subscribeQos = 1;

  mqttClient.subscribe(inTopic, subscribeQos);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(inTopic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(inTopic);
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    String payload;

    payload = "{\"params\":{\"led\":1},\"version\":\"1.0.0\"}";

    Serial.print("Sending message to topic: ");
    Serial.println(outTopic);
    Serial.println(payload);

    // send message, the Print interface can be used to set the message contents
    // in this case we know the size ahead of time, so the message payload can be streamed

    bool retained = false;
    int qos = 1;
    bool dup = false;

    mqttClient.beginMessage(outTopic, payload.length(), retained, qos, dup);
    mqttClient.print(payload);
    mqttClient.endMessage();

    Serial.println();

    count++;
  }

}



void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();

  Serial.println();
}


/*void setup()
{
  pinMode(33,OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(500);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  mqttClient.setId("k28ycH54tFN.ESP32-CAM|securemode=2,signmethod=hmacsha256,timestamp=1736843240291|");
  mqttClient.setUsernamePassword("ESP32-CAM&k28ycH54tFN","4271ba61e7030b4b181f67263e8fc0c2613872c938585a5c7ec7d489eac21a40");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback   /* 连接MQTT服务器 
}*/

/*void loop()
{
  digitalWrite(33,LED);
} */