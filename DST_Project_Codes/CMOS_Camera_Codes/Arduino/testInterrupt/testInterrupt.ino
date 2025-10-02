int pin = 2;
long long int lastTime = 0;
volatile int state = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT);
  lastTime = micros();
  attachInterrupt(digitalPinToInterrupt(pin), onchange, CHANGE);
  
}

void loop() {
  
  
}

void onchange() {
  
  noInterrupts();
  
  int state = digitalRead(pin);
  if(state == 0){
    long long int t = micros();
    Serial.println((double)t - lastTime);    
  }else{
    lastTime = micros();
  }
  /*
   * int state = digitalRead(pin);
  if(state == 1){
    long long int t = micros();
    Serial.println((double)t - lastTime);
    lastTime = t;
  }
   * 
   */
  interrupts();
  
  
  
}

