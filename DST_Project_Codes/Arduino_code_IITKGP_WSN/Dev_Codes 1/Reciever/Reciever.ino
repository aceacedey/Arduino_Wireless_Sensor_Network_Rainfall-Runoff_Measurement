//Reciever Code (Leonardo)
#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  vw_setup(2000);
  vw_set_rx_pin(7);
  vw_rx_start();
}

void loop()
{
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[buflen];
  
  if(vw_get_message(buf, &buflen))
  {
    Serial.println("in the recive loop");
    for(int i = 0;i < buflen;i++)
    {
      if(buf[i] == '1')
      {
        digitalWrite(13,HIGH);
        Serial.println("LED on");  
    }
      else if(buf[i] == '0')
      {
        digitalWrite(13,LOW);
        Serial.println("LED off");
      }
    }
  }
}
