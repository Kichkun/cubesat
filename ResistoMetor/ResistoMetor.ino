// ОММЕТР (С)2013 ALEN Studio by Little_Red_Rat

// Омметр на осное ARDUINO
//
// Подключение делителя напряжения к ARDUINO
// Arduino 5V -> R1 10kOm -> Arduino Analog 0 -> R2 -> Arduino GND

int analogPin_0 = A0; // Анлоговый вход для считывания напряжения с делителя напряжения
int analogPin_1 = A1; 

float Vout = 0; // Переменная для хранения значения напряжения в средней точки делителя (0-5.0)

float R2_0 = 0; // Переменная для хранения значения резистора R2
float R2_1 = 0;
int R1_0 = 300;
int R1_1 = 180;
float V_in = 4.78;
void setup() 
{ 
Serial.begin(9600); // Подготовка Serial Monitor для вывода информации
} 

float mesure(float pin, int R1){
  Vout = (V_in / 1023.0) * analogRead(pin); // Вычисляем напряжение в средней точки делителя (0-5.0)
  R2_0 = R1/ ((V_in / Vout) - 1); // Вычисляем сопротивление R2 (10000 это значение R1 10 кОм) 
  Serial.print(" Voltage: "); // 
  Serial.print(Vout); // Напряжения в средней точки делителя (0-5.0) для справки
  Serial.print(" R2: "); // 
  Serial.print(R2_0); // Значение сопротивления R2
  return R2_0;
}
void loop() 
{ 
  float a = mesure(analogPin_0, R1_0);
  float b = mesure(analogPin_1, R1_1);
  Serial.println();
  delay(1000); // Пауза 1 сек
}
