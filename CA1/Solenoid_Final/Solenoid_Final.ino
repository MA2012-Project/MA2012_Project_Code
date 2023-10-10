#define solenoidPin 9 //be sure to connect the arduino to the ground 

void setup() {
  Serial.begin(9600);
  pinMode(solenoidPin, OUTPUT);


}

void loop() {
  digitalWrite(solenoidPin, HIGH);
  delay(3000);
  digitalWrite(solenoidPin, LOW);
  delay(2000);
  

}
