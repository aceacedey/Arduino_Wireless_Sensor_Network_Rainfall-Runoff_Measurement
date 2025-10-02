/*mashup of arduino ethernet example and vortex anemometer example*/ 

#include <Ethernet.h>
#include <SPI.h>

//network configuration for the ethernet shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x2E, 0xC7 };
byte ip[] = { 10, 172, 3,201};
EthernetServer server(80);

//declare variables anemo
#define uint  unsigned int
#define ulong unsigned long
// declare pin 3, --> pin 2 or 3 are used as interrupt pins
#define PIN_ANEMOMETER  3     // Digital 3

// How often we want to calculate wind speed. Over a period of 5 secs, the number of revolutions are counted
#define MSECS_CALC_WIND_SPEED 5000

volatile int numRevsAnemometer = 0; // Incremented in the interrupt
ulong nextCalcSpeed;                // When we next calc the wind speed
ulong time;                         // Millis() at each start of loop().


//=======================================================
// Initialize
//=======================================================
void setup() {
    Ethernet.begin(mac, ip);
    server.begin();

    pinMode(PIN_ANEMOMETER, INPUT);
    digitalWrite(PIN_ANEMOMETER, HIGH);
    attachInterrupt(0, countAnemometer, FALLING);
    nextCalcSpeed = millis() + MSECS_CALC_WIND_SPEED;
}





//=======================================================
// Main loop.
//=======================================================
void loop() {
   time = millis();
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
              client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5"); 
          client.println();
 
          client.print("windspeed: ");
          client.print( //call windspeed function
 //               if (time >= nextCalcSpeed) {
                    calcWindSpeed()
 //                   nextCalcSpeed = time + MSECS_CALC_WIND_SPEED;
 //                }
          );
          client.print("\n");
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}


//=======================================================
// Interrupt handler for anemometer. Called each time the reed
// switch triggers (one revolution).
//=======================================================
void countAnemometer() {
   numRevsAnemometer++;
}

//=======================================================
// Calculate the wind speed,
// 1 rev/sec = 2.5 mph
//=======================================================
int calcWindSpeed() {
   int x, iSpeed;
   // This will produce mph * 10
   // (didn't calc right when done as one statement)
   long speed = 25000;//2.5 mph * 1000
   speed *= numRevsAnemometer;
   speed /= MSECS_CALC_WIND_SPEED;
   iSpeed = speed;         // Need this for formatting below
   numRevsAnemometer = 0;        // Reset counter
   return iSpeed;
}
 



