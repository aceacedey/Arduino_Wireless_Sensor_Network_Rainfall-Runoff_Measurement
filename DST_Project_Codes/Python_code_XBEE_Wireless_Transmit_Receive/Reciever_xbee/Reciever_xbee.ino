

#include <SPI.h>
#include <Ethernet.h>
int p,r,i,j=0,value;
int data[10]; 
char c;
unsigned int integerValue=0;  

// Max value is 65535
char incomingByte;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}



void serialRead() 
{

  if (Serial.available() > 0) 
  {   // something came across serial
    Serial.println("------");
    i = 0;                                                                         // throw away previous integerValue
    while(1)
    {                                                                           // force into a loop until 'n' is received
      incomingByte = Serial.read();
      //      Serial.print("----the incoming byte is : ");
      //      Serial.println(incomingByte);
      if (incomingByte == '\n') 
        break;                                                                       // exit the while(1), we're done receiving
      if (incomingByte == -1)  
        continue;                                                        // if no characters are in the buffer read() returns -1
      // shift left 1 decimal place
      data[i] = incomingByte;
      Serial.print("Reading = ");
      Serial.println(data[i]);
      i = i +1 ;

    }
    
    j = 0;
    while(data[j] != 9)
    {
    
      j = j + 1;
    }
    
    p = (data[2] - 48) + 10 * (data[1] - 48) + 100 * (data[0] - 48);
    value = 0 ;
    Serial.println(j);
    Serial.println(i);
    while(j != (i-1))
    {
     value = value + (data[j + 1] - 48) * pow(10,(i-j-2));
     j = j + 1 ;
     } 
    r = value;
    
      Serial.println("---------------------------------------------------------------------------------------------------------------------------------------");
      Serial.println(p);
      Serial.println(r);
      // Do something with the value

  }
  // sensorReading = integerValue;

}


void loop() 
{
  serialRead();
  delay(2000); 
}


