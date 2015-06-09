// sendIR_TAI_1.1.ino
#include "IRremote.h"
#include "IRremoteInt.h"

//int PinIR = 3; Def na Lib.
int RECV_PIN = 4;
int PIN_BR = 8;
int PIN_AZ = 9;
int PIN_VD = 10;
int PIN_VM = 11;
bool Rele = false;

IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend irsend;

void select(decode_results results);
void blinkStatus(int op);
void LGEnergySaving();

void setup()
{
	irrecv.enableIRIn(); // Start the receiver
	pinMode(PIN_BR, OUTPUT);
	pinMode(PIN_AZ, OUTPUT);
	pinMode(PIN_VD, OUTPUT);
	pinMode(PIN_VM, OUTPUT);
	digitalWrite(PIN_BR, LOW);
	digitalWrite(PIN_AZ, LOW);
	digitalWrite(PIN_VD, LOW);
	digitalWrite(PIN_VM, LOW);
  	Serial.begin(9600);
}

void loop() {
  	if (irrecv.decode(&results)) {
  		blinkStatus(1);
  		Serial.print(results.value);
  		Serial.print("   ");
  		select(results);
  		blinkStatus(2);
  		//results.value=0;
	}
	delay(200);
	irrecv.enableIRIn();
	//irrecv.resume(); // Receive the next value
  	blinkStatus(3);
}

void select(decode_results results){
	switch (results.value) {
	    case 16753245://Liga Desliga
	      irsend.sendNEC(0x20DF10EF,32);
	      break;
	    case 16736925://Mode
	      irsend.sendNEC(0x20DF0CF3,32);
	      break;
	    //case 4294967295:// Alternativo
	    case 16769565://Mute
	      //irsend.sendNEC(0x20DF906F,32);
	      irsend.sendSony(0xa90,12);
	      if(Rele)
	      	digitalWrite(PIN_BR, 0);
	      else
	      	digitalWrite(PIN_BR, 100);
	      	Rele=!Rele;
	      Serial.println("foi");
	   	  break;
	   	case 16720605://Play/Pause
	      irsend.sendNEC(0x20DF22DD,32);
	   	  break;
	   	case 16712445://Back
	      irsend.sendNEC(0x20DF807F,32);
	   	  break;
	   	case 16761405://Forward
	      irsend.sendNEC(0x20DF00FF,32);
	   	  break;
	   	case 16769055://EQ -> EnergySaving
			LGEnergySaving();
	   	  break;
	   	case 16754775://-
	      irsend.sendNEC(0x20DFC03F,32);
	      delay(30);
	      irsend.sendNEC(0x20DF827D,32);
	   	  break;
	   	case 16748655://+
	      irsend.sendNEC(0x20DF40BF,32);
	      delay(30);
	      irsend.sendNEC(0x20DF02FD,32);
	   	  break;
	   	/*case :// 0
	      
	   	  break;*/
	   	case 16750695:// Rand -> Exit
	      irsend.sendNEC(0x20DFDA25,32);
	   	  break;
	   	/*case ://
	      
	   	  break;
	   	case ://
	      
	   	  break;
	   	case ://
	      
	   	  break;
	   	case ://
	      
	   	  break;
	   	case ://
	      
	   	  break;*/
	}
	//asm volatile ("jmp 0"); //Restart Arduino
}

void blinkStatus(int op){
	switch (op) {
		case 0: //Branco = Status
	        digitalWrite(PIN_BR, 100);
		    delay(100);
		    digitalWrite(PIN_BR, 0);
	      break;
	    case 1: //Azul = Enviado
	        digitalWrite(PIN_AZ, 100);
		    delay(100);
		    digitalWrite(PIN_AZ, 0);
	      break;
	    case 2: //Verde = Recebido
	        digitalWrite(PIN_VD, 100);
		    delay(100);
		    digitalWrite(PIN_VD, 0);
	      break;
	    case 3: //Vermelho = Erro/Esperando
	        digitalWrite(PIN_VM, 100);
		    delay(50);
		    digitalWrite(PIN_VM, 0);
		    delay(50);
	      break;
	}
}

void LGEnergySaving(){
	blinkStatus(1);
	irsend.sendNEC(0x20DFC23D,32);//Settings
    delay(50);
    irsend.sendNEC(0x20DF22DD,32);//ok
    delay(50);
    irsend.sendNEC(0x20DF609F,32);//right
    delay(50);
}