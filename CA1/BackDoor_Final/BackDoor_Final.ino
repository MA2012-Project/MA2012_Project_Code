//SOLENOID DEFINE
#define solenoidPin 2 

//SOLENOID VARIABLE - (EMPTY) 

//LED & PUSH BUTTON  DEFINE
#define LED_GREEN 5 //digital //Note OFF(HIGH), ON(LOW)
#define LED_RED 6 //digital
#define pushButton 4

//LED & PUSH BUTTON VARIABLE 
int pushButtonStatus = 0;

//SERVOMOTOR DEFINE
#include <Servo.h>
#define servoPin 9

//SERVOMOTOR VIARIABLE
int signalDuration =0; 

//SERVOMOTOR Object 
Servo myServo; 



void setup() {
  Serial.begin(9600);
  
  //LED & PUSH BUTTON
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(pushButton, INPUT);

  //SOLENOID
  pinMode(solenoidPin, OUTPUT); //note that high means unlock, low means lock 

  //SERVOMOTOR
  myServo.attach(servoPin);
  myServo.writeMicroseconds(1120); //default close door 
}

void loop() {
  //PUSH BUTTON 
  pushButtonStatus = digitalRead(pushButton);
  Serial.print(pushButtonStatus);
  delay(100);
  
  //if normal 
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  if ( pushButtonStatus == 0){
    digitalWrite(solenoidPin, HIGH);
    delay(300);
    myServo.writeMicroseconds(1920);
    delay(10000);
    myServo.writeMicroseconds(1120);
    delay(300);
    digitalWrite(solenoidPin, LOW);
  }

  //if locked 
  /*digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW); 
  myServo.writeMicroseconds(1920); */
}
