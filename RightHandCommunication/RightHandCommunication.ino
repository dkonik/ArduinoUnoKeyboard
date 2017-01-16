// Using Leonardo
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

const int CE = 13;
const int CSN = 12;
const int IRQ = 2;

char msg[2];

// Args = [ce_pin, csn_pin]
RF24 radio(CE, CSN);

//void check_radio(){
//  Serial.println("Hi");
//  bool tx, fail, rx;
//  radio.whatHappened(tx, fail, rx);
//
//  // Will ignore fail and tx because we will never send
//  // from this hand
//
//  // Message was received
//  if(rx){
//    radio.read(&message, sizeof(message));
//    Serial.println(message);
//  }
//}

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void) {
  Serial.begin(9600);
  radio.begin();

  // args = [pipe#, pipe_address]
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  // attachInterrupt(digitalPinToInterrupt(IRQ), check_radio, FALLING);
}

//Do nothing because check_radio handles everything
void loop(void) {
  if (radio.available()) {
    radio.read(msg, 2);
    Serial.println(msg);
  } else {
    Serial.println("No radio avail.");
  }
  delay(100);
}
