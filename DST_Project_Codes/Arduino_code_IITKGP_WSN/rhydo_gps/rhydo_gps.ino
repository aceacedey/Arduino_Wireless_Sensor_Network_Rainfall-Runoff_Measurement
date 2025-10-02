
 
  int Gpsdata;             // for incoming serial data
  unsigned int finish =0;  // indicate end of message
  unsigned int pos_cnt=0;  // position counter
  unsigned int lat_cnt=0;  // latitude data counter
  unsigned int log_cnt=0;  // longitude data counter
  unsigned int flg    =0;  // GPS flag
  unsigned int com_cnt=0;  // comma counter
  char lat[20];            // latitude array
  char lg[20];             // longitude array
 
  void Receive_GPS_Data();
 
/*******************************************************************************************
 Function    : setup()
 Description : Use it to initialize variables, pin modes, start using
               libraries, etc.The setup function will only run once,
               after each power up or reset of the Arduino board.
*******************************************************************************************/
  void setup()
  {
    Serial.begin(9600);   // opens serial port, sets data rate to 9600 bps
  }
 
/*******************************************************************************************
  Function    : loop()
  Description : loops consecutively, allowing your program to change and
                respond.Use it to actively control the Arduino board.
*******************************************************************************************/
  void loop()
  {
      Receive_GPS_Data();              
      Serial.print("Latitude : ");
      Serial.println(lat);
      Serial.print("Longitude : ");
      Serial.println(lg);
      finish = 0;pos_cnt = 0;
 
  }
/*******************************************************************************************
  Function    : Receive_GPS_Data()
  Description : finding Latitudse and longitude from GPRMC message
 
*******************************************************************************************/
  void Receive_GPS_Data()
  {
    while(finish==0){
      while(Serial.available()>0){         // Check GPS data
        Gpsdata = Serial.read();
        flg = 1;
       if( Gpsdata=='$' && pos_cnt == 0)   // finding GPRMC header
         pos_cnt=1;
       if( Gpsdata=='G' && pos_cnt == 1)
         pos_cnt=2;
       if( Gpsdata=='P' && pos_cnt == 2)
         pos_cnt=3;
       if( Gpsdata=='R' && pos_cnt == 3)
         pos_cnt=4;
       if( Gpsdata=='M' && pos_cnt == 4)
         pos_cnt=5;
       if( Gpsdata=='C' && pos_cnt==5 )
         pos_cnt=6;
       if(pos_cnt==6 &&  Gpsdata ==','){   // count commas in message
         com_cnt++;
         flg=0;
       }
 
       if(com_cnt==3 && flg==1){
        lat[lat_cnt++] =  Gpsdata;         // latitude
        flg=0;
       }
 
       if(com_cnt==5 && flg==1){
         lg[log_cnt++] =  Gpsdata;         // Longitude
         flg=0;
       }
 
       if( Gpsdata == '*' && com_cnt >= 5){
         com_cnt = 0;                      // end of GPRMC message
         lat_cnt = 0;
         log_cnt = 0;
         flg     = 0;
         finish  = 1;
 
      }
    }
 }
}
