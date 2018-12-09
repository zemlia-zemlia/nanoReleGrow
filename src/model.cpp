#include "conf.hpp"
#include "model.hpp"
#include <EEPROM.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24
#include <DHT.h>
const uint64_t pipe = 0xF0F1F2F3F4AA; // индитификатор передачи, "труба"
DHT sensor(DHTPIN, DHT11);
RF24 radio(4, 5); // CE, CSN
Hard::Hard(Data   _data){
data = _data;
}

bool Hard::logic(){
  // Serial.print("log-");
  // Serial.println((float)(data.temperS + data.delta_T));
  if (data.temper <= (float)(data.temperS - data.delta_T)){
    // Serial.print("t--");
    hardSwap(1, true);
  }
  if (data.temper >= (float)(data.temperS + data.delta_T)){
    hardSwap(1, false);
    // Serial.println(data.temper);
  }
  if (data.humi <= (float)(data.humiS - data.delta_H)){
    hardSwap(2, true);
  }
  if (data.humi >= (float)(data.humiS + data.delta_H)){
    hardSwap(2, false);
  }


return true;
}

bool Hard::setupRadio()
{
  radio.begin();
  delay(2);
  radio.setChannel(60);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  return true;
}
bool Hard::radioSendData()
{
  radio.stopListening();
  delay(50);
  radio.openWritingPipe(pipe);
  radio.write(&data, sizeof(data));
  delay(50);
  radio.startListening();
  return true;
}
bool Hard::eepromWrite(byte numB, byte val)
{

  EEPROM.write(numB, val);
  delay(100);
  return true;
}

bool Hard::eepromRead()
{
  data.temperS = float(EEPROM.read(0)); // temper
  data.delta_T = float(EEPROM.read(1)); // delta temper
  data.humiS = float(EEPROM.read(2)); // humi

  data.delta_H = float(EEPROM.read(3)); // delta H
  return true;
}
bool Hard::hardSwap(byte num){
 switch (num) {
  case 1: // battary
    if (data.Radiator) {
      hardSwap(1, false);
    }else hardSwap(1, true);
    break;
  case 2:
  if (data.Humi) {
    hardSwap(2, false);
  }else hardSwap(2, true);
    break;
  case 3:
  if (data.VentIn) {
    hardSwap(3, false);
  }else hardSwap(3, true);
    break;
  case 4:
  if (data.Pompa) {
    hardSwap(4, false);
  }else hardSwap(4, true);
    break;
 }
 return true;
}
bool Hard::hardSwap(byte num, bool state){
  switch (num) {
   case 1: // battary
    digitalWrite(battPIN, state);
    data.Radiator = state;
     break;
   case 2:
   digitalWrite(humPIN, !state);
   data.Humi = state;
     break;
   case 3:
   digitalWrite(ventinPIN, !state);
   data.VentIn = state;
     break;
   case 4:
   digitalWrite(pompaPIN, !state);
   data.Pompa = state;
     break;
  }
  return true;
}
bool Hard::outSerial()
{
      Serial.print(data.temper);
      Serial.print(";");
      Serial.print(data.humi);
      Serial.print(";");

      Serial.print(data.soil);
      Serial.print(";");
      Serial.print(data.temperS);
      Serial.print(";");

      Serial.print(data.delta_T);
      Serial.print(";");
      Serial.print(data.humiS);
      Serial.print(";");
      Serial.print(data.delta_H);
      Serial.print(";");
      Serial.print(data.Radiator);
      Serial.print(";");
      Serial.print(data.Humi);
      Serial.print(";");
      Serial.print(data.VentIn);
      Serial.print(";");
      Serial.println(data.Pompa);
}
bool Hard::sensors(){
  int soil = analogRead(HUMI_SOIL);
  data.soil = map(soil, 1023, 0, 0, 99);
  float temper = sensor.readTemperature();
  if (temper != 0){
  data.temper = temper ;
  }
  float humi = sensor.readHumidity();
  if (humi != 0){
  data.humi = humi;
  }
  return true;
}
bool Hard::Lisnener(){
  byte val[2];
  if (radio.available())
   {
     // проверяем не пришло ли чего в буфер.
     radio.read(val, sizeof(val)); // читаем данные и указываем сколько байт читать
// Serial.print("++");
     // Serial.println(val[0]);

     if (val[0] == 1)
     {
       Hard::radioSendData();
     }
      if (val[0] == 2) // записать в EEPROM !!!  temperS
     {
       // Serial.print("-------");
       // Serial.println(val[1]);
       Hard::eepromWrite(0, val[1]);
       delay(100);
       Hard::eepromRead();
       delay(100);
     }
     if (val[0] == 3) // записать в EEPROM !!!  delta_T
    {
      // Serial.print("-------");
      // Serial.println(val[1]);
      Hard::eepromWrite(1, val[1]);
      delay(100);
      Hard::eepromRead();
      delay(100);
    }
    if (val[0] == 4) // записать в EEPROM !!!  humiS
   {
     // Serial.print("-------");
     // Serial.println(val[1]);
     Hard::eepromWrite(2, val[1]);
     delay(100);
     Hard::eepromRead();
     delay(100);
   }
   if (val[0] == 5) // записать в EEPROM !!!  delta_H
  {
    // Serial.print("-------");
    // Serial.println(val[1]);
    Hard::eepromWrite(3, val[1]);
    delay(100);
    Hard::eepromRead();
    delay(100);
  }
  if (val[0] == 6) // переключить радиатор
  {
   // Serial.print("rad");
   hardSwap(1);
   // Serial.print(data.Radiator);
  }
  if (val[0] == 7) // переключить Humi
  {
   // Serial.print("humi");
   hardSwap(2);
   // Serial.print(data.Humi);
  }
  if (val[0] == 8) // переключить радиатор
  {
   // Serial.print("vent");
   hardSwap(3);
   // Serial.print(data.VentIn);
  }
  if (val[0] == 9) // переключить радиатор
  {
   // Serial.print("pomp");
   hardSwap(4);
   // Serial.print(data.Pompa);
  }



   }
   return true;
}
