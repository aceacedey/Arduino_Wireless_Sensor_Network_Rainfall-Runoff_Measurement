int p = 0, r = 0, i = 0, j = 0, k = 0, value = 0;
int data[10];
char c;
unsigned int integerValue = 0;



char incomingByte;
int sensorReading = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{

  if (Serial.available() > 0)
  { // something came across serial
     i = 0;                                                                         // throw away previous integerValue
    while (1)
    { // force into a loop until 'n' is received
      incomingByte = Serial.read();
      //      Serial.print("----the incoming byte is : ");
      //      Serial.println(incomingByte);
      if (incomingByte == '\n')
        break;                                                                       // exit the while(1), we're done receiving
      if (incomingByte == -1)
        continue;                                                        // if no characters are in the buffer read() returns -1
      // shift left 1 decimal place
      data[i] = incomingByte;
      i = i + 1 ;
    }
    p = (data[2] - 48) + 10 * (data[1] - 48) + 100 * (data[0] - 48);

    sensorReading = p;
   pr = 0.00;
  }
  Serial.println(sensorReading);
  delay(1000);

}
