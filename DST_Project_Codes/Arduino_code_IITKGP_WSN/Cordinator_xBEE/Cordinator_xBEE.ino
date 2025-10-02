void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;
int sensorReading;
void loop() {
    if (Serial.available() > 0) 
  {   // something came across serial
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      Serial.print("-----------------------the in coming byte is : ");
      Serial.println(incomingByte);
      if (incomingByte == '\n') 
            break;   // exit the while(1), we're done receiving
      else    
          {
            if (incomingByte == -1) 
            {
                 continue;  // if no characters are in the buffer read() returns -1
                 integerValue *= 10;  // shift left 1 decimal place
                 // convert ASCII to integer, add, and shift left 1 decimal place
                 integerValue = ((incomingByte - 48) + integerValue);
            }
            else    
                  break;
          }
}
    Serial.println(integerValue);   // Do something with the value
  }
 sensorReading = integerValue;
Serial.println(sensorReading);
delay(200);
}
