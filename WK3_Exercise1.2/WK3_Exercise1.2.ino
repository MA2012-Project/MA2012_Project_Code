//Exercise1.2 : Reverse Parking Sensor 
//lmao
#define ultrasonicSensor_trig 9
#define ultrasonicSensor_echo 8
#define piezoBuzzer 4
#define Slotted_Opto 5 
volatile int duration = 0;
volatile int Opto_state = 0; 
volatile int distance = 0; 


void setup() {
  pinMode(ultrasonicSensor_trig, OUTPUT);
  pinMode(ultrasonicSensor_echo, INPUT);
  pinMode(Slotted_Opto, INPUT); 
  pinMode(piezoBuzzer, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  Opto_state = digitalRead(Slotted_Opto);
  Serial.println("Opto_State :" + String(Opto_state));
  
  
  if (Opto_state == HIGH) {
    while(1) {
      Opto_state = digitalRead(Slotted_Opto);
      getUltraSonicReading();
      if (distance > 25) {
        //Serial.println("distance > 20");
        tone(piezoBuzzer, 200, 200);
        delay(400);
      }else if (distance > 10) { 
        //Serial.println("distance > 20");
        tone(piezoBuzzer, 400, 375);
        delay(400);
      }else if ( distance > 0 && distance <= 10) {
        //Serial.println("distance <= 20");
        tone(piezoBuzzer, 600,420);
        delay(400);
        //Serial.print("Yes");
      }else {
        Serial.println("Out of range");
        }
    if (Opto_state == LOW) { 
        break;
      }
        
    }
    
  }else {
  
  }
 }
  
  
void getUltraSonicReading () {
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
