// библиотека для работы с SPI
#include <SPI.h>
// библиотека для работы с SD-картами
#include <SD.h>
// библиотека для работы с датчиком DHT11
#include <TroykaDHT.h>
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <TroykaIMU.h>

Gyroscope gyro;
// создаём объект для работы с акселерометром
Accelerometer accel;
// создаём объект для работы с компасом
Compass compass;
// создаём объект для работы с барометром
Barometer barometer;
// создаём объект класса DHT11 и передаём номер пина к которому подключён датчик
DHT dht(8, DHT11);
// даём разумное имя для CS пина microSD-карты
#define SD_CS_PIN  10
bool flag = false; 
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
String dataString = "";
void setup()
{
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  dht.begin();
  // выводим сообщение в Serial-порт о поиске карты памяти
  Serial.println("Initializing SD card...");
  // если microSD-карта не была обнаружена
  if (!SD.begin(SD_CS_PIN)) {
    // выводим сообщение об ошибке
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  } else {
    Serial.println("Card initialized.");

  }
  delay(1000);
    Serial.println("Begin init...");
  // инициализация гироскопа
  gyro.begin();
  // инициализация акселерометра
  accel.begin();
  // инициализация компаса
  compass.begin();
  // инициализация барометра
  barometer.begin();
  // калибровка компаса
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);
  // выводим сообщение об удачной инициализации
  Serial.println("Initialization completed");
  //Serial.println("Gyroscope\t\t\tAccelerometer\t\t\tCompass\t\tBarometer");
  namesSD();
  delay(1000);
}

void loop()
{
  if (!flag){
        namesSD();
        flag = true;
        delay(1000);
    }
    
  // переменная состояния датчика
  int check;
  // переменная покозания температуры
  int temperature;
  // переменная показания влажности
  int humidity;
  dataString = "";
  // мониторинг ошибок
  // считывание данных с датчика DHT11
  dht.read();
  switch (dht.getState()) {
    // всё OK
    case DHT_OK:
      // считываем показания влажности и температуры
      temperature = dht.getTemperatureC();
      humidity = dht.getHumidity();
      // формируем строку с результатами показаний датчика
      //dataString += "Temperature = ";
      dataString += String(temperature);
      dataString += "\t";
      //dataString += "Humidity = ";
      dataString += String(humidity);
      dataString += "\t";
      // выводим результаты в serial-порт
      Serial.print(dataString);
      // сохраняем на microSD
      break;
    // ошибка контрольной суммы
    case DHT_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    // превышение времени ожидания
    case DHT_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    case DHT_ERROR_NO_REPLY:
      Serial.println("Sensor not connected");
      break;
  }
  // ждём 1 секунду
  delay(1000);
  imuSD();
  delay(1000);
}
void namesSD(){
  String dataNames = "temperature C \t humidity % \t gyroX \t gyroY \t gyroZ \t accelX \t accelY \t accelZ \t accelTotal \t pressureIMU \t temperatureIMU \t altitude \t";
  // создаём файл для записи
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // если файл доступен для записи
  if (dataFile) {
    // сохраняем данные
    dataFile.println(dataNames);
    // закрываем файл
    dataFile.close();
    // выводим сообщение об удачной записи
    ////Serial.println("Save OK");
  } else {
    // если файл не доступен
    ////Serial.println("Error opening temperature.txt");
  }
} 

void imuSD(){
  float pressure = barometer.readPressureMillibars();
  // создаём переменную и присваиваем ей значения высоты над уровнем море
  float altitude = barometer.pressureToAltitudeMeters(pressure);
  // создаём переменную и присваиваем ей температуру окружающей среды
  float temperature = barometer.readTemperatureC();
  String dataIMU = "";
  dataIMU += dataString;
  dataIMU += String(gyro.readDegPerSecX());
  dataIMU += "\t";
  dataIMU += String(gyro.readDegPerSecY());
  dataIMU += "\t";
  dataIMU += String(gyro.readDegPerSecZ());
  dataIMU += "\t";
  dataIMU += String(accel.readAX());
  dataIMU += "\t";
  dataIMU += String(accel.readAY());
  dataIMU += "\t";  
  dataIMU += String(accel.readAZ());
  dataIMU += "\t";
  dataIMU += String(compass.readAzimut());
  dataIMU += "\t";
  dataIMU += String(pressure);
  dataIMU += "\t";
  dataIMU += String(temperature);
  dataIMU += "\t";
  dataIMU += String(altitude);
  dataIMU += "\t";
  // создаём файл для записи
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // если файл доступен для записи
  if (dataFile) {
    // сохраняем данные
    dataFile.println(dataIMU);
    // закрываем файл
    dataFile.close();
    // выводим сообщение об удачной записи
    ////Serial.println("Save OK");
  } else {
    // если файл не доступен
    ////Serial.println("Error opening temperature.txt");
  }
} 
void saveSD()
{
  // создаём файл для записи
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // если файл доступен для записи
  if (dataFile) {
    // сохраняем данные
    dataFile.println(dataString);
    // закрываем файл
    dataFile.close();
    // выводим сообщение об удачной записи
    Serial.println("Save OK");
  } else {
    // если файл не доступен
    Serial.println("Error opening temperature.txt");
  }
}
