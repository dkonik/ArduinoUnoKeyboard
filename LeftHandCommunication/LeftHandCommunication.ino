// Using micro
#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

const int CE = 13;
const int CSN = 12;
const int IRQ = 2;
uint16_t msg = 1;

// Args = [ce_pin, csn_pin]
RF24 radio(CE, CSN);

const uint64_t pipe = 0xE8E8F0F0E1LL;
int button = 7;

void setup(void){
 Serial.begin(9600);
 radio.begin();
 radio.openWritingPipe(pipe);
}
 
 
void loop(void){
 if (digitalRead(button) == HIGH){
   radio.write(&msg, sizeof(msg));
   ++msg;
 }
}
