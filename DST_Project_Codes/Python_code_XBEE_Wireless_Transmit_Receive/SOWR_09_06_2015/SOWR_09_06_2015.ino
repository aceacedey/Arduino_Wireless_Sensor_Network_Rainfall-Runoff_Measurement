#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long

String datetime;
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}



#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 6
 

// ADC readings: 
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {25, 50, 100, 150, 175, 200, 225, 256};

// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
char *strVals[NUMDIRS] = {"W","NW","N","NE","E","SE","S","SW"};
byte dirOffset=0;
//=======================================================
//For Humidity sensor
//=======================================================
unsigned long previousMillis = 0;


long interval =5000;

int wind_sensorValue=0,rain_sensorValue=0;
int wind_sum=0,rain_sum=0,rain_Value=0,rain_n=0,rain_day_prev = 0,wind_n=0,wind_temp=0,j=0,a, rain_m = 0, y, mo, d, h, m, s, m_now, m_old, rain_day, rain_15;

char buffer[6];
float humid, tempcel, wind_speed, tempf, tempk, f, wind_angle;
int WindDir = 0, angle = 0, wind_Value=0;

float raina = 0.0, rain_temp=0.0, raint = 0.0;

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {10,172,3,202}; // This is typically 10.0.0.x

EthernetServer server(80); // Port 80 is http.

void setup()
{
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
     DateTime now = RTC.now();
     m_old = now.minute();     

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());  

}


void loop()
{ 
  // listen for incoming clients
  
  EthernetClient client = server.available();
  if (client) 
  {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) 
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 15");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");

          client.println("<html>");

          client.println("<font face=Tahoma Size=5 color=red><h1 align=center>Weather Report using Arduino Ethernet Shield @ IIT-Kharagpur </font></h1>");

          client.println("<font face=Tahoma Size=4 color=Green><h2 align=center>LOCATION 2 : SCHOOL OF WATER RESOURCES </font></h2>");
          client.println("<font face=Tahoma Size=6 color=blue><div align=right > ");
          
          
         DateTime now = RTC.now();
         
          y = now.year();
          mo = now.month();
          d = now.day();
          h = now.hour();
          m = now.minute();
          s = now.second();
          client.println();
          client.print(y);
          client.print('/');
          client.print(mo);
          client.print('/');
          client.print(d);
          client.print('\t');
          client.print(h);
          client.print(':');
          client.print(m);
          client.print(':');
          client.print(s);
          // client.println(y,'/', mo, '/', d, ' ', h, ':', m, ':', s);
          client.println("</div></font>");
        /*  calcHumidity();
         
          client.println("<font face=verdana size=5 color=blue><P>HUMIDITY :");
          client.println(humid);
          client.println(" %   ");
          client.println("</font></P>");

          client.println("<font face=verdana size=5 color=blue><P>TEMPERATURE :");
          client.println(tempcel);
          client.println("C   ");
          client.println(tempf);
          client.println("F    ");
          client.println(tempk);
          client.println("K    "); 
          client.println("</font></P>");
                                        */
        rainfall();
             
 /*
          client.println("<font face=verdana size=5 color=blue><P> Current RAINFALL in Present Time :");
          client.println(rain_m * 0.254);
          client.println(" mm");
          client.println("</font></P>");
*/
          client.println("<font face=verdana size=5 color=blue><P> Total RAINFALL in past 5 mintutes :");
          client.println(rain_15 * 0.254);
          client.println(" mm");
          client.println("</font></P>");

          client.println("<font face=verdana size=5 color=blue><P> Total RAINFALL after 00:00:00 hours :");
          client.println(rain_day * 0.254);
          client.println(" mm");
          client.println("</font></P>");

          
          client.println("</html>");
          break;
        }
        
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void calcHumidity()
{
  Serial.println("\n");
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  humid = DHT11.humidity;
  Serial.println(humid);


  Serial.print("Temperature (oC): ");
  tempcel = DHT11.temperature;
  Serial.println(tempcel);

  Serial.print("Temperature (oF): ");
  tempf = Fahrenheit(DHT11.temperature);
  Serial.println(tempf);

  Serial.print("Temperature (K): ");
  tempk = Kelvin(DHT11.temperature);
  Serial.println(tempk);
}




void rainfall()
{
  rain_sensorValue = analogRead(A0);//Analog 0
 // Serial.println(rain_sensorValue);
  if(rain_sensorValue > 900)
  {
    rain_Value=1;
  }
  else
  {
    rain_Value=0;
  }
  if ((rain_n == 0)&&(rain_Value == 1))
  {
    rain_sum = rain_sum + 1;
  }
  rain_n=rain_Value;
  rain_m = rain_sum;
  Serial.println(rain_sum);
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
     
    DateTime now = RTC.now();
    int ht = now.hour();
    int mt = now.minute();
    int st = now.second();
       
    if (ht == 0 && mt == 0 )
    {
      rain_day = 0;
      rain_15 = 0;
      rain_sum = 0;
    }
    else
    {
      Serial.println("--------------time---------");
      Serial.println(mt);
      Serial.println(m_old);
      Serial.println("--------------time---------");
      if ( ((mt - m_old) <= 5 && (mt - m_old) >= 0   ) || ((mt - m_old) <= -55 ))
      {
        rain_15 = rain_sum;
       // rain_day_prev = rain_15;
       Serial.println(rain_15);
       Serial.println("--------------Rain_15---------");
     }
      else
      {
        m_old = now.minute();
        rain_day = rain_day + rain_15 ;
        Serial.println(rain_day);
        Serial.println("--------------Rain_Day---------");
        rain_15 = 0 ;
        rain_sum = 0;
        
      }
    }
  rain_m = 0; 
  } 
 else 
    rainfall();
  
}















void date_time()
{
        DateTime now = RTC.now();
         
          y = now.year();
          mo = now.month();
          d = now.day();
          h = now.hour();
          m = now.minute();
          s = now.second();
          String c1 = "/";
          String c2 = "   ";
          String c3 = ":";
         String datetime = d + c1 + mo + c1 + y + c2 + h + c3 + m + c3 + s;
     
          Serial.println("-------------------------------------------------");
          Serial.print(datetime);
      /*    client.print('/');
          client.print(mo);
          client.print('/');
          client.print(y);
          client.print('\t');
          client.print(h);
          client.print(':');
          client.print(m);
          client.print(':');
          client.print(s);
          */

}


