#define ultrasound_trig 12//digital
#define ultrasound_echo 11//digital

float duration =0.00;
int distance = 0;

void setup() {
  pinMode(ultrasound_trig,OUTPUT);
  pinMode(ultrasound_echo, INPUT);

}

void loop() {
  digitalWrite(ultrasound_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasound_trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasound_trig, LOW);

  duration = pulseIn(ultrasound_echo, HIGH);
  distance = 0.034 * duration /2;

}
