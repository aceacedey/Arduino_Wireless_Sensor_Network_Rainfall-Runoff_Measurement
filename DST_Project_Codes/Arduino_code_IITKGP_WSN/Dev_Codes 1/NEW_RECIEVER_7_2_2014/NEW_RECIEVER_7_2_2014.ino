// RF Link using VirtualWire to receive messages
// simplex (one-way) receiver with a 315MHz RF Link Receiver module
// rx pin 23 on mega

#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round
#include <SPI.h>
#include <Ethernet.h>

int value = 0 ;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xC7 };
IPAddress ip(10,172,3,201);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup()
{
  Serial.begin(9600);    

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);                   // Bits per sec
  vw_set_rx_pin(3);           // We will be receiving on pin 23 (Mega) ie the RX pin from the module connects to this pin. 
  vw_rx_start();                      // Start the receiver 
  pinMode (7,OUTPUT); 

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop()
{ 
  EthernetClient client = server.available();
  if (client) 
  {
  
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
  
// listen for incoming clients
  
  
    
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
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
             client.print("The Pressure Sensor Reading is ");
         //  int sensor = analogRead(A0);
        delay(2000);
                 int j[20], k;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // check to see if anything has been received
  {
    int i;
    // Message with a good checksum received.

    for (i = 0; i < buflen; i++)
    {
      //   Serial.println(buf[i]);  // the received data is stored in buffer
      j[i] = buf[i] - '0' ;

      Serial.println(j[i]);     
    }
    k = 100 * j[0] + 10 * j[1] + j[2] ;
    Serial.println(k);
    
  //  for ( int t = 0; t < k ; t++)
   //  value = value + 1 ;
   
      value = 1000 ;
          }
          
        
          int val = map(k, 0, 1023, 0, 255);
          client.print(value);
          client.println(k);
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
    delay(1000);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

}





