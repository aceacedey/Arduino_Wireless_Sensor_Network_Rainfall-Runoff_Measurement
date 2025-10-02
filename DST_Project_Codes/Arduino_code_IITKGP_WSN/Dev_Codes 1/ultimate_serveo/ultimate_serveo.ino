/*
 Arduino Data Web Server

 Performs actions on Arduino and/or returns data from Arduino to a webbrowser request URL:
  - To turn on LED on pin 8, use web browser to visit your Arduino via: http://x.x.x.x/digitalWrite/7/1
  - To turn it off send: http://x.x.x.x/digitalWrite/7/0
  - To read analog0 value, send: http://x.x.x.x/analogRead/0


*/

#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <Servo.h> 
Servo myservo;
// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x98, 0x52 };
byte ip[] = {10,124,68,182}; // This is typically 10.0.0.x

EthernetServer server(80); // Port 80 is http.

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

}

void setup()
{
  myservo.attach(9);
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.begin(9600);
  SetupSamplePorts();
}

#define bufferMax 128
int bufferSize;
char buffer[bufferMax];

void loop()
{
  EthernetClient client = server.available();
  if (client)
  {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
//	  client.println("Refresh: 5");
          client.println();
 //         client.println("<!DOCTYPE HTML>");
 //         client.println("<html>");
          // output the value of each analog input pin
          
 //           client.println("</html>");
          
    WaitForRequest(client);
    ParseReceivedRequest();
    PerformRequestedCommands();
    client.stop();
  }
}

void WaitForRequest(EthernetClient client) // Sets buffer[] and bufferSize
{
  bufferSize = 0;
 
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n')
       
          break;
      else
        if (bufferSize < bufferMax)
          buffer[bufferSize++] = c;
        else
          break;
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
  if ( strcmp(cmd,"?move") == 0 ) movement();
  
  if ( strcmp(cmd,"?analogRead") == 0 ) RemoteAnalogRead();
}

void movement()
{
  
  int x = param1[0] - '0'; 
  int y = 0 ;
  Serial.println("x is equal to = ");
  Serial.println(x);
  Serial.println(y);
  if ( x > y )
  {
  i = x;
  Serial.println("i will be") ;
  Serial.print(x);
  int pos=0;
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);   
  }
  }
  if ( x < y )
  { 
  j = x;
  Serial.println("j is = "); 
  Serial.print (x);
  int pos=0;
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  }
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
