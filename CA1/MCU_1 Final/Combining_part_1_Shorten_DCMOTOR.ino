//GLOBAL VARIABLE
bool messageStatus = false; // NOT SENT(FALSE), SENT(TRUE)

//DIGITAL SENSOR DEFINE
#define sensorPin A4 //digital

//DIGITAL SENSOR VARIABLE
int sensorState = 1; //1 means it detect nothing

//DC MOTOR DEFINE
#define motor_1 10 //PWM
#define motor_2 11 //PWM
#define encoder_A 2 //digital
#define encoder_B 3 //digital

//DC MOTOR VARIABLE 
int enc_A = 0;
int enc_B = 0;
long volatile int encoder_count = 0; 
int motor_state_in =0; 

//KEYBOARD DEFINE 
#include <LiquidCrystal.h>
#define NO_OF_CHAR 3 //last one must be 0 or ' '  
#define KA A3 //digital
#define KB A2 //digital 
#define KC A1 //digital 
#define KD A0 //digital
#define KAvail A5 //fixed don't touch 
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 13, d7 = 12; // digital
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

//KEYBOARD VARIABLE
char slidingChar[NO_OF_CHAR+1];
char keyboardChar[] = {'1','2','3','F',
                       '4','5','6','E',
                       '7','8','9','D',
                       'A','0','B','C'};

//MCU1_COMMS DEFINE
#include <SoftwareSerial.h>
#define RxD 9
#define TxD 8

//MCU1_COMMS VARIABLE
char receivedChar;


//MCU1_COMMS OBJECT
SoftwareSerial master(RxD, TxD);

void setup() {
  
  Serial.begin(9600);
  
  //FOR DC MOTOR
  pinMode( motor_1, OUTPUT);
  pinMode( motor_2, OUTPUT);
  pinMode( encoder_A, INPUT);
  pinMode( encoder_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_A), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_B), encoderCount, CHANGE);

  //FOR DIGITAL SENSOR
  pinMode(sensorPin, INPUT);

  //FOR KEYBOARD
  // initializing the list 
  for (int i = 0; i < NO_OF_CHAR; i++)  slidingChar[i] = ' ';  // initialize by clearning memory
  slidingChar[NO_OF_CHAR] =0;
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);
  // initializing the lcd 
  lcd.begin(16,2);

  //MCU1_COMMS
   pinMode(RxD, INPUT);   //received data from slave to master 
   pinMode(TxD, OUTPUT);  //send data from master to slave 
   master.begin(9600); // Initialise the communication (Why do we need to set baud rate?) 
}

void loop() {
    Serial.println("NORMAL");
    lcd.setCursor(0,0);
    lcd.print("NORMAL");
    if (digitalRead(KAvail)){
      readingInput();
      lcd.setCursor(0,1);
      lcd.print(slidingChar);  
      delay(150);
    }  
      //check if it detects DD
      if (slidingChar[NO_OF_CHAR -1] == 'D' && slidingChar[NO_OF_CHAR -2] == 'D'){
        while(1){ //make sure that it stays in locked mode
          //MCU1_COMMS locked mode 
          while (messageStatus != true){
            Serial.println("LOCKED SENT");
            master.write("LOCKED");
            delay(1000); // time for slave to react since we are using while loop, minimum 1 second (empherical experiment) 
            if (master.available()>0) {
              messageStatus = master.read(); //THE SLAVE SHOULD SEND THIS
              Serial.println(messageStatus);
            }
          }
          //END MCU1_COMMS
          lcd.setCursor(0,0);
          lcd.print("LOCKED");
          if (digitalRead(KAvail)) readingInput();
          //lcd.setCursor(0,1);
          //lcd.print(slidingChar);
          Serial.println("LOCKED");
          delay(150);   // for trackpad sensitivity. Suggested (100-300)
          
          if(slidingChar[NO_OF_CHAR -1] != 'E' && slidingChar[NO_OF_CHAR -1] != 'E' ) {
              lcd.clear();
              for (int i = 0; i < NO_OF_CHAR; i++)slidingChar[i] = ' '; // clear memory 
              Serial.println("HELP ME DUDE");
          }
          
          //check if it detects EE
          if (slidingChar[NO_OF_CHAR -1] == 'E' && slidingChar[NO_OF_CHAR -2] == 'E' || slidingChar[NO_OF_CHAR -2] == 'E' && slidingChar[NO_OF_CHAR -3] == 'E' ){
            while(1){ //make sure that it stays in pin mode
              if (digitalRead(KAvail)) readingInput();
              lcd.setCursor(0,1);  
              lcd.print("PIN: ");
              lcd.setCursor(6,1);
              lcd.print(slidingChar);
              delay(150); // for trackpad sensitivity. Suggested (100-300)
              Serial.println("PIN");
              
              if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){
                break; //break from PIN
              }
              else if (slidingChar[NO_OF_CHAR -1] == 'F' && slidingChar[NO_OF_CHAR -2] == 'F'){ //return back to locked mode
                lcd.clear();
                for (int i = 0; i < NO_OF_CHAR; i++)slidingChar[i] = ' '; // clear memory 
                break;
              }
              else if (slidingChar[0] != ' ') {
                delay(500); //Slow down so you can view your number for short while before deleting it 
                for (int i = 0; i < NO_OF_CHAR; i++)slidingChar[i] = ' '; // clear memory 
              }
            } //end of while(1)
            if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){//double break from LOCKED
               for (int i = 0; i < NO_OF_CHAR; i++)slidingChar[i] = ' '; //clear memory
               messageStatus = "false";
               lcd.clear();
               //MCU1_COMMS
               while (1){
                master.write("NORMAL");
                delay(1000); //delay for slave to react since we using while 
                if (master.available()>0) {
                  messageStatus = master.read(); //THE SLAVE SHOULD SEND THIS //TRUE
                  Serial.println(messageStatus);
                  if (messageStatus == true){                   
                    break;
                  }
                }
               } 
               if (messageStatus == true) {
                messageStatus = false;
                break;
               }//END MCU1_COMMS 
            }
          }
        }
      }
    //DC MOTOR
    sensorState = digitalRead(sensorPin); 
    encoderCount();
    Serial.println(sensorState);
    if (sensorState ==0){
      while(1){
        Serial.println("LOL");
        if (motor_state_in ==0) {
          analogWrite(motor_1, 255);
          analogWrite(motor_2, 100);
        }
        if (encoder_count >= 2500){
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          delay(4000);
          
          analogWrite(motor_1, 100);
          analogWrite(motor_2, 255);
          
          motor_state_in = -2;
          encoder_count = 0;
          //Serial.println(motor_state);
        }
        if (motor_state_in ==-2 && encoder_count <=-1100) {
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          
          delay(500);
          analogWrite(motor_1, 180);
          analogWrite(motor_2, 255);
          encoder_count = 0;  
          motor_state_in = -1;
        }  
        if (motor_state_in ==-1 && encoder_count <=-1425) {
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          delay(500);
          motor_state_in = 0;
          encoder_count = 0;
          break;
        } 
      }
    }
}    

//FOR DC MOTOR
void encoderCount(){ //Jackson, this one cfm correct, don't need to touch it
  static const int lookout_table[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; //truth table for direction 
  static int encoder_val = 0; 
  enc_A = digitalRead(encoder_A);
  enc_B = digitalRead(encoder_B);  
  encoder_val = encoder_val << 2;
  encoder_val = encoder_val | (PIND & 0b1100) >> 2; 
  encoder_count = encoder_count + lookout_table[encoder_val & 0b1111];
}

//FOR KEYBOARD
void readingInput(){
  int ka = 0, kb = 0, kc = 0, kd = 0, k=0;
  ka = digitalRead(KA);
  kb = digitalRead(KB);
  kc = digitalRead(KC);
  kd = digitalRead(KD);
  
  k = ka*1 + kb*2 + kc*4 + kd*8;
  for (int i =0; i <(NO_OF_CHAR -1); i++) slidingChar[i] = slidingChar[i+1];
  slidingChar[NO_OF_CHAR-1] = keyboardChar[k];
  slidingChar[NO_OF_CHAR] = 0; 
}
