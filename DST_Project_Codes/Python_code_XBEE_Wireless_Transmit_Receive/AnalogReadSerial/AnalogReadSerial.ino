int p = 0, r = 0; 

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
}

// the loop routine runs over and over again forever:
void loop() 
{
  
  // read the input on analog pin 0:
  int p = analogRead(A0);
  int r = analogRead(A3);
  
  Serial.print(p);
  Serial.print('\t');
  Serial.print(r);
 // Serial.println('r');
  Serial.print('\n');
  delay(1000);   
  
  // delay in between reads for stability
}
