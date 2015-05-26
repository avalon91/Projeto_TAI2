/*

Arduinos com módulo RF e relé. Porta 8 conectada no SINAL do relé e
5V na porta A3.

*/

#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(9,10);

const uint64_t pipes[] = {0x78787878A3LL, 0x78787878E1LL};

int vai;
int veio[] = {0,0,0};

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.enableAckPayload();
  radio.setChannel(25);
  radio.setRetries(15,15);
  radio.setPayloadSize(16);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_16);
  attachInterrupt(0, radioReceive, FALLING);
}

void loop(){
  if(Serial.available() > 0){
    int c = Serial.read();
    if(c == 3){
      vai = 20;
      radio.openWritingPipe(pipes[0]);
      radio.startWrite(&vai, sizeof(vai));
    }
    if(c == 4){
      vai = 21;
      radio.openWritingPipe(pipes[0]);
      radio.startWrite(&vai, sizeof(vai));
    }
    if(c == 1){
      vai = 10;
      Serial.println("Foi");
      radio.openWritingPipe(pipes[1]);
      radio.startWrite(&vai, sizeof(vai));
    }
    if(c == 2){
      vai = 11;
      Serial.println("Foi");
      radio.openWritingPipe(pipes[1]);
      radio.startWrite(&vai, sizeof(vai));
    }
  }
}

void radioReceive(){
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);
  if(rx){
    //Serial.println("Chegou");
    radio.read(&veio, sizeof(veio));

    if(veio[0] == 1){
      Serial.println("Chegou de 1: ");
      if(veio[1] == 1){
        Serial.println("Lampada acesa");
      } else if(veio[1] == 0){
        Serial.println("Lampada apagada");
      }
      Serial.print("Leitura de A3: ");
      Serial.println(veio[2]);
    }

    if(veio[0] == 2){
      Serial.print("Chegou de 2: ");
      if(veio[1] == 1){
        Serial.println("Lampada acesa");
      } else if(veio[1] == 0){
        Serial.println("Lampada apagada");
      }
      Serial.print("Leitura de A3: ");
      Serial.println(veio[2]);
    }
  }
}