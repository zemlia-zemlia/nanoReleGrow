#ifndef MODEL_HPP
#define MODEL_HPP
#include <Arduino.h>


struct Data{
  float temper = 0;
  float humi = 0;
  byte soil = 0;
  byte temperS = 0;
  byte delta_T =0;
  byte humiS = 0;
  byte delta_H = 0;
  bool Humi = false;
  bool Radiator = false;
  bool VentIn = false;
  bool Pompa = false;
};

struct Hard{
  Hard(Data  _data);
  Data  data;
  bool hardSwap(byte num);
  bool hardSwap(byte num, bool state);
  bool setupRadio();
  bool radioSendData();
  bool eepromWrite(byte numB, byte val);
  bool eepromRead();
  bool outSerial();
  bool sensors();
  bool Lisnener();
  bool logic();


};



#endif
