#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Servo.h"
#include "SAC21.h"

//Fonctions de configurations
void setup() {
  lcd.begin(16, 2);
  servo.attach(servoPin);
  Configure_US(ultrasons);
  coef_Matrix(Coord_US);
}
//Fonction principale
int main() {
  setup();
  while (1) {
    static uint32_t lastTime = 0;
    uint32_t currenttime = millis();
    //Execution tous les delayTime
    if (millis() - lastTime >= delayTime) {
      Init();
      lastTime = currenttime;                                   
    }
  }
  return 0;
}
