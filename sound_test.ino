#include <SoftwareSerial.h>               // Подключаем библиотеку SoftwareSerial
#include <DFPlayerMini_Fast.h>            // Подключаем библиотеку DFPlayerMini_Fast

SoftwareSerial mySerial(40, 41);          // Указываем к какими портам подключен DFPlayer
DFPlayerMini_Fast myMP3;                  // Создаем объект 

void setup()
{
  mySerial.begin(9600);                   // Открываем последовательную связь
  myMP3.begin(mySerial);                  // инициализация
  myMP3.volume(20); 
 // myMP3.play(1);                    // Указываем громкость (0-30)
  pinMode(42, INPUT);
}
void loop()
{
  static int nowPlay = 1;
  static int tmr;
 /* if (!(myMP3.isPlaying())){
    myMP3.play(nowPlay);
    nowPlay++;
    delay(3000);
    //tmr = millis();
  }
  if (nowPlay >= 4){
    nowPlay = 1;
  }*/
  
  //Serial.println(digitalRead(42));

  if (digitalRead(42)){
    myMP3.play(nowPlay);
    nowPlay++;
    delay(100);
    //tmr = millis();
  }
  if (nowPlay >= 5){
    nowPlay = 1;
  }
  
                            // Воспроизведение трека 0001
                     // Пауза 10 секунд
} 