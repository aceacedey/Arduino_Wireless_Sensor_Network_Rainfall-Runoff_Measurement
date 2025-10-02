//Transmitter Code (Uno)
#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);
  
  vw_setup(2000);
  vw_set_tx_pin(7);
}

void loop()
{
  if(Serial.available())
  {
    char c = Serial.read();
    
    if(c == '1')
    {
      vw_send((uint8_t *)c, 1);
      Serial.println("1 sent");
  }
    else if(c == '0')
    {
      vw_send((uint8_t *)c, 1);
      Serial.println("0 sent");
     }
  }
}
