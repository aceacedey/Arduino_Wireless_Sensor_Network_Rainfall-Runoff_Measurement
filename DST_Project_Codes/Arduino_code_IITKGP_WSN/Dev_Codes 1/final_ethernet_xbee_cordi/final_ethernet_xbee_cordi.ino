

#include <SPI.h>
#include <Ethernet.h>
unsigned long previousMillis = 0;

int rain_Value=0, rain_sum=0, rain_n=0,rain_m=0, rain_sensorValue=0;        
float humid, tempcel, tempf, tempk;
long interval = 5000;

float raina = 0.0, rain_temp=0.0, raint = 0.0;
int sensorReading = 0;
char c;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xC7 };
IPAddress ip(10,144,0,51);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

unsigned int integerValue=0;  // Max value is 65535
char incomingByte;




void loop() {
    
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<font face=Tahoma Size=5 color=blue><h1 align=center>Measurement of Water Pressure of the Lane using Arduino Ethernet Shield & X-BEES @IIT-Kharagpur </font></h1>");

          client.println("<font face=Tahoma Size=5 color=GREEN><h2 align=center>LOCATION 5 : S.T.E.P. </font></h2>");
      
          // output the value of each analog input pin
          serialRead();
         // int s = analogRead(A0);
          client.println("</font></P>");
          
          client.println("<font face=verdana size=5 color=red><P> Pressure Sensor Reading is >>> ");
          client.println(sensorReading);
          
          client.println("</font></P>");
         
          rainfall();
          client.println("<font face=verdana size=5 color=Green><P> Counting Since >>> 21st September, 2014");
          
          client.println("</font></P>");
          
          client.println("<font face=verdana size=5 color=red><P> TOTAL RAIN TIP COUNT = ");
          client.println(rain_sum);
          
          client.println("<font face=verdana size=5 color=red><P> Current RAINFALL : ");
          client.println(raint);
          client.println(" mm");
          client.println("</font></P>");

          
          client.println("<font face=verdana size=5 color=red><P> Total RAINFALL since 11-05-2015 : ");
          client.println(raina);
          client.println(" mm");
          client.println("</font></P>");        
         
          client.println("<br />");       
          
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
    Serial.println("client disonnected");
  }
}
void serialRead() 
{
  if (Serial.available() > 0) 
  {   // something came across serial
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      Serial.print("-----------------------the in coming byte is : ");
      Serial.println(incomingByte);
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
    }
    Serial.println(integerValue);   // Do something with the value
  }
 sensorReading = integerValue;
}
void rainfall()
{
  rain_sensorValue = analogRead(A5);//Analog 0
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
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;
    raina = rain_sum * 0.254;
    rain_m = rain_sum - rain_temp;
    raint = rain_m * 0.254;
    rain_temp = rain_sum;
    Serial.println(raina);
    rain_m = 0;
 //   rain_sum = 0;
//listenForEthernetClients();  
  } 
 else 
    rainfall();
}


