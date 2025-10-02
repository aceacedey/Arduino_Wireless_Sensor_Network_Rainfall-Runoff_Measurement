
#include <SPI.h>
#include <WiFi.h>

#include <string.h>



char ssid[] = "Departments";      // your network SSID (name) 
//char pass[] = "secretPassword";   // your network password
//int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80); // Port 80 is http.

//-- Commands and parameters (sent by browser) --
char cmd[15];    // Nothing magical about 15, increase these if you need longer commands/parameters
char param1[15];
char param2[15];
int motorPin1 = 8;
int motorPin2 = 9;
int motorPin3 = 6;
int motorPin4 = 7;
int x=0;
int i=1;
int j=1;
int delayms = 1000;
//-- Sample Ports ---
void SetupSamplePorts()
{
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(13,OUTPUT);
//  pinMode(A0,INPUT);
}

void setup()
{
  Serial.begin(9600);
  SetupSamplePorts();
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
   while ( status != WL_CONNECTED) 
   { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
   
}

#define bufferMax 128
int bufferSize;
char buffer[bufferMax];

void loop()
{
  
  WiFiClient client = server.available();
  if (client)
  {
          Serial.println("new client");
          
    WaitForRequest(client);

    ParseReceivedRequest();
    PerformRequestedCommands();

    client.stop();
    Serial.println("client disonnected");
    }
}

void WaitForRequest(WiFiClient client) // Sets buffer[] and bufferSize
{
  bufferSize = 0;
 
String currentLine = "";
while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      if (c == '\n')
       {
             if (currentLine.length() == 0) 
             {
               client.println("HTTP/1.1 200 OK");
               client.println("Content-Type: text/html");
              client.println("Refresh: 5");            
                client.println();
               break;
             }
             else 
             {      // if you got a newline, then clear currentLine:
             currentLine = "";
             } 
       }
      else
        if (bufferSize < bufferMax)
          buffer[bufferSize++] = c;
  
    }
  }
  
 PrintNumber("bufferSize", bufferSize);
}

void ParseReceivedRequest()
{
  Serial.println("in ParseReceivedRequest");
  Serial.println(buffer);
  
  //Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
  char* slash1;
  char* equal;
  //char* slash3;
  char* space2;
  
  slash1 = strstr(buffer, "/") + 1; // Look for first slash
  equal = strstr(slash1, "=") + 1; // second slash
 // slash3 = strstr(slash2, "/") + 1; // third slash
  space2 = strstr(equal, " ") + 1; // space after second slash (in case there is no third slash)
  //if (slash3 > space2) slash3=slash2;

  PrintString("slash1",slash1);
  PrintString("slash2",equal);
//  PrintString("slash3",slash3);
  PrintString("space2",space2);
  
  // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
  cmd[0] = 0;
  param1[0] = 0;
  param2[0] = 0;
  strncat(cmd, slash1, equal-slash1-1);
  strncat(param1, equal, space2-equal-1);
 // strncat(param2, slash3, space2-slash3-1);
  
  PrintString("cmd",cmd);
  PrintString("param1",param1);
  PrintString("param2",param2);
}

void PerformRequestedCommands()
{
 // if ( strcmp(cmd,"?move") == 0 ) movement();
  
  if ( strcmp(cmd,"?analogRead") == 0 ) RemoteAnalogRead();

  if ( strcmp(cmd,"?led") == 0 ) led();
}


void RemoteAnalogRead()
{
  // If desired, use more server.print() to send http header instead of just sending the analog value.
  int analogPin = param1[0] - '0'; // Param1 should be one digit analog port
  int analogValue = analogRead(analogPin);
  
  //-- Send response back to browser --

  //server.println(" - The Sensor is Working -");
  server.print("A");
  server.print(analogPin, DEC);
  server.print(" is ");
  server.print(analogValue,DEC);
  
  //-- Send debug message to serial port --
  Serial.println("RemoteAnalogRead");
  PrintNumber("analogPin", analogPin);
  PrintNumber("analogValue", analogValue);
}
void PrintString(char* label, char* str)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(str);
}

void PrintNumber(char* label, int number)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(number, DEC);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void led()
{
     if ( param1[0] >= '0')
       digitalWrite(13,HIGH);
      else
       digitalWrite(13,LOW);
}
