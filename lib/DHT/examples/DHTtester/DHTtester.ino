// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <SPI.h>

#include <RF24.h> // https://github.com/maniacbug/RF24
#include "DHT.h"

#define DHTPIN 8    // what pin we're connected to


#define DHTTYPE DHT11   // DHT 22  (AM2302)
const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"


DHT dht(DHTPIN, DHTTYPE);
RF24 radio(9, 10); // CE, CSN


 int data[2];

 void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
 radio.begin();
  delay(2);
 radio.setChannel(9); // канал (0-127)
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
 radio.openWritingPipe(pipe); // открываем трубу на передачу.
  delay(1000);
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);

}

void loop() {

  delay(2000);


  dhtread();
  radio.write(&data, sizeof(data)); // отправляем данные и указываем сколько байт пакет

  Serial.println(data[0]);
   Serial.println(data[1]);

   if (data[0] < 22){
    digitalWrite(5, 0);

   }
   if (data[0] > 24){
    digitalWrite(5, 1);

   }

}




// ..функции
// датчик:
void dhtread(){


  int h = dht.readHumidity();

  int t = dht.readTemperature();



  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }




 data[1] = h;
 data[0] = t;



}

// radio

