unsigned long previousMillis = 0;
long interval = 5000;
int wind_sensorValue =0 ;
int wind_sum=0;
int wind_n=0;
int wind_Value = 0;
void setup()
{
  Serial.begin(9600);
}
void loop ()
{
  for (wind_n = 0 ; wind_n < 10 ; wind_n++)

  {

    wind_sensorValue = analogRead(A1);//Analog 0
    Serial.print("Sensor value - ");
    Serial.println(wind_sensorValue);
    if (wind_sensorValue > 900)
      wind_Value = 1 ;
    else 
      wind_Value = 0 ;

    wind_sum = wind_Value + wind_sum ;
    Serial.print("value of Sum in the loop");
    Serial.println(wind_sum);
    delay(500);

  }
  Serial.println(wind_sum);
  Serial.print(",");
  float Speed = wind_sum * 2.4;//in Kph
  Serial.print(Speed);
  Serial.println("kph ");
  
  wind_sum = 0;

}






