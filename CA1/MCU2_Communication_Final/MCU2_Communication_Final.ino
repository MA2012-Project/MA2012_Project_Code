#include <SoftwareSerial.h>
#define RxD 7
#define TxD 8
#define LED_2 13
SoftwareSerial slave(RxD,TxD);

char receivedChar;
String receivedStr = "";//initialize string
//int receivedInt


void setup() {
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  slave.begin(9600);

  pinMode(LED_2, OUTPUT);
  
}

void loop() {
  if(slave.available()>0){
    receivedChar = slave.read();
    receivedStr = String(receivedStr + receivedChar);
  }     
  if (receivedStr == "LOCKED") {
    Serial.println(receivedStr);
    receivedStr = ""; //refreshed 
    writeBack();
  }
  if (receivedStr == "NORMAL") {
    Serial.println(receivedStr);
    receivedStr = ""; //refreshed 
    writeBack();

  }
}
void writeBack() {
  slave.write("true");
}
