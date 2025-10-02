#include <SPI.h>
#include<string.h>
#define uint  unsigned int
#define ulong unsigned long
#define PIN_VANE        A0
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {26, 45, 77, 118, 161, 177, 197, 256};
//unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
long interval = 10000;
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;
char buffer[6];
byte WindDir;
int angle = 0, i = 0;  
float f ;
int n = 0;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
    WindDir = 0;
//    previousMillis = 0;
    
    windDirection();
   // delay(10000);
}

void windDirection()
{
  Serial.println("-----------------i-------------------------");
  int val;
  byte x, reading;

  val = analogRead(PIN_VANE);
  Serial.print("Inital Value : ");
  Serial.println(val);
  val >>=2;                        // Shift to 255 range
  Serial.println(val);
  reading = val;

  // Look the reading up in directions table. Find the first value
  // that's >= to what we got.
  for (x=0; x<NUMDIRS; x++)
  {

    if (adc[x] >= reading)

      break;
  }
    Serial.println(reading, DEC);
    Serial.print("x 1st ");
    Serial.println(x);
  x = (x + dirOffset) % 8;   // Adjust for orientation
    Serial.print("x 2nd ");
    Serial.println(x);
    
  switch(x) 
  {
  case 0:
    angle = 0 ;
    break;
  case 1:
    angle = 1 ;
    break;
  case 2:
    angle = 2 ;
    break;
  case 4:
    angle = 3 ;
    break;
  case 7:
    angle = 4 ;
    break;
  case 6:
    angle = 5 ;
    break;
  case 5:
    angle = 6 ;
    break;
  case 3:
    angle = 7 ;
    break;
  }  
  Serial.print("Dir: ");
  Serial.println(strVals[angle]);
  Serial.println(angle);
  //   WindDir[0] = strVals[0];
  unsigned long currentMillis = millis();
  Serial.print("Current : ");
  Serial.println(currentMillis);
  
  WindDir = angle + WindDir;
  if(currentMillis - previousMillis > interval) 
  {
  previousMillis = currentMillis;
  Serial.print("Previous : ");
  Serial.println(previousMillis);
 
  Serial.println("-----------------f-------------------------");  
  Serial.print("Final : ");
  Serial.println(WindDir);
  f = (float)WindDir / n;
  f = f * 45;
  Serial.println(f); 
  n=0;
 }  
   
   else
      
      { 
        Serial.print("Previous : ");
        Serial.println(previousMillis);
        n++;
        Serial.print("value of n = ");
        Serial.println(n);
        windDirection();
       }
    
  
}

