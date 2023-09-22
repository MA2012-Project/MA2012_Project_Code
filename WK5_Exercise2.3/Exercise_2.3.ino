#include <LiquidCrystal.h> 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pins for 74C922 keypad encoder
#define KB_DataAvailable A5//Data ready pin from Keyboard
#define KB_A 10
#define KB_B 11
#define KB_C 12
#define KB_D 13

// Declare keypad layout
char keys[] = {'1','2','3','F',
               '4','5','6','E',
               '7','8','9','D',
               'A','0','B','C'};

#define NO_OF_CHAR 6
char virt_1[NO_OF_CHAR+1]; //means got 7 spaces for the list 
char virt_2[NO_OF_CHAR+1];
volatile int calcStatus =0;
int intVirt_1 = 0.00;

void setup() {
  for (int i=0;i<NO_OF_CHAR;i++){
    virt_1[i]=' ';
    virt_2[i]=' ';
  }
  virt_1[NO_OF_CHAR] =0;
  virt_2[NO_OF_CHAR] =0;
  pinMode(KB_DataAvailable, INPUT);
  pinMode(KB_A,INPUT);
  pinMode(KB_B,INPUT);
  pinMode(KB_C,INPUT);
  pinMode(KB_D,INPUT);
  
  lcd.begin(16,2);
  lcd.print("Hello World!");
}

void loop() {
 //check if the keypad is hit
    if(digitalRead(KB_DataAvailable)) {
      //initCalc();
    }
      if (calcStatus == 1){ //means calc is online 
        while(1){
          KB_Read();
          if ((virt_1[NO_OF_CHAR -1]) == "+") {
            addition();
            for (int i=0;i<NO_OF_CHAR;i++){
              virt_1[i]=' ';
            }
          }
          else if ((virt_1[NO_OF_CHAR -1] == "-")) {
            subtraction();
            for (int i=0;i<NO_OF_CHAR;i++){
              virt_1[i]=' ';
            }
          }    
          else if ((virt_1[NO_OF_CHAR -1] == "/")){
            devision();
            for (int i=0;i<NO_OF_CHAR;i++){
              virt_1[i]=' ';
            }
          }  
           else if ((virt_1[NO_OF_CHAR -1] == "*")){
            multiplication();
            for (int i=0;i<NO_OF_CHAR;i++){
              virt_1[i]=' ';
            }
           }  
          else if ((virt_1[NO_OF_CHAR -1] == "=")){
            equal();  
          }  
          else if ((virt_1[NO_OF_CHAR -1]) == "A" && (virt_1[NO_OF_CHAR - 1]) == "A") { //To clear the data to 0 
            intVirt_1 = 0.00;
            for (int i=0;i<NO_OF_CHAR;i++){
              virt_1[i]=' ';
              virt_2[i]=' ';
            }
          }  
          else {
            continue;
          }
        }
      }
      delay(1000);
}
    

void KB_Read(){
  int ka,kb,kc,kd,i;
  char k;
  ka=digitalRead(KB_A); //read encoder output A
  kb=digitalRead(KB_B); //read encoder output B
  kc=digitalRead(KB_C); //read encoder output C
  kd=digitalRead(KB_D); //read encoder output D

  k=ka+kb*2+kc*4+kd*8; // combine the encoder outputs 

  for(i=0;i<(NO_OF_CHAR-1);i++) virt_1[i]=virt_1[i+1];//move displayed characters in FIFO queue forward discarding the first one
  virt_1[NO_OF_CHAR-1]=keys[k]; // update the key into the queue
  virt_1[NO_OF_CHAR]=0; // end with NULL
}

void initCalc(){ //checks for 'F' 'F'
    if (virt_1[NO_OF_CHAR -2] == 'F' && virt_1[NO_OF_CHAR -1] == 'F'){
      calcStatus = 1;
    }
}
void addition() {
    volatile double setPos =0.00;
    for (int i=0; i<NO_OF_CHAR; i++){
      if ((virt_1[i] - 48) >=0 && (virt_1[i] - 48) <58){
       setPos *= 10;
       setPos = (setPos + (virt_1[i] -48));
    }
     intVirt_1 += setPos;
  }
}

void subtraction() {
    volatile double setPos =0.00;
    for (int i=0; i<NO_OF_CHAR; i++){
      if ((virt_1[i] - 48) >=0 && (virt_1[i] - 48) <58){
       setPos *= 10;
       setPos = (setPos + (virt_1[i] -48));
    }
     intVirt_1 -= setPos;
  }
}

void devision() {
  volatile double setPos= 0.00;
  for (int i =0; i<NO_OF_CHAR; i++){
    if ((virt_1[i] -48) >= 0 && (virt_1[i] < 58)){
      setPos *= 10;
      setPos = (setPos + (virt_1[i]-48));
    }
  }
  intVirt_1 /= setPos;
}

void multiplication() {
  volatile double setPos= 0.00;
  for (int i =0; i<NO_OF_CHAR; i++){
    if ((virt_1[i] -48) >= 0 && (virt_1[i] < 58)){
      setPos *= 10;
      setPos = (setPos + (virt_1[i]-48));
    }
  }
  intVirt_1 *= setPos;
}

void equal() {
  itoa(intVirt_1, virt_2, 10); // Convert integer to string in base 10
  lcd.setCursor(0, 1);    
  lcd.print(virt_2);
}
