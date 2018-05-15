#include "Buzzer.h"
#include <Arduino.h>

void toneShort(){
  tone(BUZZER, NOTE_C7, 150);
  delay(150);
  noTone(BUZZER);
}

void toneLong(){
  tone(BUZZER, NOTE_C7, 550);
  delay(550);
  noTone(BUZZER);
}
