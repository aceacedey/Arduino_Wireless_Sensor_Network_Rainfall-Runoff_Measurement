
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long

#define PIN_ANEMOMETER  A0     
#define PIN_VANE        A5     // Analog 5
#include <DHT22.h>
// Only used for sprintf
#include <stdio.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 6

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);

long wind_interval = 5000;
int sensorwind_value =0 ;
int wind_sum=0;
int wind_n=0;
int wind_value = 0;

// ADC readings:
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};

// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;
//=======================================================
//For Humidity sensor
//=======================================================
unsigned long previousMillis = 0;
unsigned long previoushumidMillis = 0;

long interval =60000;
long intervalhumid=60000;
int sensorValue=0;
int sum=0,j=0,a;
int n=0;
char buffer[6];
float humid, tempcel, wind_Speed;

byte WindDir;

int Raina;

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xC7};
byte ip[] = {
  10,172,3,200}; // This is typically 10.0.0.x

EthernetServer server(80); // Port 80 is http.

void setup()
{
  Serial.begin(9600); 
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

          client.println("<font face=Tahoma Size=5 color=blue><h1 align=center>Weather Report using Arduino Ethernet Shield @ IIT-Kharagpur </font></h1>");
          calcHumidity();
          client.println("<font face=verdana size=5 color=RED><P>HUMIDITY :");
          client.println(humid);
          client.println("</font></P>");


          client.println("<font face=verdana size=5 color=RED><P>TEMPERATURE :");
          client.println(tempcel);
          client.println("C   ");
          /*             client.println(tempfar);
           client.println("F    ");
           client.println(tempkel);
           client.println("K    "); */
          client.println("</font></P>");

          calcWindDir();
          client.println("<font face=verdana size=5 color=red><P>");
          client.print("WIND DIRECTION : ");
          client.print(strVals[WindDir]);
          client.println("</font></P>");


          calcWindSpeed();
          client.println("<font face=verdana size=5 color=red><P> WINDSPEED:");
          client.println(wind_Speed);
          client.println("</font></P>"); 

          calcRainFall();
          client.println("<font face=verdana size=5 color=red><P> RAINFALL:");
          client.println(Raina);

          client.println("</font></P>");

          client.println("</html>");
          break;
        }
        //        }
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
    delay(1000);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
//=======================================================
// Find vane direction.
//=======================================================
void calcWindDir() {
  int val;
  byte x, reading;

  val = analogRead(PIN_VANE);
  val >>=2;                        // Shift to 255 range
  reading = val;

  // Look the reading up in directions table. Find the first value
  // that's >= to what we got.
  for (x=0; x<NUMDIRS; x++) {
    if (adc[x] >= reading)
      break;
  }
  //Serial.println(reading, DEC);
  x = (x + dirOffset) % 8;   // Adjust for orientation
  Serial.print("Dir: ");
  Serial.print(strVals[x]);
  Serial.println();
  //   WindDir[0] = strVals[0];
  WindDir = x;       
}


//=======================================================
// Calculate the wind speed, and display it (or log it, whatever).
//=======================================================
void calcWindSpeed() 
{
  sensorwind_value = analogRead(A1);//Analog 0
  if(sensorwind_value > 511)
  {
    wind_value=1;
  }
  else
  {
    wind_value=0;
  }
  if ((n == 0)&&(wind_value == 1))
  {
    wind_sum = wind_sum + 1;
  }
  wind_n=wind_value;
  windSpeed();         
}
void windSpeed()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > wind_interval) 
  {
    previousMillis = currentMillis;
    Serial.print(sum);
    Serial.print(",");
    wind_Speed = sum * 2.4;//in Kph
    Serial.print(wind_Speed);
    Serial.println("kph ");
    sum = 0;
  }  
}

//=======================================================
// Calculate the rain , and display it (or log it, whatever).
// 1 bucket = 0.2794 mm
//=======================================================
void calcRainFall() {
  int Value = 0;
  sensorValue = analogRead(A1);//Analog 0
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
  Raina = sum;
  Serial.print("RainFall = ");
  Serial.println(sum);
  sum = 0;
}       // Reset counter


void calcHumidity()
{
  DHT22_ERROR_t errorCode;
  Serial.println("calculating humidity");
  errorCode = myDHT22.readData();
  
  switch(errorCode)
  {
  case DHT_ERROR_NONE:
    //      Serial.print("Got Data ");
    humid = myDHT22.getHumidity();
    tempcel = myDHT22.getTemperatureC();
    Serial.print(myDHT22.getTemperatureC());
    Serial.print("C ");
    Serial.print(myDHT22.getHumidity());
    Serial.println("%");
    // Alternately, with integer formatting which is clumsier but more compact to store and
    // can be compared reliably for equality:
    //	  
          char buf[128];
     sprintf(buf, "Integer-only reading: Temperature %hi.%01hi C, Humidity %i.%01i %% RH",
     myDHT22.getTemperatureCInt()/10, abs(myDHT22.getTemperatureCInt()%10),
     myDHT22.getHumidityInt()/10, myDHT22.getHumidityInt()%10);
     Serial.println(buf);
        break;
  case DHT_ERROR_CHECKSUM:
    humid = myDHT22.getHumidity();
    tempcel = myDHT22.getTemperatureC();
    Serial.print("check sum error ");
    Serial.print(myDHT22.getTemperatureC());
    Serial.print("C ");
    Serial.print(myDHT22.getHumidity());
    Serial.println("%");
    break;
  case DHT_BUS_HUNG:
    Serial.println("BUS Hung ");
    break;
  case DHT_ERROR_NOT_PRESENT:
    Serial.println("Not Present ");
    break;
  case DHT_ERROR_ACK_TOO_LONG:
    Serial.println("ACK time out ");
    break;
  case DHT_ERROR_SYNC_TIMEOUT:
    Serial.println("Sync Timeout ");
    break;
  case DHT_ERROR_DATA_TIMEOUT:
    Serial.println("Data Timeout ");
    break;
  case DHT_ERROR_TOOQUICK:
    Serial.println("Polled to quick ");
    break;
  }

}



