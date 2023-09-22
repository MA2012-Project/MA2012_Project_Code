//Please research the switch mode for the sketch 
// 0 volt is 45deg, 5v is 135deg
// How do u want to control the servo? 
#include <Servo.h>

#define potentiometer 3
#define toggleSwitch 2
#define pushButton  1
#define dcMotor 6

char incomingByte; 
int direction =0;
int setPos = 0;
int passStatus = 0;
int angleInput =0;
//static volatile int potentiometerVal = 0; 
// 


Servo myServo; //creates an object 

void setup() {
  Serial.begin(9600);
  pinMode(potentiometer, INPUT);
  pinMode(toggleSwitch, INPUT);
  pinMode(pushButton,INPUT);
  pinMode(dcMotor,OUTPUT);
  myServo.attach(4); // try to use the manual digital method by delaying the high signal 

}

void loop() {
   userInput();
   angleInput = setPos;
   Serial.println(angleInput);
   if (passStatus ==1){
    servoAngle(myServo, angleInput);
    
   }
  
}



void servoAngle(Servo& servo, int angle) {
  //Neutral 1520us, Min 1120us, Max 1920us
  static int angle_mapped = 0;
  angle = constrain(angle, 0,120);
  angle_mapped = map(angle, 0, 120, 980,2054);
  servo.writeMicroseconds(angle_mapped);
  passStatus =0; //show that it has already been fed 
}

int userInput() {
  if (Serial.available() > 0){
    setPos = 0;
    while(1){
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;
      if (incomingByte == '\r') break;
      if (incomingByte == '-'){
        direction  =  1;
        }
      if ((incomingByte - 48) >=0 && (incomingByte - 48) <=9) {
        setPos *= 10;
        setPos = ( setPos + (incomingByte -48));
      }
    }
     if (direction == 1){
       setPos = -setPos;
     }
  }
  delay(500);
  return setPos; 
  passStatus = 1; // cuz i dont want it to always feed into servoAngle
    
}

void potentiometerInput() {
  int angle = 0;
  static volatile int potentiometerVal = 0;
  potentiometerVal = analogRead(potentiometer);
  angle = map(potentiometerVal,0,1023,0,120);
  return angle; 
  
  
  
}
