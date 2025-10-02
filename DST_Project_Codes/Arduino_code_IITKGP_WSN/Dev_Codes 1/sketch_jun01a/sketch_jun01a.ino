// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
//#define SS_SD_CARD   4
//#define SS_ETHERNET 10
RTC_DS1307 RTC;
int h,m,s, m_now, m_old;
float rain_day, rain_15;
void setup () {
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
  // ...
   // pinMode(SS_SD_CARD, OUTPUT);
   // pinMode(SS_ETHERNET, OUTPUT);
   // digitalWrite(SS_SD_CARD, HIGH);  // SD Card not active
   // digitalWrite(SS_ETHERNET, HIGH); // Ethernet not active
    // ...
    //RTC_DS1307 RTC;
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
 
  if (! RTC.isrunning()) 
  {
  
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
}
 
void loop () 
{
 // RTC_DS1307 RTC;  
  DateTime now = RTC.now();
    h = now.hour();
    m = now.minute();
    m_old = now.minute();
    s = now.second();
    Serial.println(h);
    Serial.println(m);
    Serial.println(s);
      m_now = now.minute();
      
    
     
    delay(1000);
}



