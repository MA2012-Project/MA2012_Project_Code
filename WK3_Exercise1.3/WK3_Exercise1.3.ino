#define ledPin_1 10
#define ledPin_2 11
#define ledPin_3 12
#define potentiometer A0

Exercise1.3: Light Dimmer
int potentiometerValue = 0;
int mappedPotentiometerValue = 0;

void setup() {
  pinMode(ledPin_1,OUTPUT);
  pinMode(ledPin_2,OUTPUT);
  pinMode(ledPin_3,OUTPUT);
  pinMode(potentiometer,INPUT);
  Serial.begin(9600); 

}

void loop() {
  potentiometerValue = analogRead(potentiometer);
  discreteMode();
  //continuousMode(); //remember to insert the LedPin_1 or 2 or 3. 
  delay(100);
  
  
}

void continuousMode (int ledNum) { //ledNum is the number of led you want to lit as define in the #define. (ledPin_1, ledPin_2, ledPin_3)
  mappedPotentiometerValue = map(potentiometerValue, 0, 1023, 0 , 255);
  analogWrite(ledNum, mappedPotentiometerValue);
}

void discreteMode () {
  static float voltageResolution = 0.0196; //static means only declare once 
  static float voltage = 0.0000;
  mappedPotentiometerValue = map(potentiometerValue, 0, 1023, 0 , 255);
  voltage = mappedPotentiometerValue * voltageResolution;
  Serial.println("Voltage : " + String(voltage));
  if (voltage >= 0 && voltage < 1.25 ) {
    digitalWrite(ledPin_1, LOW);
    digitalWrite(ledPin_2, LOW);
    digitalWrite(ledPin_3, LOW);
    }else if ( voltage >= 1.25 && voltage < 2.5) {
      digitalWrite(ledPin_1, HIGH);
      digitalWrite(ledPin_2, LOW);
      digitalWrite(ledPin_3, LOW);
    }else if (voltage >= 2.5 && voltage < 3.75) {
      digitalWrite(ledPin_1, LOW);
      digitalWrite(ledPin_2, HIGH);
      digitalWrite(ledPin_3, LOW);
    }else if (voltage >= 3.75) {
      digitalWrite(ledPin_1, HIGH);
      digitalWrite(ledPin_2, HIGH);
      digitalWrite(ledPin_3, HIGH);
    }else {
      Serial.println("Error");
    }
    
  }
