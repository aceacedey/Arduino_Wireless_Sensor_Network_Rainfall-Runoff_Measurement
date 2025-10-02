
#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;



#define uint unsigned int
#define ulong unsigned long
#include <SD.h>
const int chipSelect = 4;


#include <string.h>

char filename[] = "00000000.csv";
//unsigned long previousMillis_data
#include <Ethernet.h>
#include <SPI.h>
#include <string.h>

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
 
unsigned long previousMillis = 0;

int rain_Value=0, rain_sum=0, rain_sensorValue=0, rain_n=0;
float humid, tempcel, tempf, tempk;
long interval = 15000;
float raina = 0.0;

byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xC7};
// assign an IP address for the controller:
IPAddress ip(10,172,3,201);
IPAddress gateway(10,172,3,2);	
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(80);

void setup() 
{
 
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  server.begin();
  // initialize serial communication at 9600 bits per second:

  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
   rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  
   Serial.print("Initializing SD card...");
   // make sure that the default chip select pin is set to
   // output, even if you don't use it:
   pinMode(10, OUTPUT);
   // see if the card is present and can be initialized:
   if (!SD.begin(chipSelect)) 
   {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
   }
   Serial.println("card initialized.");

}

// the loop routine runs over and over again forever:

void loop() 
{  
 
  listenForEthernetClients();
          // delay in between reads for stability
}



void listenForEthernetClients() 
{
  rain_sum = 0; 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 15");  // refresh the page automatically every 5 sec
          client.println();
          // print the current readings, in HTML format:
          client.println("<!DOCTYPE HTML>");

          client.println("<html>");

           
          client.println("<font face=Tahoma Size=5 color=blue><h1 align=center>Weather Report using Arduino Ethernet Shield @ IIT-Kharagpur </font></h1>");

          client.println("<font face=Tahoma Size=5 color=Red><h2 align=center>LOCATION 3 : SCHOOL OF WATER RESOURCES </font></h2>");
      
          client.println("<font face=verdana size=5 color=Green><P> Counting Since >>> 1st September, 2014");
          rainfall();
          
          client.println("</font></P>");
          
          client.println("<font face=verdana size=5 color=red><P> TOTAL RAIN TIP COUNT = ");
          client.println(rain_sum);
          
          client.println("</font></P>");
          client.println("<font face=verdana size=5 color=red><P>TOTAL RAINFALL:");
          client.println(raina);
          client.println(" mm");
          client.println("</font></P>");

          client.println("</html>");
          break;  
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

void rainfall()
{
  rain_sensorValue = analogRead(A1);//Analog 0
  if(rain_sensorValue > 512)
  {
    rain_Value=1;
  }
  else
  {
    rain_Value=0;
  }
      rain_sum = rain_sum + rain_Value;
  
//    rain_n=rain_Value;
 

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
    Serial.print(rain_sum);
    Serial.print(",");
    raina = ((float)rain_sum * 0.254);
    Serial.println(raina);
   } 
  else 
    rainfall();

}



