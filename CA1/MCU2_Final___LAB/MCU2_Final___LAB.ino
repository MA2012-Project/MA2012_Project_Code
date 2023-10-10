//Ultrasonic Sensor & Piezo Buzzer. 
#define ultrasonicSensor_trig 12
#define ultrasonicSensor_echo 11
#define piezoBuzzer 3

volatile float duration = 0;
volatile float distance = 0; 

//SOLENOID DEFINE
#define solenoidPin 2 

//SOLENOID VARIABLE - (EMPTY) 

//LED & PUSH BUTTON  DEFINE
#define LED_GREEN 5 //digital //Note OFF(HIGH), ON(LOW)
#define LED_RED 6 //digital
#define pushButton 4

//EXTRA MARK Orange LED: 
#define LED_ORANGE 10

//LED & PUSH BUTTON VARIABLE 
int pushButtonStatus = 0;

//SERVOMOTOR DEFINE
#include <Servo.h>
#define servoPin 9

//SERVOMOTOR VIARIABLE
int signalDuration =0; 

//SERVOMOTOR Object 
Servo myServo; 

//MCU2 COMMS DEFINE
#include <SoftwareSerial.h>
#define RxD 7
#define TxD 8

//MCU2 COMMS VARIABLE
char receivedChar;
String receivedStr = "";

//MCU2 COMMS OBJECT
SoftwareSerial slave(RxD,TxD);


void setup() {
  
  Serial.begin(9600);
  
  //LED & PUSH BUTTON
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT); 
  pinMode(pushButton, INPUT);

  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_ORANGE, LOW); 

  //SOLENOID
  pinMode(solenoidPin, OUTPUT); //note that high means unlock, low means lock 

  //SERVOMOTOR
  myServo.attach(servoPin);
  myServo.writeMicroseconds(1120); //default close door   
  
  //MCU2 COMMS
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  slave.begin(9600);

  // Ultrasonic Sensor setup & Piezo Buzzer Setup: 
  pinMode(ultrasonicSensor_trig, OUTPUT);
  pinMode(ultrasonicSensor_echo, INPUT);
  pinMode(piezoBuzzer, OUTPUT);

}

void loop() {
  //MCU2_COMMS

  //For turning Orange LED ON if object detected within 1.2m. 
  //This code needs to run in LOCKED mode as well. 
  
  getUltraSonicReading();
  if (distance > 0 && distance <= 120) {
    digitalWrite(LED_ORANGE, LOW);
  }
  else {
    digitalWrite(LED_ORANGE, HIGH); 
  }
  
  if (slave.available()>0){
    receivedChar = slave.read();
    receivedStr = String(receivedStr + String(receivedChar));
    Serial.println(receivedStr);
  }
  else {
    if (receivedStr == "LOCKED") {
      Serial.println("LOCKED");
      slave.write(true);
      digitalWrite(LED_GREEN,HIGH);
      digitalWrite(LED_RED,LOW);
      myServo.writeMicroseconds(1120); //close door
      receivedStr = ""; 
      while (1) { // Enter LOCKED mode & while loop keeps LOCKED state. 

        //Keep getting distance reading for Orange LED check. 
        getUltraSonicReading();
        if (distance > 0 && distance <= 120) {
          digitalWrite(LED_ORANGE, LOW);
        }
        else {
          digitalWrite(LED_ORANGE, HIGH); 
        }

        // Checking if there is data to be read from MCU1. 
        if (slave.available()>0) { //If signal sent from Master
          receivedChar = slave.read(); 
          receivedStr = String(receivedStr + String(receivedChar));
          if (receivedStr == "NORMAL") {
            slave.write(true);
            receivedStr = "";
            break;
          } 
        }
      }
    }
  }
  
  Serial.println("NORMAL");
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,HIGH);

  //PUSH BUTTON
  pushButtonStatus = digitalRead(pushButton);
  //Serial.print(pushButtonStatus); //Check pushButton Status
  delay(100);

  if ( pushButtonStatus == 0 ){
    
    //Getting distance reading from Ultrasound Sensor when pushbutton pressed.
    getUltraSonicReading();
    
    //Checking if there is object within 0.6m: 
    
    if (distance > 0 && distance <= 60){
      tone(piezoBuzzer, 400, 3000);
      delay(200);
    }

    else{ 
      digitalWrite(LED_ORANGE, LOW); 
      digitalWrite(solenoidPin, HIGH);
      delay(600);
      myServo.writeMicroseconds(1920);
      delay(10000);
      myServo.writeMicroseconds(1120);
      delay(600);
      digitalWrite(solenoidPin,LOW);
      delay(600);
      digitalWrite(LED_ORANGE, HIGH); 
    } 
  }

}


 
void getUltraSonicReading () { //For getting distance reading. 
    digitalWrite(ultrasonicSensor_trig, LOW);
    delayMicroseconds(2);
    //Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(ultrasonicSensor_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonicSensor_trig, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ultrasonicSensor_echo, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Displays the distance on the Serial Monitor
    Serial.println("Distance: " + String(distance) + " cm ");
}


  
