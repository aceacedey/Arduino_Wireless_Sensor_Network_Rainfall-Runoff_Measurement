#include <VirtualWire.h>  
#include <NewPing.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
#define TRIGGER_PIN  12  
#define ECHO_PIN     11  
#define MAX_DISTANCE 80
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
String speedval;
String distval;
int speedvaltemp;
String message;
char sentvalues[27];
double sensorValue;
int sensorValueint;
int sensorValuedist;
int tempholder;
void setup()
{
  Serial.begin(9600);

  vw_setup(2000);            
  vw_set_tx_pin(3);        
}

void loop()
{
  delay(50); 
  //setting up values to send
  long cm;
  unsigned int uS = sonar.ping();
  sensorValue = analogRead(A0);

  // just to get a resonable 2 digit number from the sensor, not a real world solution for a 
  //higher number of digits unless I reverse the calculation at the other end and lose accuracy
  sensorValue = sensorValue/11;
  //

  cm = uS / US_ROUNDTRIP_CM;
  sensorValueint = (int) round(sensorValue);
  sensorValuedist = (int) round(cm);
  if(sensorValueint < 10){
    speedval = "0";  
    speedval += sensorValueint;
  }
  else {
    speedval += sensorValueint; 
  }

  if(sensorValuedist < 10){
    distval = "0";  
    distval += sensorValuedist;
    if(sensorValuedist > 0){
      tempholder = sensorValuedist; 
    }

  }
  else {
    distval += sensorValuedist; 
    if(sensorValuedist > 0){
      tempholder = sensorValuedist; 
    }
  }

  if(sensorValuedist == 0){
    sensorValuedist = tempholder; 
    distval = "";
    distval += sensorValuedist;
  }

  // stop setting up values

  // construct message
  message = "S" + speedval + "D" + distval; 
  //send to array
  message.toCharArray(sentvalues, 27);

  // reset strings to empty
  speedval = "";
  distval = "";

  //Check message going out
  Serial.print(message);

  // send message
  vw_send((uint8_t *)sentvalues, strlen(sentvalues));
  vw_wait_tx();                                          
  delay(200);
}



