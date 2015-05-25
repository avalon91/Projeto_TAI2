#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

RF24 radio(9,10);

const uint64_t pipe = 0x78787878E1LL;

int foi;
int array[] = {1,10};

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
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  attachInterrupt(0, radioReceive, FALLING);
  //pinMode(3, OUTPUT);
  //digitalWrite(3, LOW);
}

void loop(){
  
}

void radioReceive(){
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);
  if(rx){
    Serial.println("Chegou");
    radio.read(&foi, sizeof(int));
    if(foi == 10){
      array[1] = foi+1;
      radio.writeAckPayload(1, &array, sizeof(array));
    }
    /*if(go == '3'){
      Serial.println("3");
      digitalWrite(3, HIGH);
    }
    if(go == '4'){
      Serial.println("4");
      digitalWrite(3, LOW);
    }*/
  }
}
