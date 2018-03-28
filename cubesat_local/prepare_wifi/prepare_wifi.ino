#include <SoftwareSerial.h>
SoftwareSerial SIM800(8, 9);        // 8 - RX Arduino (TX SIM800L), 9 - TX Arduino (RX SIM800L)
void setup() {
  Serial.begin(115200);               // Скорость обмена данными с компьютером
  //Serial.println("Start!");
  SIM800.begin(115200);               // Скорость обмена данными с модемом
  SIM800.println("AT+CIOBAUD=9600,8,1,0,0");
  delay(200);
  SIM800.begin(9600);
  SIM800.println("AT");
  delay(200);
  SIM800.println("ATI");
  delay(200);
  SIM800.println("AT+CWMODE_DEF=2");
  delay(200);
  SIM800.println("AT+CWSAP=\"esp_2\",\"1234567890\",5,3");
}
 
void loop() {
  //SIM800.println("AT");
  if (SIM800.available())           // Ожидаем прихода данных (ответа) от модема...
    Serial.write(SIM800.read());    // ...и выводим их в Serial
  if (Serial.available())           // Ожидаем команды по Serial...
    SIM800.write(Serial.read());    // ...и отправляем полученную команду модему
}
