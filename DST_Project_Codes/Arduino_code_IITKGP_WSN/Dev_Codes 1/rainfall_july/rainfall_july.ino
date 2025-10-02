
#include <Ethernet.h>
#include <SPI.h>
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
  Ethernet.begin(mac, ip);
  server.begin();
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:

void loop() 
{  
 
  listenForEthernetClients();
          // delay in between reads for stability
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
    raina = ((float)rain_sum * 0.254);
    
    Serial.println(raina); 
  } 
 else 
    rainfall();

}


void listenForEthernetClients() 
{
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
	  client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          // print the current readings, in HTML format:
          client.println("<!DOCTYPE HTML>");

          client.println("<html>");

           
          client.println("<font face=Tahoma Size=5 color=blue><h1 align=center>Weather Report using Arduino Ethernet Shield @ IIT-Kharagpur </font></h1>");

          client.println("<font face=Tahoma Size=5 color=GREEN><h2 align=center>LOCATION 3 : School of Water Resources </font></h2>");
          calcHumidity(); 
          
          
          client.println("<font face=verdana size=5 color=RED><P>HUMIDITY :");
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
          client.println("<font face=verdana size=5 color=Green><P> Counting Since >>> 10th July, 2014");
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

