#include <VirtualWire.h> 
#undef int
#undef abs
#undef double
#undef float
#undef round
#define CHAR_ESC "\x1B" // necessary to make sending ESC codes easier
int speednum;
char tempnum;
boolean speednow = false;
boolean tempnow = false;
boolean started = false;
void setup()
{
  Serial.begin(57600);    
  //tellymate code
  showcursor(false);
  //

  vw_setup(2000);                   
  vw_set_rx_pin(23);           
  vw_rx_start();                     
}

void loop()
{
  speednow = false;
  tempnow = false;
  speednum = 0;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // check to see if anything has been received
  {
    int i;


    for (i = 0; i < buflen; i++)
    {
      if(buf[i] == 'S'){  
        speednow = true;
        tempnow = false;
      }

      if(buf[i] == 'T'){  
        speednow = false;
        tempnow = true;
      }


    }

  }



  movecursor(7,5);
  Serial.print("Your Speed is: ");
  //tellymate code
  movecursor(9,5);
  Serial.write(CHAR_ESC); 
  Serial.print( "_9") ;
  Serial.write(CHAR_ESC); 
  Serial.print( "_2") ; // Large text, top half
  //

  Serial.write(buf[1]);
  Serial.write(buf[2]);

  //tellymate code
  movecursor(10,5);
  Serial.write(CHAR_ESC); 
  Serial.print( "_3") ; // Large text, bottom half
  //


  Serial.write(buf[1]);
  Serial.write(buf[2]);

  //tellymate code
  movecursor(12,5);
  //
  Serial.print("The distance to the next car is: ");
  //tellymate code
  movecursor(15,5);
  Serial.write(CHAR_ESC); 
  Serial.print( "_9") ;
  Serial.write(CHAR_ESC); 
  Serial.print( "_2") ; // Large text, top half
  //
  Serial.write(buf[4]);
  Serial.write(buf[5]);
  //tellymate code
  movecursor(16,5);
  Serial.write(CHAR_ESC); 
  Serial.print( "_3") ; // Large text, bottom half
  //

  Serial.write(buf[4]);
  Serial.write(buf[5]);

  if(started == false){
    //tellymate code
    screen_clear();
    //
  }
  started = true;

}
//tellymate code
void movecursor( uint8_t row , uint8_t col )
{ // <ESC>Yrc
  // screen is 38 across, 25 down
  Serial.write( CHAR_ESC "Y" ) ;
  Serial.write((unsigned char)(32 + row)) ;
  Serial.write((unsigned char)(32 + col)) ;
}

void showcursor( boolean show )
{ // <ESC>e or <ESC>f
  Serial.print( CHAR_ESC ) ;
  Serial.print( show?'e':'f' ) ;
}

void screen_clear()
{ //clears the screen
  Serial.write( CHAR_ESC "E" ); 
}
//

