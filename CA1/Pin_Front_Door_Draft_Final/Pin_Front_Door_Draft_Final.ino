//GLOBAL VARIABLE
int doorMode = 0; // NORMAL(0), LOCKED (1)

//DIGITAL SENSOR DEFINE
#define sensorPin A4 //digital

//DIGITAL SENSOR VARIABLE
int sensorState = 1; //This is because 1 is nothing

//DC MOTOR DEFINE
#define motor_1 10 //PWM
#define motor_2 11 //PWM
#define encoder_A 2 //digital
#define encoder_B 3 //digital

//DC MOTOR VARIABLE 
int enc_A = 0;
int enc_B = 0;
long volatile int encoder_count = 0;  
int motor_state = 0;

//KEYBOARD DEFINE 
#include <LiquidCrystal.h>
#define NO_OF_CHAR_1 3 
#define KA A3 //digital
#define KB A2 //digital //duplicate careful
#define KC A1 //digital 
#define KD A0 //digital
#define KAvail A5 //fixed don't touch 
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 13, d7 = 12; //digital //duplicate becareful
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

//KEYBOARD VARIABLE
char slidingChar[NO_OF_CHAR_1];
char keyboardChar[] = {'1','2','3','F',
                       '4','5','6','E',
                       '7','8','9','D',
                       'A','0','B','C'};
                 
void setup() {
  
  Serial.begin(9600);
  
  //FOR DC MOTOR
  /*pinMode( motor_1, OUTPUT);
  pinMode( motor_2, OUTPUT);
  pinMode( encoder_A, INPUT);
  pinMode( encoder_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_A), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_B), encoderCount, CHANGE);*/

  //FOR DIGITAL SENSOR
  //pinMode(sensorPin, INPUT);

  //FOR KEYBOARD
  // initializing the list 
  for (int i = 0; i < NO_OF_CHAR_1; i++)  slidingChar[i] = ' ';  // initialize by clearning memory
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);
  // initializing the lcd 
  lcd.begin(16,2);
  
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
      //check if it detects DD
      if (slidingChar[NO_OF_CHAR_1 -1] == 'D' && slidingChar[NO_OF_CHAR_1 -2] == 'D'){
        while(1){ //make sure that it stays in locked mode
          lcd.setCursor(0,0);
          lcd.print("LOCKED");
          if (digitalRead(KAvail)) readingInput();
          lcd.setCursor(0,1);
          lcd.print(slidingChar);
          Serial.println("LOCKED");
          delay(150);   // for trackpad sensitivity. Suggested (100-300)
          
            //check if it detects EE
            if (slidingChar[NO_OF_CHAR_1 -1] == 'E' && slidingChar[NO_OF_CHAR_1 -2] == 'E'){
              while(1){ //make sure that it stays in pin mode
                if (digitalRead(KAvail)) readingInput();
                lcd.setCursor(0,1);  
                lcd.print("PIN: ");
                lcd.setCursor(5,1);
                lcd.print(slidingChar);
                delay(150); // for trackpad sensitivity. Suggested (100-300)
                Serial.println("PIN");
                
                if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){
                  break; //break from PIN
                }
                else if (slidingChar[NO_OF_CHAR_1 -1] == 'F' && slidingChar[NO_OF_CHAR_1 -2] == 'F'){ //return back to locked mode
                  lcd.clear();
                  for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' '; // clear memory 
                  break;
                }
                else if (slidingChar[0] != ' ') {
                  delay(500); //Slow down so you can view your number for short while before deleting it 
                  for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' '; // clear memory 
                }
              } //end of while(1)
              if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){//double break from LOCKED
                 for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' '; //clear memory
                 lcd.clear();
                 break;
              }
            }
        }
      }
    }
    //Start
    /*sensorState = digitalRead(sensorPin); 
    encoderCount();
    Serial.println(encoder_count);
    if (sensorState ==0){
      while(1){
        Serial.println(encoder_count);
        if (motor_state ==0) {
          analogWrite(motor_1, 100);
          analogWrite(motor_2, 255);
        }
        if (encoder_count >= 2500){
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          delay(4000);
          
          analogWrite(motor_1, 255);
          analogWrite(motor_2, 100);
          
          motor_state = -2;
          encoder_count = 0;
          //Serial.println(motor_state);
        }
        if (motor_state ==-2 && encoder_count <=-1075) {
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          
          delay(500);
          analogWrite(motor_1, 255);
          analogWrite(motor_2, 180);
          encoder_count = 0;  
          motor_state = -1;  
          
        }
        if (motor_state ==-1 && encoder_count <=-1375) {
          analogWrite(motor_1, HIGH);
          analogWrite(motor_2, HIGH);
          delay(500);
          motor_state = 0;
          encoder_count = 0;
          break;
        } 
      }
    }*/

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
  slidingChar[0] = slidingChar[1];
  slidingChar[1] = slidingChar[2];
  slidingChar[2] = keyboardChar[k];
}
