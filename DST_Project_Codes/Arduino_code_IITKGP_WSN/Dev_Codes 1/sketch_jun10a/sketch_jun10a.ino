// Simple angle meter using ADXL335 accelerometer
//from electronicsblog.net/

#define ADC_ref 2.56

#define zero_x 1.569

#define zero_y 1.569

#define zero_z 1.569

#define sensitivity_x 0.3

#define sensitivity_y 0.3

#define sensitivity_z 0.3

unsigned int value_x;
unsigned int value_y;
unsigned int value_z;

float xv;
float yv;
float zv;

float angle_x;
float angle_y;
float angle_z;

void setup()   {

  //analogReference(INTERNAL2V56);

  Serial.begin(115200);

}

void loop() {

  value_x = analogRead(A0);
//  value_y = analogRead(1);
//  value_z = analogRead(2);
Serial.print (value_x);
  xv=(value_x/1024.0*ADC_ref-zero_x)/sensitivity_x;
//  Serial.print (xv);
  /*Serial.print ("x= ");
  Serial.print (xv);
  Serial.print(" g ");

  yv=(value_y/1024.0*ADC_ref-zero_y)/sensitivity_y;

  Serial.print ("y= ");
  Serial.print (yv);
  Serial.print(" g ");

  zv=(value_z/1024.0*ADC_ref-zero_z)/sensitivity_z;

  Serial.print ("z= ");
  Serial.print (zv);
  Serial.print(" g ");

  Serial.print("\n");

  */
  delay(10);
}


