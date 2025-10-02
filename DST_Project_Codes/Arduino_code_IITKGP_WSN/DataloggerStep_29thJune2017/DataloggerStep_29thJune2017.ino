#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;
File dataFile;
int now_day = 0;
int prev_day = 0; 
char filename[] = "00000000.CSV";

const int chipSelect = 4;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  //rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  DateTime now = rtc.now();
  
  now_day = int(now.day());  
  if (now_day != prev_day)
  {
    prev_day = now_day;
    getFilename(filename);  
  }
  else
  {
    dataFile = SD.open((filename), FILE_WRITE);
    if (dataFile)
    { 
      String dataString = "";
  
      int sensor = analogRead(A2);
      dataString =  String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + '\t' + String(sensor);
      Serial.println(dataString);
      dataFile.println(dataString);
      dataFile.close();// print to the serial port too:
      Serial.println(dataString);
    }
    else 
    {
      Serial.println("error opening datalog.txt");
    }
  }
  delay(60000);

}

void getFilename(char *filename) {
   DateTime now = rtc.now(); 

   filename[0] = int(now.year())/10 + '0';        // im getting error here so for now force it to 1.
   filename[1] = int(now.year())%10 + '0';
   filename[2] = int(now.month())/10 + '0'; 
   filename[3] = int(now.month())%10 + '0';
   filename[4] = int(now.day())/10 + '0';
   filename[5] = int(now.day())%10 + '0';
   filename[6] = int(now.hour())/10 + '0';
   filename[7] = int(now.hour())%10 + '0';
   filename[8] = '.';
   filename[9] = 'C';
   filename[10] = 'S';
   filename[11] = 'V';
   return;
 }









