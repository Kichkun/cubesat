#include <iarduino_OLED_txt.h>
iarduino_OLED_txt myOLED(0x78);                       
extern uint8_t SmallFontRus[];

int pin[6] = {2,3,4,5,7,9};//первые 4 - концевики, пятый - реле, шестой - пищалка
bool flag[4] = {false,false,false,false};
unsigned long duration[4];
int schet[4] = {0,0,0,0};
int fullSchet = 0;
void setup() {
   myOLED.begin();                                    
   myOLED.setFont(SmallFontRus);
   Serial.begin(9600);
   for (int i=0; i<4; i++)
      pinMode(pin[i], INPUT);
   pinMode(pin[4], OUTPUT);
   pinMode(pin[5], OUTPUT);
}


void kon(int i=0){
    duration[i] = pulseIn(pin[i], HIGH);
    if (duration[i]==0){
      if (flag[i]){     // итератор, пока концевик зажат
        schet[i]++;
        tone(pin[5], 40*schet[i], 100000);
        }
      else{       //момент срабатывания концевика. старт
        digitalWrite(pin[4], HIGH);
        tone(pin[5], 40*schet[i], 100000);
        flag[i] = true;
        schet[i]++;
        }
      }
     else{
      if (schet[i]>0){       //момент отжатия концевика. Можно управлять порогом(длительность нажатия) 
        Serial.println(1);
        digitalWrite(pin[4], LOW);
        tone(pin[5], 40*schet[i], 100000);
        fullSchet ++;
        }
      else{               // концевик не нажат
        Serial.println(0);
        noTone(pin[5]);
      }
        flag[i] = false;
        schet[i]=0;
     }
}
void loop() { 
  kon(0);
  kon(1);
  myOLED.clrScr();
  myOLED.print( "КОМАНДА ЗЕЛЕНЫХ:"  , OLED_C, 2);   
  myOLED.print(String(fullSchet), OLED_C, 4);   
  delay(100);
}



