//Remember address are usually 6 bits, then the remaining 2 bits are read/write and multiple/single bite. 
// If address 111000, if i want to read singular byte, 10111000 -> 1 byte
// If address 111000, if i want to write singular byte, 00111000 -> 1 byte
// If address 111000, if i want to read multiple byte, 11111000 -> 1 byte
// If address 111000, if i want to write multiple byte, 01111000 -> 1 byte
// At first need to close "Chip_Select", to stop all communications. Only applicable for SPI_Mode3
// Once u want to communicate, set "Chip_Select" to low, then do what u want to do, followed by "Chip_Select" to high. 
// Here they did use BitwiseOR, since the data of x,y,z are 2 bytes, but the arduino UNO can only receive 1byte at a time. Therefore, they use BitWise OR and BitWise Shift. 
// All the values are in 0x (HEX) but the address and everything are in (BINARY). So what they do is they give Hex val, and the computer auto translate to binary. 

#include <SPI.h> 
#define CS_Pin  10
#define LED 3
#define pi 3.14159
SPISettings settingsA(2000000, MSBFIRST, SPI_MODE3); // SPI_Mode3 means if high no signal, low only got signal 


int range = 8; // measurement range is 8g 
float scale =0.0000;
float angleZ = 0.00; //convert acceleration Z axis to rotational angle (but not accurate)
float rotationDirection = 0.00;
int LED_State = LOW; 
int intervalTime = 200;


void setup() { 
  Serial.begin(9600);
  pinMode(CS_Pin, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(CS_Pin,HIGH); //default off the communication firs
  SPI.begin();
  SPI.beginTransaction(settingsA);


  //Read the ID, pg15 
  digitalWrite(CS_Pin, LOW);
  SPI.transfer(0x80);
  int id = SPI.transfer(0);
  digitalWrite(CS_Pin, HIGH);

  //print the ID of the device 
  Serial.println("ID: "+ String(id, BIN));

  //Power on the controller 
  digitalWrite(CS_Pin, LOW);
  SPI.transfer(0x2D);
  SPI.transfer(0x08);
  digitalWrite(CS_Pin, HIGH);

  //Set the range
  digitalWrite(CS_Pin, LOW);
  SPI.transfer(0x31);
  switch(range) {
    case 2:
      SPI.transfer(0x00);
      break;
    case 4:
      SPI.transfer(0x01);
      break;
    case 8:
      SPI.transfer(0x02);
      break;
    case 16:
      SPI.transfer(0x03);
      break;
    default:
      Serial.println("Please define range");
      break;
  digitalWrite(CS_Pin, HIGH);
  }
  
  //Set the scale 
  switch(range) {
    case 2:
      scale = 0.0039;
      break;
    case 4:
      scale = 0.0078;
      break;
    case 8:
      scale = 0.0156;
      break;
    case 16:
      scale = 0.0312;
      break;
    default:
      scale = 1;
      Serial.println("scale not defined");
      break;
 }
}

void loop() {
  if (scale == 1) return; // stops the loop if scale == 1, means its not defined properly 
  unsigned long currentMillis = millis();
  static long previousMillis =0.000;
  
  digitalWrite(CS_Pin, LOW);
  SPI.transfer(0xF2);
  volatile int x = SPI.transfer(0) | SPI.transfer(0) << 8;
  volatile int y = SPI.transfer(0) | SPI.transfer(0) << 8;
  volatile int z = SPI.transfer(0) | SPI.transfer(0) << 8;
  digitalWrite(CS_Pin, HIGH);
  
  float accelX = x*scale;
  float accelY = y*scale;
  float accelZ = z*scale;

  angleZ = acos(accelZ) *180 / pi;
  directionCondition(accelY, accelX); // reduce global variable storage 
  if(accelZ >1) angleZ = 0;
  if (angleZ <= 10 && angleZ >= -10) {
    if (currentMillis - previousMillis >= intervalTime) { //why use this instead of delay? Because delay will delay whole script, but millis won't. 
      if  (LED_State ==LOW) LED_State = HIGH;
      else LED_State =LOW;
      digitalWrite(LED, LED_State);
    }
    angleZ = 0; 
    Serial.println(String(angleZ) + " degrees"); 
  }
  else {
    angleZ = round(angleZ* 2) * 0.5; //change it to only 1dp.  0 and 0.5 
    digitalWrite(LED,LOW); 
    Serial.println(String(angleZ) + " degree");
  }  
   delay(250);
}


void directionCondition(float accely, float accelx){ //determines the direction of rotation. Clockwise = +ve
  if ( accely >= accelx ) rotationDirection = 1;
  else rotationDirection = -1 ; 
  angleZ *= rotationDirection;
}
