#include <SD.h>
#include "RTClib.h"
#include <Wire.h>
#include <string.h>
RTC_DS1307 RTC;
char filename[] = "00000000.csv";
File myFile;
unsigned long previousMillis = 0;
long interval = 15000;
int sensorValue =0 ;
int sum=0;
int n=0;
int Value = 0;
void setup()
{
Serial.begin(9600);
Wire.begin(); //Important for RTClib.h
RTC.begin();

if (! RTC.isrunning()) {

Serial.println("RTC is NOT running!");
return;
}

Serial.print("Initializing SD card...");

pinMode(10, OUTPUT);

if (!SD.begin(10)) {
Serial.println("initialization failed!");
return;
}
Serial.println("initialization done.");

}

void loop()
{
sensorValue = analogRead(A0);//Analog 0
if(sensorValue > 1000)
{
  Value=1;
}
else
{
  Value=0;
}
if ((n == 0)&&(Value == 1))
{
  sum = sum + 1;
 }
  n=Value;
getFileName();
createFileName();
rain();
}
void getFileName(){

DateTime now = RTC.now();

filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()

}

void createFileName(){
myFile = SD.open(filename, FILE_WRITE);
myFile.close();
}
void rain(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
     previousMillis = currentMillis;
  DateTime now = RTC.now();
     myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
// myFile  .print(now.hour(), DEC); 
 //myFile.print(':');
// myFile .print(now.minute(), DEC);  
// myFile  .print(':');
// myFile.print(now.second(), DEC); 
// myFile  .print(',');
 myFile  .print(now.hour(), DEC);
 myFile.print(',');
 myFile .print(now.minute(), DEC);  
 myFile  .print(',');
 myFile.print(now.second(), DEC); 
 myFile  .print(','); 
 myFile.println(sum);
 sum = 0;
// myFile .println();
 myFile.close();
      }  
   }
}
 
  


 

