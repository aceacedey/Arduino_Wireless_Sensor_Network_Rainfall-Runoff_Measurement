
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long

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
unsigned long previoushumidMillis = 0;

long interval =15000;
long intervalhumid=60000;
int wind_sensorValue=0,rain_sensorValue=0;
int wind_sum=0,rain_sum=0,rain_Value=0,rain_temp=0,rain_n=0,wind_n=0,wind_temp=0,j=0,a;

char buffer[6];
float humid, tempcel, wind_speed, tempf, tempk, f, wind_angle;
int WindDir = 0, angle = 0, wind_Value=0;

float raina;

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xAE};
byte ip[] = {10,124,68,100}; // This is typically 10.0.0.x

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

          client.println("<font face=Tahoma Size=6 color=blue><h1 align=center>Weather Report using Arduino Ethernet Shield @ IIT-Kharagpur </font></h1>");
          client.println("<font face=Tahoma Size=4 color=GREEN><h2 align=center>LOCATION : 2 >> V.S.R.C. </font></h1>");
         
   windDirection();
   windspeed();
    calcHumidity();
 
/*          client.println("<font face=verdana size=5 color=RED><P>HUMIDITY :");
          client.println(humid);
          client.println(" %   ");
          client.println("</font></P>");

          client.println("<font face=verdana size=5 color=RED><P>TEMPERATURE :");
          client.println(tempcel);
          client.println("C   ");
          client.println(tempf);
          client.println("F    ");
          client.println(tempk);
          client.println("K    "); 
          client.println("</font></P>");
          
         
 */         
          client.println("<font face=verdana size=5 color=red><P>");
          client.print("WIND DIRECTION : ");
          client.print(strVals[angle]);
          client.println("</font></P>");

          
          
        //  rainfall();
           
 
          client.println("<font face=verdana size=5 color=red><P> WINDSPEED:");
          client.println(wind_speed);
          client.println(" Km/h   ");
          client.println("</font></P>"); 

          
          client.println("<font face=verdana size=5 color=red><P> RAINFALL:");
          client.println(raina);
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


void windDirection()
{
             int val;
             byte x, reading;

            val = analogRead(A3);
            val >>=2;                        // Shift to 255 range
            reading = val;
Serial.println(reading);
            // Look the reading up in directions table. Find the first value
            // that's >= to what we got.
            for (x=0; x<NUMDIRS; x++) 
            {
              if (adc[x] >= reading)
                break;
            }
            //Serial.println(reading, DEC);
            x = (x + dirOffset) % 8;   // Adjust for orientation
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
           
           Serial.println(angle);
           Serial.println(strVals[angle]);
           
           f = angle ;
           wind_angle = f * 45 ;

}

void windspeed()
{
  rain_sensorValue = analogRead(A1);
  wind_sensorValue = analogRead(A5);//Analog 0
 Serial.println(rain_sensorValue);
 Serial.println(wind_sensorValue);
  if(wind_sensorValue > 1000)
  {
    wind_Value=1;
  }
  else
  {
    wind_Value=0;
  }
  if ((wind_n == 0)&&(wind_Value == 1))
  {
    wind_sum = wind_sum + 1;
  }
  wind_n=wind_Value;
 
  if(rain_sensorValue > 1000)
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
  
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
    Serial.print(wind_sum);
    Serial.print(",");
    wind_speed = (wind_sum * 2.4) / 15;//in Kph
    wind_temp = wind_sum;
    Serial.println(wind_speed);
    wind_sum = 0;
    
    Serial.print(rain_sum);
    Serial.print(",");
    raina = rain_sum * 0.254;
    rain_temp = rain_sum;
    Serial.println(raina);
//listenForEthernetClients();  
  } 
 else 
    windspeed();
}

/* void rainfall()
{
  rain_sensorValue = analogRead(A2);//Analog 0
  if(rain_sensorValue > 1000)
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
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
    Serial.print(rain_sum);
    Serial.print(",");
    raina = rain_sum * 0.254;
    rain_temp = rain_sum;
    Serial.println(raina);
 //   rain_sum = 0;
//listenForEthernetClients();  
  } 
 else 
    rainfall();
}



*/


