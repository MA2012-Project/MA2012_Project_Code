// Address of the I2c 0x53 
#include <Wire.h>
#define accelAddress 0x53
#define LED 3 
#define pi 3.14159
#define timeInterval 200 // 250 millisecond 


int range =8;
int LED_State = LOW; 
float scale = 0.000;
float angleZ = 0.00;



void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  Wire.begin();
   //Turn on the sensor, pg16
  Wire.beginTransmission(accelAddress);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();

  
  //Set the range, pg17
  Wire.beginTransmission(accelAddress);
  Wire.write(0x31);
  switch(range){
    case 2:
      Wire.write(0x00);
      break;
    case 4:
      Wire.write(0x01);
      break;
    case 8:
      Wire.write(0x02);
      break;
    case 16:
      Wire.write(0x03);
      break;
    default: 
      Serial.println("Range not defined");
      break;
  }
  Wire.endTransmission();

  //Set scale number, pg3
  switch(range){
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
      Serial.println("Scale not defined");
      break;
  }
}
void loop() {
  if (scale ==1) return; //if range not define end void loop 
  unsigned long currentMillis = millis();
  static volatile float previousMillis = 0.000;

  
  Wire.beginTransmission(accelAddress);
  Wire.write(0x32);
  Wire.endTransmission(false); //I still dunno why need this.  
  Wire.requestFrom(accelAddress, 6, true);
  int X = Wire.read() | Wire.read() << 8;
  int Y = Wire.read() | Wire.read() << 8;
  int Z = Wire.read() | Wire.read() << 8;
  //Wire.endTransmission(true); //can either put at 4 lines above as false, or last here and true
  

  float accelX = X*scale;
  float accelY = Y*scale;
  float accelZ = Z*scale;
  
  
  angleZ = acos(accelZ) *180 / pi;
  directionCorrection(accelX, accelY);
  if (accelZ>1) angleZ = 0;

  if (angleZ <= 10 && angleZ >= -10) {
    if (currentMillis - previousMillis >= timeInterval) {
      previousMillis = currentMillis;
      if (LED_State == LOW) LED_State = HIGH;
      else LED_State = LOW;
      digitalWrite(LED, LED_State);
    }
      angleZ = 0;
  }
   
  else {
    angleZ = round(angleZ* 2) * 0.5; //change it to only 1dp.  0 and 0.5 
    digitalWrite(LED,LOW); 
   }
  Serial.println("angleZ = "+ String(angleZ) + "degree");
  delay(250);
}

 
void directionCorrection(float accelx, float accely){
  int rotationDirection =0;
  if (accely > accelx) rotationDirection = 1;
  else rotationDirection = -1;
  angleZ *= rotationDirection;
}
