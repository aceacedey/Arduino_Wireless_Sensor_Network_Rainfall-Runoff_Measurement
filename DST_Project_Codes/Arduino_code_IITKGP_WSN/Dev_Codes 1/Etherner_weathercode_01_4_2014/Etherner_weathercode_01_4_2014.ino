
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long

#define PIN_ANEMOMETER  2     // Digital 2
#define PIN_RAINGAUGE  3     // Digital 3
#define PIN_VANE        5     // Analog 5


// How often we want to calculate wind speed or direction
#define MSECS_CALC_WIND_SPEED 60000
#define MSECS_CALC_WIND_DIR   60000
#define MSECS_CALC_RAIN_FALL  60000

volatile int numRevsAnemometer = 0; // Incremented in the interrupt
volatile int numDropsRainGauge = 0; // Incremented in the interrupt
ulong nextCalcSpeed;                // When we next calc the wind speed
ulong nextCalcDir;                  // When we next calc the direction
ulong nextCalcRain;                  // When we next calc the rain drop
ulong time;                         // Millis() at each start of loop().

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

byte WindDir;
float humid,tempcel,tempfar,tempkel,dewp,dewpfst;
int Raina,Rainb,Winda,Windb;
float reqhumid,reqtempcel,reqtempfar,reqtempkel,reqdewp,reqdewpfst;
float sumhumid=0;
float sumtempcel=0;
float sumtempfar=0;
float sumtempkel=0;
float sumdewp=0;
float sumdewpfst=0;
//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
	double A0= 373.15/(273.15 + celsius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * humidity;
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity/100);
	double Td = (b * temp) / (a - temp);
	return Td;
}


#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 6

// MAC address can be anything that is unique within your network.
// IP is the address the Arduino Ethernet Card would respond to.  It needs to be an unused address within your network.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x98, 0x52 };
byte ip[] = {10,19,2,42}; // This is typically 10.0.0.x

EthernetServer server(80); // Port 80 is http.

void setup()
{
   Serial.begin(9600); 
   pinMode(PIN_ANEMOMETER, INPUT);
   digitalWrite(PIN_ANEMOMETER, HIGH);
   digitalWrite(PIN_RAINGAUGE, HIGH);
   attachInterrupt(0, countAnemometer, FALLING);
   attachInterrupt(1, countRainGauge, FALLING);
   nextCalcRain = millis() + MSECS_CALC_RAIN_FALL;
   nextCalcSpeed = millis() + MSECS_CALC_WIND_SPEED;
   nextCalcDir   = millis() + MSECS_CALC_WIND_DIR;
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
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
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
//          while(client.connected())
          client.println("<body background=\"http://lh6.googleusercontent.com/-ik0yNPnA7Lc/AAAAAAAAAAI/AAAAAAAAFhE/rZa6DcOG09I/photo.jpg\"</body>");
          client.println("<font color=blue><h1 align=center>Weather Report using Arduino Ethernet Sheild </font></h1>");
             calcHumidity();
             client.println("<font color=green><P>HUMIDITY :");
             client.println(humid);
             client.println("</font></P>");
//             delay(1000);
              
             client.println("<font color=red><P>TEMPERATURE :");
             client.println(tempcel);
             client.println("°C   ");
             client.println(tempfar);
             client.println("F    ");
             client.println(tempkel);
             client.println("K    ");
             client.println("</font></P>");
             client.println("<font color=green><P> DEWPOINT :");
             client.println(dewp);
             client.println("</font></P3>");
             client.println("<font color=red><P> DWEFST :");
             client.println(dewpfst);
             client.println("</font></P>");
  //           delay(1000);


               calcWindDir();
               client.println("<font color=green><P>");
               client.print("WIND DIRECTION : ");
               client.print(strVals[WindDir]);
               client.println("</font></P>");

//             delay(1000);
             calcWindSpeed();
             client.println("<font color=red><P> WINDSPEED:");
             client.println(Winda);
             client.print(".");
             client.print(Windb);
             client.println("</font></P>"); 
//             delay(1000);
              calcRainFall();
              client.println("<font color=green><P> RAINFALL:");
              client.println(Raina);
              client.print(".");
              client.print(Rainb);
              client.println("</font></P>");
//              delay(100);
  
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



void countAnemometer() 
{
   numRevsAnemometer++;
}

//=======================================================
// Interrupt handler for rain gauge. Called each time the reed
// switch triggers (one drop).
//=======================================================
void countRainGauge() {
   numDropsRainGauge++;
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
// 1 rev/sec = 1.492 mph = 2.40114125 kph
//=======================================================
void calcWindSpeed() {
   int x,y, iSpeed;
   // This will produce kph * 10
   // (didn't calc right when done as one statement)
   long speed = 24011;
   speed *= numRevsAnemometer;
   speed /= MSECS_CALC_WIND_SPEED;
   iSpeed = speed;         // Need this for formatting below

   Serial.print("Wind speed: ");
   x = iSpeed / 10;
   y = iSpeed % 10;
  Serial.print(x);
  Serial.print(".");
  Serial.print(y);
  Serial.println();
  Winda = x;
  Windb = y;
   numRevsAnemometer = 0;        // Reset counter
}

//=======================================================
// Calculate the rain , and display it (or log it, whatever).
// 1 bucket = 0.2794 mm
//=======================================================
void calcRainFall() {
   int a,b, iVol;
   // This will produce mm * 10000
   // (didn't calc right when done as one statement)
   long vol = 0.2794; // 0.2794 mm
   vol *= numDropsRainGauge;
   iVol = vol;         // Need this for formatting below

   Serial.print("Rain fall: ");
   a = iVol / 10000;
    b = iVol % 10000;
  Serial.print(a);
  Serial.print(".");
  Serial.print(b);
  Serial.println();
  Raina = a;
  Rainb = b;
 numDropsRainGauge = 0;        // Reset counter
}

void calcHumidity()
{
  n=sensorValue;
  int chk = DHT11.read(DHT11PIN);
  humid=(float)DHT11.humidity, 6;
  tempcel=(float)DHT11.temperature, 6;
  tempfar=Fahrenheit(DHT11.temperature), 6;
  tempkel=Kelvin(DHT11.temperature),6;
  dewp=dewPoint(DHT11.temperature, DHT11.humidity);
  dewpfst=dewPointFast(DHT11.temperature, DHT11.humidity);
  Serial.print("Humidity:");
  Serial.print(humid);
  Serial.println();
  Serial.print("Temp:");
  Serial.print(tempcel);
  Serial.print(" ");
  Serial.print(tempfar);
  Serial.print(" ");
  Serial.print(tempkel);
  Serial.println();
  Serial.print(" DEW POINT:");
  Serial.print(dewp);
  Serial.println();
  Serial.print("dewpfst:");
  Serial.print(dewpfst);
  Serial.println(); 
}


