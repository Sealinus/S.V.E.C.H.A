#include <VolAnalyzer.h>
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <buildTime.h>
#include <microDS3231.h>


#define microPin A0
#define outputPin 2
#define LED_PIN 30
#define LED_NUM 100
#define MP3_fb 42
#define leftRelayPin 28
#define rightRelayPin 29

#define IR_1    0xA2
#define IR_2    0x62
#define IR_3    0xE2
#define IR_4    0x22
#define IR_5    0x2
#define IR_6    0xC2
#define IR_7    0xE0
#define IR_8    0xA8
#define IR_9    0x90
#define IR_STAR 0x68
#define IR_0    0x98
#define IR_HASH 0xB0
#define IR_UP   0x18
#define IR_LEFT 0x10
#define IR_OK   0x38
#define IR_RIGHT 0x5A
#define IR_DOWN 0x4A


VolAnalyzer analyzer(microPin);
SoftwareSerial SSerial(40, 41);
DFPlayerMini_Fast myMP3;
CRGB leds[LED_NUM];
LiquidCrystal_I2C lcd(0x3f, 16, 2);
MicroDS3231 rtc;


void setup() {
  Serial.begin(9600);
  SSerial.begin(9600);
  myMP3.begin(SSerial);
  myMP3.volume(30);
  analogReference(EXTERNAL);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(50);
  rtc.setTime(COMPILE_TIME);
  lcd.init();
  lcd.backlight();
  pinMode(MP3_fb, INPUT);
  pinMode(leftRelayPin, OUTPUT);
  pinMode(rightRelayPin, OUTPUT);
  pinMode(46, OUTPUT);
  digitalWrite(46, LOW);
}

void loop() {
  static int nowPlay = 1;
  static int historyPlay;
  static int tracksCount = 10;

  //irComands();
  time();
//__LEDка__________________________________________________________________________________
  LEDline();
//__LEDка__________________________________________________________________________________

//__Микрофоша__________________________________________________________________________________
  microphone();
//__Микрофоша__________________________________________________________________________________

//__Музышка__________________________________________________________________________________
  if (digitalRead(MP3_fb)){
    nowPlay = random(1, tracksCount + 1);
    if (nowPlay != historyPlay){
      myMP3.play(nowPlay);
      historyPlay = nowPlay;
      delay(100);
    }
  }
//__Музышка__________________________________________________________________________________

 static int timer;
  while (millis() - timer >= 300000){
    if(random(0, 10) == 10){
      digitalWrite(44, LOW);
    }
    else{
      digitalWrite(44, HIGH);
    }
  }

}


void microphone(){
  static int curVol;
  static int tmr;
  static int noiseVal;
  if (analyzer.tick() && millis() - tmr >= 20) {
    curVol = analyzer.getVol();
    Serial.println(curVol);
    if (curVol >= 1){    
      if (noiseVal >= 4){
        noiseVal = 0;
        Serial.println("Be quiet plz");
        digitalWrite(46, HIGH);
           //если зафиксирован шум  Предположительно будет проигроваться запись "ТИИИИИИИИИИИИИИИХО"
      }
      else{
        noiseVal++;
        digitalWrite(46, LOW);
      }
      
    }
    else{
      noiseVal = 0;
      digitalWrite(46, LOW);
    }
    tmr = millis();
  }  
}

void LEDline(){
  static byte counter;
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(counter + i * 255 / LED_NUM);
  }
  counter++;
  FastLED.show();
  delay(5);
}

/*void switchRealy(int relaySide){
  static bool leftRelay;
  static bool rightRelay;
  switch (relaySide){
    case 0: 
      digitalWrite(leftRelayPin, !leftRelay);
      leftRelay = !leftRelay;
    break;
    case 1: 
      digitalWrite(rightRelayPin, !rightRelay);
      rightRelay = !rightRelay;
    break;
  }
}

/*void irComands(){
  String str;
  if (Serial.available() > 0) {
    str = Serial.readString();
    switch (str.toInt()) {
      case 1: myMP3.incVolume(); break;
      case 2: digitalWrite(leftRelayPin, HIGH); Serial.println("Turning on left relay"); break;
      case 3: digitalWrite(rightRelayPin, HIGH); Serial.println("Turning on right relay"); break;
      case 4: myMP3.decVolume(); break;
      case 5: digitalWrite(leftRelayPin, LOW); Serial.println("Turning off left relay"); break;
      case 6: digitalWrite(rightRelayPin, LOW); Serial.println("Turning off left relay"); break;
      case 7: break;
      case 8: break;
      case 9: break;
      case 10: break;
      case 0: break;
      case 11: break;
      case 12: break;
      case 13:  break;
      case 14:  break;
      case 15: break;
      case 16:  break;
    }
  }
}*/


void time(){
  static int hours;
  static int minutes;

  hours = rtc.getHours();
  minutes = rtc.getMinutes();

  if (hours >= 22 && minutes >= 50){
    lcd.setCursor(1, 0);
    lcd.print("time to sleep");
    lcd.setCursor(1, 1);
    lcd.print("@Danya Pavlov");
  }
}

