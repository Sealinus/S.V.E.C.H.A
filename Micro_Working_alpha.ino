#include "VolAnalyzer.h"
#include "GyverPID.h"

#define microPin A0
#define outputPin 2


VolAnalyzer analyzer(microPin);  // микрофон на А0

int tmr = 0;
int tmr_led = 0;

void setup() {
  Serial.begin(9600);

  // переключаем на внешнее опорное
  // его подключаем к 3.3V
  analogReference(EXTERNAL);
}

void loop() {
  static int curVol;
  // если анализ завершён
  if (analyzer.tick() && millis() - tmr >= 20) {
    curVol = analyzer.getVol();
    Serial.println(curVol);
    if (curVol >= 1){
      
      digitalWrite(outputPin, HIGH);
    }
    else{
      if (millis() - tmr_led >= 5000)
      digitalWrite(outputPin, LOW);
    }
    tmr = millis();
  }
}