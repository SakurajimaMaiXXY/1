#include <Arduino.h>

void setup() {
    // 初始化串口，设置波特率为115200
    Serial.begin(115200);
    while (!Serial) {
        // 等待串口连接（对于某些开发板是必需的）
    }
    Serial.println("Serial communication initialized at 115200 baud");
}
String dataToSend = "Hello, World!";
void sendData(String data) {
    Serial.println(data);
}

void loop() {
    // 调用发送函数
    sendData(dataToSend);
    
    // 为了避免发送过于频繁，可以添加一些延时
    delay(1000); // 延时1秒
}
