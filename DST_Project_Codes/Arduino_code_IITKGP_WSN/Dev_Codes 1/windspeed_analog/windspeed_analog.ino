unsigned long previousMillis = 0;
long wind_interval = 5000;
int sensorValue =0 ;
int sum=0;
int temp=0;
int wind_value = 0, i=0; 
void setup()
{
Serial.begin(9600);
}
void loop()
{
  wind_sum = 0 ;
//  Serial.println("OFFSET SUM");
for (wind_n = 0 ; wind_n < 10 ; wind_n++)
{

wind_sensorValue = analogRead(A1);//Analog 0
//Serial.print("Sensor value - ");
//Serial.println(sensorValue);
if (wind_sensorValue > 900)
wind_Value = 1 ;
else 
wind_Value = 0 ;

wind_sum = wind_Value + wind_sum ;
//Serial.print("value of Sum in the loop");
//Serial.println(sum);
delay(500);
}
Serial.print("::::: Fianl Sum :::::");
Serial.println(sind_sum);
wind_speed = wind_sum * 2.4;
Serial.println(wind_speed);
//delay(5000);
}

 
  


 

