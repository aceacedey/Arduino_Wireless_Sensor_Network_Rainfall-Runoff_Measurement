int a = 0, b = 0;
char serialData[16];
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  a = analogRead(A0);
    Serial.print(a) ;
    Serial.print('\n');
  
 
 
//  char c = Serial.read();
//  Serial.println(c);

  delay(1000);
 Serial.flush();
}

