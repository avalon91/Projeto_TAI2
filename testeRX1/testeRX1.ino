/*

Arduinos com módulo RF e relé. Porta 8 conectada no SINAL do relé e
5V na porta A3.

*/

#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(9,10);

const uint64_t pipe = 0x78787878E1LL;

int foi;
int array[] = {1,0,0};

void setup(){
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.enableAckPayload();
  radio.setChannel(25);
  radio.setRetries(15,15);
  radio.setPayloadSize(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  attachInterrupt(0, radioReceive, FALLING);
}

void loop(){

}

void radioReceive(){
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);
  if(rx){
    //Serial.println("Chegou");
    radio.read(&foi, sizeof(int));

    if(foi == 10){
      digitalWrite(8, LOW);
      array[1] = 0;
      array[2] = analogRead(3);
      radio.writeAckPayload(1, &array, sizeof(array));
    }
    if(foi == 11){
      digitalWrite(8, HIGH);
      array[1] = 1;
      array[2] = analogRead(3);
      radio.writeAckPayload(1, &array, sizeof(array));
    }
  }
}
