//Servomotor location - Min 720 (-90), Neutral 1520, Max 2320 (90)
// We only use 45 (1120), 90 (1520), 135 (1920)
#include <Servo.h>
#define servoPin 9

int signalDuration =0;


Servo myServo; 

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);

}

void loop() {    
  
  myServo.writeMicroseconds(1120);
  delay(2000);
  myServo.writeMicroseconds(1520);
  delay(2000);
  myServo.writeMicroseconds(1920);
  delay(2000);  
  myServo.writeMicroseconds(1520);
  delay(2000);  
  
 

}
