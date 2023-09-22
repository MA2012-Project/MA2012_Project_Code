//Exercise 1.1:Cutting Machine
//h
//another
#define pushButtonPin 2
#define toggleSwitchPin 3
#define limitSwitch 4
#define piezoBuzzer 8
#define solenoidPin 10
#define ledPin 13 

volatile int toggleSwitchPinState = 0;


void setup() {
  Serial.begin(9600);
  pinMode(pushButtonPin,INPUT);
  pinMode(toggleSwitchPin,INPUT);
  pinMode(limitSwitch,INPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(ledPin,OUTPUT);
  
  
  
}

void loop() {
  
  toggleSwitchPinState = digitalRead(toggleSwitchPin);
  if (toggleSwitchPinState == HIGH){
    digitalWrite(ledPin,HIGH);
    Serial.println("System is online");
    system_on();
  }else {
    digitalWrite(ledPin, LOW);
    while(1){
    Serial.println("Systems is offline");
    toggleSwitchPinState = digitalRead(toggleSwitchPin);
    if (toggleSwitchPinState == HIGH){
      break;
    }
    delay(1000);
    }
  }
  delay(1000);
}




void system_on(){
  static volatile int limitSwitchState =0;
  static volatile int pushButtonPinState =0;
  
  limitSwitchState = digitalRead(limitSwitch); //If depress --> 1, If open --> 0
  if (limitSwitchState == HIGH){
    pushButtonPinState = digitalRead(pushButtonPin);
    if (pushButtonPinState == LOW){ //If push --> 0, If no push --> 1
      while(1){
        static volatile int count = 0;
        digitalWrite(solenoidPin, HIGH);
        Serial.println("HIGH");
        delay(500);
        digitalWrite(solenoidPin,LOW);
        Serial.println("LOW");
        delay(500);
        count++;
        Serial.println(count);
        limitSwitchState = digitalRead(limitSwitch);
        if (count > 9){
          count = 0;
          break;
        }
        else if (limitSwitchState == LOW){
          for ( int p = 0; p <3; p++){
          tone(8,200,200);
          delay(333);
          }
          count =0;
          break;
        }
      }
    }
    return;
  }else{
    return;
  }
  }
  
