#define uint unsigned int
#define ulong unsigned long
#include <SD.h>
const int chipSelect = 10;
#include "RTClib.h"
#include <Wire.h>
#include <string.h>
RTC_DS1307 RTC;
char filename[] = "00000000.csv";
//unsigned long previousMillis_data = 0;
//long interval_data = 15000;
/// all connection :anemometer-digital 2,vane analog 3,rain gauge Analog 0,dht11 digital 6/////
#define PIN_ANEMOMETER  2     // Digital 2
#define PIN_VANE        3     // Analog 3

// How often we want to calculate wind speed or direction
#define MSECS_CALC_WIND_SPEED 1000
#define MSECS_CALC_WIND_DIR   1000

volatile int numRevsAnemometer=0; // Incremented in the interrupt
ulong nextCalcSpeed;                // When we next calc the wind speed
ulong nextCalcDir;                  // When we next calc the direction
ulong time;                         // Millis() at each start of loop().

//ADC readings:
#define NUMDIRS 8
ulong adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};
int count =0; //number of elements already added. Should be less than samples
// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
int w, median,o,g,t,x;
int middle, samples;
int reading[200];

char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;
//======================================================
//FOR RAINGAUGE
//======================================================
//unsigned int previousMillis = 0;
//int interval = 1000;
int sensorValue =0 ;
int sum=0;
int n=0;
int Value = 0;
//=======================================================
//For Wind Direction
//=======================================================
int index; //index corresponding to the direction to be printed
//=======================================================
//For Wind Speed
//=======================================================
int speed_correct_temp_x,speed_correct_temp_y;
//======================================================
//=======================================================
//For Humidity sensor
//=======================================================
unsigned int previous_humid_Millis = 0;
unsigned int interval_humid = 1000;
int sensorValue_humid=0;
int sum_humid=0,j=0,a;
int n_humid=0;
float humid,tempcel_median,dewp_median,dewpfst_median;
float humid_temp, tempcel_temp, dewp_temp, dewpfst_temp;
float sumhumid=0;
float sumtempcel=0;
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
//=======================================================
// Initialize
//=======================================================
void setup() {
   Serial.begin(9600);
   Wire.begin(); //Important for RTClib.h
   RTC.begin();
   if (! RTC.isrunning()) {
   Serial.println("RTC is NOT running!");
   return;
   }
   Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
   Serial.println("card initialized.");
   pinMode(PIN_ANEMOMETER, INPUT);
   digitalWrite(PIN_ANEMOMETER, HIGH);
   attachInterrupt(0, countAnemometer, FALLING);
  // pinMode(2, INPUT);
  // pinMode(3, INPUT);
   nextCalcSpeed = millis() + MSECS_CALC_WIND_SPEED;
   nextCalcDir   = millis() + MSECS_CALC_WIND_DIR;
   //number of samples (has to be odd number)
   samples=5;
}
//=======================================================
// Main loop.
//=======================================================
void loop() {
   if(count==samples)
     {
      getFileName();
      DateTime now = RTC.now();
      File dataFile = SD.open(filename, FILE_WRITE);
     // if the file is available, write to it:
      if (dataFile) {
        dataFile.print(now.hour(), DEC);
        dataFile.print(',');
        dataFile.print(now.minute(), DEC);  
        dataFile.print(',');
        dataFile.print(now.second(), DEC); 
        //dataFile.print(','); 
        //dataFile.print(sum);
        dataFile.print(',');
        dataFile.print(strVals[index]);
        dataFile.print(',');
        dataFile.print(speed_correct_temp_x);
        dataFile.print(".");
        dataFile.print(speed_correct_temp_y);
        dataFile.print(',');
        dataFile.print(humid_temp);
        dataFile.print(',');
        dataFile.print(tempcel_temp);
        dataFile.print(',');
        dataFile.print(dewp_temp);
        dataFile.print(',');
        dataFile.println(dewpfst_temp);
        //sum = 0; //reset value
       // count = 0;//reset count
       // numRevsAnemometer=0;
        dataFile.close();
       }
       Serial.print(now.hour(), DEC);
       Serial .print(',');
       Serial .print(now.minute(), DEC);  
       Serial .print(',');
       Serial.print(now.second(), DEC); 
       Serial .println(); 
    // Serial   .print(sum);
    //  Serial .println();
      Serial .print(strVals[index]);
     Serial  .println();
      Serial .print(speed_correct_temp_x);
     Serial   .print(".");
     Serial  .print(speed_correct_temp_y);
      Serial .println();
    Serial    .print(humid_temp);
    Serial  .print(',');
     Serial   .print(tempcel_temp);
     Serial   .print(',');
       Serial .print(dewp_temp);
      Serial .print(',');
     Serial  .println(dewpfst_temp);
        sum = 0; //reset value
        count = 0;//reset count
       // numRevsAnemometer=0;
        
     } 
   
if(count<samples)
    { 
      unsigned int currentMillis = millis();
    // if(currentMillis - previousMillis > interval) {
     // previousMillis = currentMillis;
           calcRainTips();
           delay(1000);
    // }
      time = millis();
    if (time >= nextCalcDir) {
          calcWindDir();
     nextCalcDir = time + MSECS_CALC_WIND_DIR;
     delay(1000);
     }
     if (time >= nextCalcSpeed) {
         calcWindSpeed();
      nextCalcSpeed = time + MSECS_CALC_WIND_SPEED;
      delay(1000);
     }
     if(currentMillis - previous_humid_Millis > interval_humid) {
        previous_humid_Millis = currentMillis; 
          calcHumidity();
          delay(1000);
     }
   }
 count = count +1; 
 }
void getFileName(){
DateTime now = RTC.now();
filename[0] = (now.year()/1000)%10 + '0'; //To get 1st digit from year()
filename[1] = (now.year()/100)%10 + '0'; //To get 2nd digit from year()
filename[2] = (now.year()/10)%10 + '0'; //To get 3rd digit from year()
filename[3] = now.year()%10 + '0'; //To get 4th digit from year()
filename[4] = now.month()/10 + '0'; //To get 1st digit from month()
filename[5] = now.month()%10 + '0'; //To get 2nd digit from month()
filename[6] = now.day()/10 + '0'; //To get 1st digit from day()
filename[7] = now.day()%10 + '0'; //To get 2nd digit from day()
}
//========================================================
// Interrupt handler for anemometer. Called each time the reed
// switch triggers (one revolution).
//=======================================================
void countAnemometer() {
   numRevsAnemometer++;
}
//=======================================================
// Find vane direction.
//=======================================================
void calcWindDir() {
   int val;
   int reading;

   val = analogRead(A3);
   val >>=2;                        // Shift to 255 range
   reading = val;

   // Look the reading up in directions table. Find the first value
   // that's >= to what we got.
   for (index=0; index<NUMDIRS; index++) {
      if (adc[index] >= reading)
         break;
   }
  index = (index + dirOffset) % 8;   // Adjust for orientation
 }
//=======================================================
// Calculate the wind speed, and display it (or log it, whatever).
// 1 rev/sec = 1.492 mph = 2.40114125 kph
//=======================================================
void calcWindSpeed() {
   int iSpeed;
   // This will produce mph * 10
   // (didn't calc right when done as one statement)
   long speed = 24011;
   speed *= numRevsAnemometer;
   speed /= MSECS_CALC_WIND_SPEED;
   iSpeed = speed;         // Need this for formatting below
    speed_correct_temp_x = iSpeed / 10;
    speed_correct_temp_y = iSpeed % 10;
    /*
   Serial.print("SPEED :");
   Serial.print(speed_correct_temp_x);
  Serial.print(".");
  Serial.print(speed_correct_temp_y);
  Serial.println();
  */
   numRevsAnemometer=0;
}

void calcRainTips() {
sensorValue = analogRead(A0);//Analog 0
//Serial.println(sensorValue);
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
  //Serial.println(sum);
  //sum = 0;
}

void calcHumidity()
{
  n=sensorValue_humid;
  int i, j;
  int chk = DHT11.read(DHT11PIN);
  humid_temp=(float)DHT11.humidity, 6;
  tempcel_temp=(float)DHT11.temperature, 6;
  dewp_temp=dewPoint(DHT11.temperature, DHT11.humidity);
  dewpfst_temp=dewPointFast(DHT11.temperature, DHT11.humidity);
}

