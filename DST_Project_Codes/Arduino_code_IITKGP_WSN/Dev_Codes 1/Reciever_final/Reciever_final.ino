// RF Link using VirtualWire to receive messages
// simplex (one-way) receiver with a 315MHz RF Link Receiver module
// rx pin 23 on mega

#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round
void setup()
{
  Serial.begin(9600);    

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);                   // Bits per sec
  vw_set_rx_pin(3);           // We will be receiving on pin 23 (Mega) ie the RX pin from the module connects to this pin. 
  vw_rx_start();                      // Start the receiver 
 // pinMode (9,OUTPUT); 
}

void loop()
{ 
  //Serial.println("initializing");
  int j[20]; 
  double temp = 0;
  float k;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // check to see if anything has been received
  {
    int i = 0;
    // Message with a good checksum received.
    Serial.print("length ");
    Serial.println(buflen);
    for (i = 0; i < buflen; i++)
    {
     Serial.println(buf[i]);  // the received data is stored in buffer
      j[i] = buf[i] - '0' ;

      Serial.println(j[i]);     
      temp = pow(10, ( buflen - i - 1)) * j[i] + temp;
      Serial.println(temp);  
}
      k = temp ;
  //  k = 100 * j[0] + 10 * j[1] + j[2] ;
      long val = ((long)k);
      Serial.println(val);
     
   val = map(k, 0, 1023, 0, 255);
  Serial.println(val);
  
//   analogWrite(9, val);
  
 
 /*  if( k < 500) 
      digitalWrite(7,HIGH);
    else
      digitalWrite(7,LOW); 
 */
 
  }
 delay(1000);
}



