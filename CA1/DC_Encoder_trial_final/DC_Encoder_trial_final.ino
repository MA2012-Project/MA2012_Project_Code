//link: http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
//255 & 50 (1400 per rotation) , 255 & 165 (1450 per rotation)
#define motor_1 10 
#define motor_2 11 
#define encoder_A 2
#define encoder_B 3 

int enc_A =0;
int enc_B =0;
long volatile int encoder_count = 0;  
int motor_state_in = 0;
bool motor_state = true;

void setup() {
  Serial.begin(9600);
  pinMode( motor_1, OUTPUT);
  pinMode( motor_2, OUTPUT);
  pinMode( encoder_A, INPUT);
  pinMode( encoder_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_A), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_B), encoderCount, CHANGE);

}

void loop() {
  encoderCount();
  Serial.println(encoder_count);
  if (motor_state_in == 0) {
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
  }
  if (motor_state_in ==-2 && encoder_count <=-1100) {
    analogWrite(motor_1, HIGH);
    analogWrite(motor_2, HIGH);
    
    delay(500);
    analogWrite(motor_1, 180);
    analogWrite(motor_2, 255);
    encoder_count = 0;  
    motor_state_in =-1;  
    
  }
  if (motor_state_in ==-1 && encoder_count <=-1425) {
    analogWrite(motor_1, HIGH);
    analogWrite(motor_2, HIGH);
    motor_state = false;
    //break; only applicable for the whole code
  }

}
void encoderCount() {
  static const int lookout_table[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
  static int encoder_val = 0; 
  enc_A = digitalRead(encoder_A);
  enc_B = digitalRead(encoder_B);  
  encoder_val = encoder_val << 2;
  encoder_val = encoder_val | (PINB & 0b1100) >> 2; 
  encoder_count = encoder_count + lookout_table[encoder_val & 0b1111];
}
