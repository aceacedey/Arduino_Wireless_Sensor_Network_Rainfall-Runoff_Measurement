/*
Wind Meter – by Keff Edwards
 
 CC-ShareAlike
 */

int verbose = 1;

// DATA Vars
float windSpeed; //WindSpeed VAR

//Pin Ass VARS

int windSpeedPin = A1; //WindSpeed INT

// Prog CTRL Vars
int windOnce = 0; //Var used to calculate wind speed triggers
float windTime = 0; //When Wind Speed Trigger Occurs
float sSinceLast = 0; //How long since the last wind speed trigger

float windSpeedCon = 1.492; //Constant for MPH/Revolution

void setup()
{
  Serial.begin(9600);

  //Base
  Serial.println("WindSpeed/Direction Sensor Init");
  attachInterrupt(windSpeedPin, windspeed, RISING); //Init the Interupt to trigger on rising edge

}

void loop()
{
   windspeed();
  if (windOnce > 1) 
  {
    calcWind();
    delay(1000);
  } //Every Second Trigger process data
}

//Wind Speed INT Trigger
void windspeed() 
{
  windOnce++; //increment windOnce by one
}

//Function to Process Wind Data
void calcWind() {

  float actRPS; //local VAR to store Revolutions per second
  int vainData; //local VAR to store ADC data for WindVAIN

  sSinceLast = ( millis() - windTime) ; //Calculate how much time has passed since the
  windTime = millis(); //Set windTime to current time to use on next Trigger.
  actRPS = 30 * 1000 / (sSinceLast) / 60; //calculate the RPS
  windSpeed = actRPS * windSpeedCon; //Calculate Windspeed in MPH
  if (verbose) {
    Serial.print(windSpeed);
    Serial.println("mph");
  }
  windOnce = 0; //reset trigger VAR to 0
}

