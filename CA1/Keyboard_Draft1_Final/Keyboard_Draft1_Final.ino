#include <LiquidCrystal.h>
#define NO_OF_CHAR_1 3 
#define KA A3
#define KB A2
#define KC A1
#define KD A0
#define KAvail A5


const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 13, d7 = 12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

char slidingChar[NO_OF_CHAR_1];
char keyboardChar[] = {'1','2','3','F',
                       '4','5','6','E',
                       '7','8','9','D',
                       'A','0','B','C'};
                
                 
void setup() {
  // initializing the list 
  for (int i = 0; i < NO_OF_CHAR_1; i++)  slidingChar[i] = ' ';  // initialize by clearning memory
  
  
  Serial.begin(9600);
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);
  
  // initializing the lcd 
  lcd.begin(16,2);
}
 
void loop() {
  lcd.setCursor(0,0);
  lcd.print("NORMAL");
  if (digitalRead(KAvail)){
    Serial.println("yea");
    readingInput();
    lcd.setCursor(0,1);
    lcd.print(slidingChar);  
    delay(100);
  }
  if (slidingChar[NO_OF_CHAR_1 -1] == 'D' && slidingChar[NO_OF_CHAR_1 -2] == 'D'){
    while(1){
      lcd.setCursor(0,0);
      lcd.print("LOCKED");
      if (digitalRead(KAvail)) readingInput();
      lcd.setCursor(0,1);
      lcd.print(slidingChar);
      Serial.println("LOCKED");
      delay(100);   // for trackpad sensitivity. Suggested (100-300)
      
      if (slidingChar[NO_OF_CHAR_1 -1] == 'E' && slidingChar[NO_OF_CHAR_1 -2] == 'E'){
        while(1){
          if (digitalRead(KAvail)) readingInput();
          lcd.setCursor(0,1);  
          lcd.print("PIN: ");
          lcd.setCursor(5,1);
          lcd.print(slidingChar);
          delay(100); // for trackpad sensitivity. Suggested (100-300)
          Serial.println("PIN");
          
          if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){
            break; //break from PIN
          }
          else if (slidingChar[NO_OF_CHAR_1 -1] == 'F' && slidingChar[NO_OF_CHAR_1 -2] == 'F'){
            lcd.clear();
            for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' ';
            break;
          }
          else if (slidingChar[0] != ' ') {
            delay(500); //Slow down so you can view your number for short while before deleting it 
            for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' '; // clear memory 
          }
        } //end of while(1)
        if (slidingChar[0] == '1' && slidingChar[1] == '2' && slidingChar[2] == '3'){//dobule break from LOCKED
           for (int i = 0; i < NO_OF_CHAR_1; i++)slidingChar[i] = ' '; //clear memory
           lcd.clear();
           break;
        }
      }
    }
  }
}

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
    
  
