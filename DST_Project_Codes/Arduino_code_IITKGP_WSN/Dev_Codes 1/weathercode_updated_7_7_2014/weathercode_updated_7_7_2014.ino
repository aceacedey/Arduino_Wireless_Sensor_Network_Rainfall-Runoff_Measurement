#include <Ethernet.h>
#include <SPI.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long

#define PIN_ANEMOMETER  A3    
#define PIN_VANE        A5     // Analog 5

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


long wind_interval = 3000;
int wind_sensorValue =0 ;
int wind_sum=0;
int wind_n=0;
int wind_Value = 0, Value = 0; 

// ADC readings:
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {26, 45, 77, 118, 161, 177, 197, 256};

// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
char *strVals[NUMDIRS] = {
  "W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;
unsigned long previousMillis = 0;
long interval = 10000;
int sensorValue =0 ;
int sum=0, temp = 0;
int n=0;
int Value = 0;
float Speed;
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
  Serial.begin(9600);
}
void loop()
{
 
//    Serial.println(Speed);
listenForEthernetClients();


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
 //         client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          // print the current readings, in HTML format:
          windspeed();
          client.print("rotation: ");
          client.print(temp);
          client.print("Wind speed: ");
          client.print(Speed);
          client.print("km/hr");
          client.println("<br />");
          
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

void windspeed()
{
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
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
    Serial.print(sum);
    Serial.print(",");
    Speed = sum * 2.4;//in Kph
    temp = sum;
    Serial.println(Speed);
    sum = 0;
//listenForEthernetClients();  
  } 
 else 
    windspeed();
}






