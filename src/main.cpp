#include <Arduino.h>
#include "conf.hpp"

#include "model.hpp"
Data data;
Hard  hard(data);
unsigned long starttime = 0;
int interval = 3000;


void setup() {
  hard.eepromRead();
  delay(50);
  #include "pinsetup.hpp"
  Serial.begin(9600);
  hard.setupRadio();



}

void loop() {
  if (millis() - starttime >= interval)
  {
    hard.sensors();
    starttime = millis();
  hard.outSerial();
  }
    hard.Lisnener();

}
