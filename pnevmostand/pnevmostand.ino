#include <Wire.h> 
#include <TroykaIMU.h> 
#include <Multiservo.h> 

Multiservo S1; 
Multiservo S2; 
int incomingByte=0; 
#define BETA 0.22 
Madgwick filter; 
Accelerometer accel; 
Gyroscope gyro; 
Compass compass; 
 
float gx, gy, gz, ax, ay, az, mx, my, mz; 
float yaw, pitch, roll; 
float fps = 100; 

const double compassCalibrationBias[3] = { 
524.21, 
3352.214, 
-1402.236 
}; 

const double compassCalibrationMatrix[3][3] = { 
{1.757, 0.04, -0.028}, 
{0.008, 1.767, -0.016}, 
{-0.018, 0.077, 1.782} 
}; 

double RIGHT_percent, LEFT_percent;
double TIME = 300;

void setup() 
{ 
  Serial.begin(115200); 
  Serial.println("Begin init..."); 
  accel.begin(); 
  gyro.begin(); 
  compass.begin(); 
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias); 
  Serial.println("Initialization completed"); 
  S1.attach(12); 
  S2.attach(5); 
  pinMode(2, INPUT); 
  pinMode(3, INPUT); 
  pinMode(4, INPUT); 
  pinMode(5, INPUT); 
  digitalWrite(2, HIGH); 
  digitalWrite(3, HIGH); 
  digitalWrite(4, HIGH); 
  digitalWrite(5, HIGH); 
  int RIGHT_percent = 0;
  int LEFT_percent = 0;
  
  delay(60000);
} 

void loop() 
{ 

  LVC();
  delay(500);
  //Serial.println(String(digitalRead(2))+" "+String(digitalRead(3))+" "+String(digitalRead(4)) + " "+String(digitalRead(5)));
  accel.readGXYZ(&ax, &ay, &az); 
  gyro.readRadPerSecXYZ(&gx, &gy, &gz); 
  compass.readCalibrateGaussXYZ(&mx, &my, &mz); 
  filter.setKoeff(fps, BETA); 
  filter.update(gx, gy, gz, ax, ay, az, mx, my, mz); 
  yaw = filter.getYawDeg(); 
  LVO();
  delay(500);
  
  
  //right_set(100);
/*
  for(int i = 1; i <= 100; i++)
  {
    right_set(i);
    delay(10);
  }
  delay(500);
  for(int i = 100; i >= 1; i--)
  {
    right_set(i);
    delay(10);
  }
*/


} 

void right_set(double percent)
{
    S1.write((percent > RIGHT_percent)?0:180);
    delay(int(TIME*abs(RIGHT_percent - percent)/100));
    S1.write(90);
    RIGHT_percent = percent; 
}
void left_set(double percent)
{
    S2.write((percent > LEFT_percent)?0:180);
    delay(int(TIME*abs(LEFT_percent - percent)/100));
    S2.write(90);
    LEFT_percent = percent; 
}
bool RVO() 
{ 
  
  if (digitalRead(2) == 1 && digitalRead(3) == 0) 
  {
    S1.write(90); 
  }
  if (digitalRead(2) == 0 && digitalRead(3) == 1)
  {
    S1.write(0);
  }
} 

void RVC() 
{ 
  if (digitalRead(2) == 1 && digitalRead(3) == 0)
  { 
    S1.write(180);
  }
  if (digitalRead(2) == 0 && digitalRead(3) == 1) 
  {
    S1.write(90); 
  }
} 

void LVO() 
{ 
  if (digitalRead(4) == 1 && digitalRead(5) == 1)
  { 
    S2.write(0);
  }
  if (digitalRead(4) == 1 && digitalRead(5) == 0)
  { 
    S2.write(0);
  }
  if (digitalRead(4) == 0 && digitalRead(5) == 1)
  {
    S2.write(90);
  }
} 

void LVC() 
{ 
  if (digitalRead(4) == 1 && digitalRead(5) == 0) 
  {
    S2.write(90); 
  }
  if (digitalRead(4) == 0 && digitalRead(5) == 1)
  {
    S2.write(180);
  }
  if (digitalRead(4) == 1 && digitalRead(5) == 1)
  {
    S2.write(180);
  }
} 
