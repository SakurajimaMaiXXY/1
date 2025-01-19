
#define  positive 1  //如果LED为共阳极，写1.如果是共阴极，写0

#if  positive
#define LED_on LOW
#define LED_off HIGH
#else
#define LED_on HIGH
#define LED_off LOW
#endif

//开机启动代码
void setup() {
  //pinMode
  pinMode(33,OUTPUT);
}
// 死循环
void loop() {
  digitalWrite(33, LED_on);  
  delay(1000);
  digitalWrite(33, LED_off);  
  delay(1000);
}

