#include <SPI.h>
#include<string.h>
#define uint  unsigned int
#define ulong unsigned long
#define PIN_VANE        A1
#define NUMDIRS 8
ulong   adc[NUMDIRS] = { 25, 50, 100, 150, 175, 200, 225, 256};

char *strVals[NUMDIRS] = { 
  "W","NW","N","NE","E","SE","S","SW"};
byte dirOffset=0;
char buffer[6];
byte WindDir;
int angle = 0, i = 0;  
float f ;
int val;
byte x, reading;
void setup()
{
  Serial.begin(9600);
}

void loop()
{

//  WindDir = 0;
//  for (int wind_n = 0 ; wind_n < 10 ; wind_n++)
//  {
    

    val = analogRead(A1);
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
    //Serial.println(reading, DEC);
 //   Serial.print("x 1st ");
 //   Serial.println(x);
    x = (x + dirOffset) % 8;   // Adjust for orientation
    Serial.print("x 2nd ");
    Serial.println(x);

//    Serial.print("Dir: ");
//    Serial.println(strVals[x]);
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

   Serial.print("angle int === ");
    Serial.println(angle);
    Serial.println(strVals[angle]);
//    WindDir = angle + WindDir;
//    Serial.println(WindDir);  
//    delay(5000);
//  }
//  Serial.print("Final : ");
//  Serial.println(WindDir);
  f = angle ;
  f = f * 45;
  Serial.println(f);
     delay(5000);
}


