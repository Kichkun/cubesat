/*
  Arduino Webserver using ESP8266
  Displays everything in dweet. 
  https://freeboard.io/board/5a5a12071b13f0b619000005
  https://freeboard.io/board/tJCRL3
  https://dweet.io/follow/kish_graphs
  For UNO users,use Softwareserial library to connect ESP8266 with
  Arduino UNO 
  modified MAR 2018
  By Anastasia Kishkun
 */
#include <TroykaDHT.h>
#include<SoftwareSerial.h>
#include "ESP8266.h"
#include <math.h>
#include <TroykaIMU.h>
#include <Wire.h>
#define DEBUG true
int connectionId;
Accelerometer accel;
Gyroscope gyro;
DHT dht(2, DHT11);
SoftwareSerial Serial1(8,9);
ESP8266 wifi(Serial1);
String name = "kish_graphs";
#define SSID     "edc"
#define PASSWORD "24242424"
int schet = 0;
Compass compass;
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
 
void setup()
{
  
  Serial.begin(9600);    ///////For Serial monitor 
  Serial1.begin(9600); ///////ESP Baud rate
  accel.begin();
  compass.begin();
  dht.begin();
  gyro.begin();
  gyro.setRange(RANGE_250DPS);
  compass.setRange(RANGE_4_GAUSS);
  accel.setRange(RANGE_2G);
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  /*sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
  */
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.println("https://dweet.io/follow/" + name);
  } else {
    Serial.println("Wi-Fi connection error");
  }
}
void sendTab(){
  
      String web1 = "<TABLE>";
      web1+= "<TR><TD>";
      web1+=String(schet);
      web1+= "</TD>";    
      web1+= "<TD>"+String(float(analogRead(A0)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A1)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A2)), 3)+"</TD>";
      web1+= "<TD>"+String(float(analogRead(A3)), 3)+"</TD>";
            
      web1+= "<TD>"+String(accel.readAX())+"</TD>";
      web1+= "<TD>"+String(accel.readAY())+"</TD>";
      web1+= "<TD>"+String(accel.readAZ())+"</TD>";
      delay(200);
      web1+= "<TD>"+String(compass.readAzimut())+"</TD>";
      web1+= "<TD>"+String(compass.readX())+"</TD>";
      web1+= "<TD>"+String(compass.readY())+"</TD>";
      web1+= "<TD>"+String(compass.readZ())+"</TD>";
      web1+= "</TR>";
      web1+= "</TABLE>";
      espsend(web1);
      schet++;
  }
  
void dataS(){
  
      Serial.println(analogRead(A0));
      Serial.println(analogRead(A1));
      Serial.println(analogRead(A2));
      Serial.println(analogRead(A3));
      Serial.println(accel.readAX());
      Serial.println(accel.readAY());
      Serial.println(accel.readAZ());
      Serial.println(compass.readAzimut());
      Serial.println(compass.readX());
      Serial.println(compass.readY());
      Serial.println(compass.readZ());
  }
void loop() {
  dht.read();
  if (wifi.createTCP("www.dweet.io", 80)) {
    String data = "GET /dweet/for/" + name + "?";
    data += "Light1=" + String(analogRead(A0)) + "&Light2=" + String(analogRead(A1)) + "&Light3=" + String(analogRead(A2))+ "&Light4=" + String(analogRead(A3));
    data +=  "&AX=" + String(accel.readAX())+"&AY=" + String(accel.readAY())+"&AZ=" + String(accel.readAZ());
    data +=  "&CompasTotal=" + String(compass.readAzimut())+"&Compas_AX=" + String(compass.readX())+"&Compas_AY=" + String(compass.readY())+"&Compas_AZ=" + String(compass.readZ());
    data +="&Temperature=" + String(dht.getTemperatureC()) +"&Humidity=" + String(dht.getHumidity());
    data +=  "&GyroX=" + String( gyro.readDegPerSecX())+"&GyroY=" + String( gyro.readDegPerSecY())+"&GyroZ=" + String( gyro.readDegPerSecZ());
    data += " HTTP/1.1\r\n";
    data += "Host: dweet.io\r\n\r\n";
    wifi.send((const uint8_t*)data.c_str(), data.length());
    wifi.releaseTCP();
  } else {
    Serial.println("create TCP error");
  }
  delay(1000);
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
