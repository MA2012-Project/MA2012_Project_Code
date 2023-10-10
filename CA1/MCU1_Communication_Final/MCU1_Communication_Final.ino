#include <SoftwareSerial.h>
#define RxD 9
#define TxD 8
#define LED_1 13 
SoftwareSerial master(RxD, TxD);

char receivedChar;
char readedChar;
String receivedStr =""; 



void setup() {
 Serial.begin(9600);
 pinMode(RxD, INPUT);   //received data from slave to master 
 pinMode(TxD, OUTPUT);  //send data from master to slave 
 pinMode(LED_1,OUTPUT);
 master.begin(9600); // Initialise the communication (Why do we need to set baud rate?) 
 digitalWrite(LED_1,LOW);
}

void loop() {
  //Serial.print(Serial.available());
  if (Serial.available()>0){ //if True
    readedChar = Serial.read();
    if (readedChar == '1'){
      Serial.println("Sending LOCKED");
      master.write("LOCKED");
    }
    if (readedChar == '2'){
      Serial.println("Sending NORMAL");
      master.write("NORMAL");
    }    
  }
  if (master.available()>0){
      receivedChar = master.read();
      receivedStr = String(receivedStr + receivedChar);
      if (receivedStr == "true"){
        Serial.println(receivedStr);
        Serial.println("Message passed");
        receivedStr = "";
      } 
  }
}
