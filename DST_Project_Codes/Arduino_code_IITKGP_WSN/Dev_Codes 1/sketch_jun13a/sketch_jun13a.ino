

#include <SPI.h>
//#include <Ethernet.h>
#include <string.h>
#define uint  unsigned int
#define ulong unsigned long


// How often we want to calculate wind speed or direction

// ADC readings:
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};

// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
//char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
//byte dirOffset=0;
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
int Raina,Winda,Windb;
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

void setup()
{
 Serial.begin(9600);
 
}

void loop()
{
  
calcHumidity();
delay(1000);
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

