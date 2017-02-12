// Using Leonardo
#define SOFTSPI
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DigitalIO.h"
const int CE = 13;
const int CSN = 12;
const int IRQ = 2;
const int MOUSE_SS = 5;

uint8_t msg;

// Args = [ce_pin, csn_pin]
RF24 radio(CE, CSN);

void check_radio(){
  bool tx, fail, rx;
  radio.whatHappened(tx, fail, rx);

  // Will ignore fail and tx because we will never send
  // from this hand

  // Message was received
  Serial.println("recvd");
  if(rx){
    radio.read(&msg, sizeof(msg));
    Serial.println(msg, BIN);
  }
}

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup() {
  //Prevent interference with SPI devices 
  pinMode (MOUSE_SS, OUTPUT);
  digitalWrite(MOUSE_SS, HIGH);
  pinMode(CSN, OUTPUT);
  digitalWrite(CSN,HIGH);
  
  Serial.begin(9600);
  radio.begin();

  // args = [pipe#, pipe_address]
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  attachInterrupt(digitalPinToInterrupt(IRQ), check_radio, LOW );
}

//Do nothing because check_radio handles everything
void loop() {
  Serial.println("here");
  delay(500);
  //check_radio();
}
