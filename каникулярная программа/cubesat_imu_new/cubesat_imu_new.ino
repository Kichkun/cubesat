/*
  Arduino Webserver using ESP8266
  Displays temperature in a webpage
  For UNO users,use Softserial library to connect ESP8266 with
  Arduino UNO 
  modified JAN 2018
  By Anastasia Kishkun
 */
#include<SoftwareSerial.h>
#include <TroykaDHT.h>
#include <TroykaIMU.h>
#include <Wire.h>
#define DEBUG true
int connectionId;
Accelerometer accel;
SoftwareSerial Serial1(13,A0);
int schet = 0;
Compass compass;
Gyroscope gyro;
Barometer barometer;
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
DHT dht(9, DHT11);
void setup()
{
  
  Serial.begin(9600);    ///////For Serial monitor 
  Serial1.begin(9600); ///////ESP Baud rate
  accel.begin();
  dht.begin();
  compass.begin();
  gyro.begin();
  gyro.setRange(RANGE_250DPS);

  compass.setRange(RANGE_16_GAUSS);
  accel.setRange(RANGE_8G);
  barometer.begin();
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
void check(){
      dht.read();
      Serial.print("L1: " + String(analogRead(A1)));
      Serial.print(" L2: " + String(analogRead(A3)));
      Serial.print(" L3: " + String(analogRead(A4)));
      Serial.print(" L4: " + String(analogRead(A5)));
      
      Serial.print(" AX: " + String(accel.readAX())); 
      Serial.print(" AY: " + String(accel.readAX())); 
      Serial.print(" AZ: " + String(accel.readAX()));   
      delay(200);
      Serial.print(" CT: " + String(compass.readAzimut())); 
      Serial.print(" CX: " + String(compass.readX())); 
      Serial.print(" CY: " + String(compass.readY())); 
      Serial.print(" CZ: " + String(compass.readZ())); 
      delay(200);
      Serial.print(" GX: " + String(gyro.readDegPerSecX())); 
      Serial.print(" GY: " + String(gyro.readDegPerSecY())); 
      Serial.print(" GZ: " + String(gyro.readDegPerSecZ()));
      delay(200);
      Serial.print(" presure: " + String(barometer.readPressureMillibars())); 
      Serial.print(" temp: " + String(barometer.readTemperatureC())); 
      Serial.print(" alt: " + String(barometer.pressureToAltitudeMeters(barometer.readPressureMillibars())));
      delay(200);
      Serial.print(" H: " + String(dht.getHumidity())); 
      Serial.print(" T: " + String(dht.getTemperatureC())); 
      Serial.println();
}
void sendNames(bool li = true, bool ac = true, bool gy = false, bool ma = true, bool dh = false,bool br = false){
      String webpage = "<h1>Cubesat Sirius Web Monitor</h1>";
      espsend(webpage);
      webpage= "<TABLE>";
      webpage+= "<TR>";
      webpage+= "<TD>No</TD>";
      if (li){
      webpage+= "<TD>Lig1 </TD>";
      webpage+= "<TD>Lig2 </TD>";
      webpage+= "<TD>Lig3 </TD>";
      webpage+= "<TD>Lig4 </TD>";
      }
      if (ac){
      webpage+= "<TD>AccX </TD>";
      webpage+= "<TD>AccY </TD>";
      webpage+= "<TD>AccZ </TD>";
     }
     if (ma){
      webpage+= "<TD>TotaM </TD>";
      webpage+= "<TD>MagX </TD>";
      webpage+= "<TD>MagY </TD>";
      webpage+= "<TD>MagZ </TD>";
     }
     if (gy){
      webpage+= "<TD>GyrX </TD>";
      webpage+= "<TD>GyrY </TD>";
      webpage+= "<TD>GyrZ </TD>";
      }
      if (dh){
      webpage+= "<TD>Hum </TD>";
      webpage+= "<TD>Temp </TD>";
      }
      if (br){
      webpage+= "<TD>pres</TD>";
      webpage+= "<TD>Temp </TD>";
      webpage+= "<TD>Alt</TD>";
      }
      webpage+= "</TR>";
      webpage+= "</TABLE>";
      espsend(webpage);
}
void sendTab(bool li = true, bool ac = true, bool gy = false, bool ma = true, bool dh = false, bool br = false){
      dht.read();
      String web1 = "<TABLE>";
      web1+= "<TR><TD>";
      web1+=String(schet);
      web1+= "</TD>";    
      if (li){
      web1+= "<TD>"+String(float(analogRead(A1)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A3)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A4)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A5)), 3)+"</TD>";
      }
      if (ac){  
      web1+= "<TD>"+String(accel.readAX())+"</TD>";
      web1+= "<TD>"+String(accel.readAY())+"</TD>";
      web1+= "<TD>"+String(accel.readAZ())+"</TD>";
      delay(200);
      }
      if (ma){
      web1+= "<TD>"+String(compass.readAzimut())+"</TD>";
      web1+= "<TD>"+String(compass.readX())+"</TD>";
      web1+= "<TD>"+String(compass.readY())+"</TD>";
      web1+= "<TD>"+String(compass.readZ())+"</TD>";
      delay(200);
      }
      if (gy){
      web1+= "<TD>"+String(gyro.readDegPerSecX())+"</TD>";
      web1+= "<TD>"+String(gyro.readDegPerSecY())+"</TD>";
      web1+= "<TD>"+String(gyro.readDegPerSecZ())+"</TD>";
      delay(200);
      }
      if (dh)
      {
      web1+= "<TD>"+String(dht.getHumidity())+"</TD>";
      web1+= "<TD>"+String(dht.getTemperatureC())+"</TD>";
      }
      if (br)
      {
      web1+= "<TD>"+String(barometer.readPressureMillibars())+"</TD>";
      web1+= "<TD>"+String(barometer.readTemperatureC())+"</TD>";
      web1+= "<TD>"+String(barometer.pressureToAltitudeMeters(barometer.readPressureMillibars()))+"</TD>";
      }
      web1+= "</TR>";
      web1+= "</TABLE>";
      espsend(web1);
      schet++;
  }
void loop()
{
  check();
  if(Serial1.available())
  {
    /////////////////////Recieving from web browser to toggle led
    if(Serial1.find("+IPD,"))
    {
     delay(300);
     connectionId = Serial1.read()-48;
     if(Serial1.find("pin="))
     { 
       Serial.println("recieving data from web browser");
       int pinNumber = (Serial1.read()-48)*10; 
       pinNumber += (Serial1.read()-48); 
       digitalWrite(pinNumber, !digitalRead(pinNumber));
     }
     else{
     sendNames(true,false,false,false,true,true); // свет, акселерометр, магнитометр, гироскоп, температура и влажность
     }
    /////////////////////Sending data to browser
    for (int i=1; i<3000;i++)
     sendTab(true,false,false,false,true,true);
     String closeCommand = "AT+CIPCLOSE=";  ////////////////close the socket connection////esp command 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     sendData(closeCommand,3000,DEBUG);
    }
  }
}
 
  //////////////////////////////sends data from ESP to webpage///////////////////////////
 
 void espsend(String d)
         {
             String cipSend = " AT+CIPSEND=";
             cipSend += connectionId; 
             cipSend += ",";
             cipSend +=d.length();
             cipSend +="\r\n";
             sendData(cipSend,1000,DEBUG);
             sendData(d,1000,DEBUG); 
         }

//////////////gets the data from esp and displays in serial monitor///////////////////////         
String sendData(String command, const int timeout, boolean debug)
            {
                String response = "";
                Serial1.print(command);
                long int time = millis();
                while( (time+timeout) > millis())
                {
                   while(Serial1.available())
                      {
                         char c = Serial1.read(); // read the next character.
                         response+=c;
                      }  
                }
                
                if(debug)
                     {
                     Serial.print(response); //displays the esp response messages in arduino Serial monitor
                     }
                return response;
            }
