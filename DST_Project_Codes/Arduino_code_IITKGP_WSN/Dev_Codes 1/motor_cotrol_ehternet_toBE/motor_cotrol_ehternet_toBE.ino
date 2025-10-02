    #include <SPI.h>
    #include "Ethernet.h"

    // Ethernet library configuration
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //physical mac address
    byte ip[] = { 192, 168, 1, 123 };                     // ip in lan
    byte gateway[] = { 192, 168, 1, 1 };                  // internet access via router
    EthernetServer server(80);                            //server port

    // HMTL processing variables
    String readString = "";             //string to get incoming data

    char c;
    char buffer[10];
    int dataLength =0;

    // Buffer containing processed hmtl data
    char data[50];
    char datamotor[5];
    int  index=3;

    // Arduino controls variables
      // Motor control Ardumoto B  PWM D5  Direction D8
       int motor_PINpwm = 5;
       int motor_PINdir = 8;
       int motor_VALpwm = 0;
       int motor_VALdir = LOW;
       
       int LEDpinlow = 6;     //simulated ground
       int LEDpin =    7;
       int LEDval=0;
       


    // Setup function
    void setup()
    {
        Ethernet.begin(mac, ip, gateway);   //start Ethernet
        Serial.begin(9600);
       
        // Arduino controls initialization
        pinMode(motor_PINdir, OUTPUT);
        pinMode(LEDpinlow, OUTPUT);
        digitalWrite(LEDpinlow, LOW);
        pinMode(LEDpin, OUTPUT);
    }


    // Loop function
    void loop()
    {
      EthernetClient client = server.available();
      if (client)
      {
         while (client.connected())
         {
            while (client.available())  // Receive client data
            {
             
              Serial.print(".");
              c = client.read();   //read char by char HTTP request
              readString +=c;
              //Serial.print(c);          //output chars to serial port
             
              // If first request upon connexion, the 3 first characters will be "GET"
              // If "GET" is caught, skip the request info
              if( readString.equals("GET"))
              {
                Serial.println("");
                Serial.println("GET caught, skipping request and printing HTML");
                break;
              }
             
              // Otherwise, if the request contains data,
              // the first characters will be "POST"
              // We then skip the request header and this "if" becomes our main function
              if( readString.equals("POST"))
              {
                Serial.println("");
                Serial.println("POST caught, skipping header and acquiring DATA");
                // 320 is arbitrary. The actual length that has to be skipped depends on
                // several user settings ( browser, language, addons...)
                // the skipped length has not to be too long to skip relevant data
                // and not to short to waste computing time
                for(int i=0; i<320; i++)
                {
                  c = client.read();
                  //Serial.print(c); // UNCOMMENT FOR DEBUG
                }
                //Searches for "Length: "
                while(c  != 'L')
                {             
                  c = client.read();
                  //Serial.print(c); // UNCOMMENT FOR DEBUG
                }
                // Skip "Length: "
                for (int i=0; i<7; i++)
                {
                  c = client.read();
                  //Serial.print(c); // UNCOMMENT FOR DEBUG
                }
               
                // Read the data package length
                readString="";
                c = client.read();
               
                while(c != '\n')
                {
                  readString += c;
                  //Serial.print(c);
                  c = client.read();   
                }
               
                // convert data read from String to int
                readString.toCharArray(buffer, readString.length());
                dataLength = atoi(buffer);
                Serial.println("");
                Serial.print("dataLength: ");
                Serial.println(dataLength);
               
                // gets DATA
                client.read(); // skips additional newline
                client.read();
                for (int i=0; i<dataLength; i++)
                {
                  data[i] = client.read();
                }
               
                Serial.println("");
                Serial.print("data: ");
                Serial.println(data);
               
                readString ="";
               
               
       // Handle data
                // Sort cases depending on the first 2 characters
               
                char tempbuff[3];
                tempbuff[0] = data[0];
                tempbuff[1] = data[1];
                switch (atoi(tempbuff)) //convert to int
                {
                  case 1:
     
                  Serial.println("In case 1");
                 
                  while(data[index] !='&')
                  { datamotor[index-3] = data[index];
                    index++; }
                  motor_VALpwm = atoi(datamotor);
                  if(motor_VALpwm > 255){motor_VALpwm = 255;}
                  if(motor_VALpwm < 0){motor_VALpwm = 0;}
                  Serial.println("PWM value: ");
                  Serial.println(motor_VALpwm);
                  index++;
                  if( atoi(&data[index]) == 0)
                  { motor_VALdir = LOW; }
                  if( atoi(&data[index]) == 1)
                  { motor_VALdir = HIGH; }
                  Serial.println("PWM direction: ");
                  Serial.println(motor_VALdir);
                 
                  digitalWrite(motor_PINdir, motor_VALdir);
                  analogWrite(motor_PINpwm, motor_VALpwm);
                 
                  break;
                 
                 
                 
                  case 2:
                  Serial.println("In case 2");
                  if (LEDval == 0)
                  {LEDval =1;}
                  else
                  {LEDval=0;}
                  digitalWrite(LEDpin, LEDval);
                  Serial.print("LED status: ");
                  Serial.println(LEDval);
                 
                  break;
                 
                }
                Serial.println("Out of switch");
               
              }       
            }
           
           
            // HTML CODE
            client.println("<!DOCTYPE html>");
            client.println("<html>");
              client.println("<body background=\"http://img256.imageshack.us/img256/5867/52577012.jpg\"</body>");
              client.println("<font color=white><h1 align=center>Jayconsystems Arduino Ethernet presentation</font></h1>");
              client.println("<P style=text-align:center><img src=\"http://img52.imageshack.us/img52/4299/logovgn.png\" width=600 height=150 /></P>");
              client.println("<hr />");
              client.println("<hr />");
                client.println("<form name=input method=post>");
                           
                    String tempSTR = "<font color=white> Motor speed: <input type=text name=01 value=";
                    tempSTR += motor_VALpwm;
                    tempSTR += " maxlength=3 /><br />";
                  client.println(tempSTR);
                  client.println("<input type=submit name=0 value=\"DIRECTION 1\" />");
                  client.println("<input type=submit name=1 value=\"DIRECTION 2\" />");
                client.println("</form> <br /> <br />");
               
                client.println("<dd><dd><dd>");
               
                if(LEDval == 1)
                  {
                    client.println("<img src=\"http://img688.imageshack.us/img688/4420/greencw.png\" width=20 height=20 />");
                  }
                  if(LEDval == 0)
                  {
                    client.println("<img src=\"http://img507.imageshack.us/img507/8505/blackko.png\" width=20 height=20 />");
                  }
           
                client.println("<form name=input method=post>");
                  client.println("<input type=hidden name=02 />");
                  client.println("<input type=submit value=\"      LED      \"  />");
                 
                 
                 
                client.println("</form> <br />");
               
              client.println("</body>");
            client.println("</html>");
           
            Serial.println("__________");
            delay(100);
           
            client.stop();
           
         }
      }
     
     
     
       
      // Reinitializing variables
      readString ="";          // Reinitialize String
      for (int i=0; i<10; i++)
        {buffer[i] = '\0';}
      for (int i=0; i<50; i++)
        {data[i] = '\0';}
      for (int i=0; i<5; i++)
        {datamotor[i] = '\0';}
      dataLength =0;
      index = 3;
     
    }
