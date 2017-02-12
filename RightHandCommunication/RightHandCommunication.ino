// Using Leonardo
#define SOFTSPI
#include <SPI.h>
#include <Keyboard.h>
#include <Mouse.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DigitalIO.h"
const int CE = 13;
const int CSN = 12;
const int IRQ = 2;
const uint8_t MOUSE_IRQ = 3, MOUSE_SS = 5;

const uint8_t SOFT_SPI_MISO_PIN_MOUSE = 0;
const uint8_t SOFT_SPI_MOSI_PIN_MOUSE = 1;
const uint8_t SOFT_SPI_SCK_PIN_MOUSE  = 7;
const uint8_t SPI_MODE_MOUSE = 3;

SoftSPI<SOFT_SPI_MISO_PIN_MOUSE, SOFT_SPI_MOSI_PIN_MOUSE, SOFT_SPI_SCK_PIN_MOUSE, SPI_MODE_MOUSE> soft_mouse_spi;

extern const unsigned short firmware_length;
extern const unsigned char firmware_data[];

// Registers
#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64


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

volatile byte xydat[4];
int16_t * deltax = (int16_t *) &xydat[0];
int16_t * deltay = (int16_t *) &xydat[2];

void check_and_move_mouse(){
  // SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
  digitalWrite(MOUSE_SS,LOW);
  xydat[0] = (byte)adns_read_reg(REG_Delta_X_L);
  xydat[1] = (byte)adns_read_reg(REG_Delta_X_H);
  xydat[2] = (byte)adns_read_reg(REG_Delta_Y_L);
  xydat[3] = (byte)adns_read_reg(REG_Delta_Y_H);
  digitalWrite(MOUSE_SS,HIGH);
  // SPI.endTransaction();
  //Reduce sensitivity at higher speeds
  if(abs(*deltax) != 1){
    *deltax = *deltax / 2;
  }
  if(abs(*deltay) != 1){
    *deltay = *deltay / 2;
  }
  if(*deltax != 0 || *deltay != 0){
    Mouse.move(*deltax, *deltay, 0);
  }
}

void setup() {
  //Prevent interference with SPI devices 
  pinMode (MOUSE_SS, OUTPUT);
  digitalWrite(MOUSE_SS, HIGH);
  pinMode(CSN, OUTPUT);
  digitalWrite(CSN,HIGH);
  
  Serial.begin(9600);
  radio.begin();
  Keyboard.begin();
  Mouse.begin();

  // args = [pipe#, pipe_address]
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  attachInterrupt(digitalPinToInterrupt(IRQ), check_radio, LOW );

  soft_mouse_spi.begin();
    performStartup();
  dispRegisters();
}

//Do nothing because check_radio handles everything
void loop() {
    check_and_move_mouse();
  Serial.println("here");
  delay(500);
  //check_radio();
}

void performStartup(void){
  adns_com_end(); // ensure that the serial port is reset
  adns_com_begin(); // ensure that the serial port is reset
  adns_com_end(); // ensure that the serial port is reset
  adns_write_reg(REG_Power_Up_Reset, 0x5a); // force reset
  delay(50); // wait for it to reboot
  // read registers 0x02 to 0x06 (and discard the data)
  adns_read_reg(REG_Motion);
  adns_read_reg(REG_Delta_X_L);
  adns_read_reg(REG_Delta_X_H);
  adns_read_reg(REG_Delta_Y_L);
  adns_read_reg(REG_Delta_Y_H);
  // upload the firmware
  adns_upload_firmware();
  delay(10);
  //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
  // reading the actual value of the register is important because the real
  // default value is different from what is said in the datasheet, and if you
  // change the reserved bytes (like by writing 0x00...) it would not work.
  byte laser_ctrl0 = adns_read_reg(REG_LASER_CTRL0);
  adns_write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );

  delay(1);

}

void adns_com_begin(){
  digitalWrite(MOUSE_SS, LOW);
}

void adns_com_end(){
  digitalWrite(MOUSE_SS, HIGH);
}

byte adns_read_reg(byte reg_addr){
  adns_com_begin();

  // send adress of the register, with MSBit = 0 to indicate it's a read
  soft_mouse_spi.transfer(reg_addr & 0x7f );
  delayMicroseconds(100); // tSRAD
  // read data
  byte data = soft_mouse_spi.transfer(0);

  delayMicroseconds(1); // tSCLK-NCS for read operation is 120ns
  adns_com_end();
  delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

  return data;
}

void adns_write_reg(byte reg_addr, byte data){
  adns_com_begin();

  //send adress of the register, with MSBit = 1 to indicate it's a write
  soft_mouse_spi.transfer(reg_addr | 0x80 );
  //sent data
  soft_mouse_spi.transfer(data);

  delayMicroseconds(20); // tSCLK-NCS for write operation
  adns_com_end();
  delayMicroseconds(100); // tSWW/tSWR (=120us) minus tSCLK-NCS. Could be shortened, but is looks like a safe lower bound
}

void adns_upload_firmware(){
  // send the firmware to the chip, cf p.18 of the datasheet
  // set the configuration_IV register in 3k firmware mode
  adns_write_reg(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved

  // write 0x1d in SROM_enable reg for initializing
  adns_write_reg(REG_SROM_Enable, 0x1d);

  // wait for more than one frame period
  delay(10); // assume that the frame rate is as low as 100fps... even if it should never be that low

  // write 0x18 to SROM_enable to start SROM download
  adns_write_reg(REG_SROM_Enable, 0x18);

  // write the SROM file (=firmware data)
  adns_com_begin();
  soft_mouse_spi.transfer(REG_SROM_Load_Burst | 0x80); // write burst destination adress
  delayMicroseconds(15);

  // send all bytes of the firmware
  unsigned char c;
  for(int i = 0; i < firmware_length; i++){
    c = (unsigned char)pgm_read_byte(firmware_data + i);
    soft_mouse_spi.transfer(c);
    delayMicroseconds(15);
  }
  adns_com_end();
  }

void dispRegisters(void){
  int oreg[7] = {
    0x00,0x3F,0x2A,0x02  };
  char* oregname[] = {
    "Product_ID","Inverse_Product_ID","SROM_Version","Motion"  };
  byte regres;

  digitalWrite(MOUSE_SS,LOW);

  int rctr=0;
  for(rctr=0; rctr<4; rctr++){
    soft_mouse_spi.transfer(oreg[rctr]);
    delay(1);
    Serial.println("---");
    Serial.println(oregname[rctr]);
    Serial.println(oreg[rctr],HEX);
    regres = soft_mouse_spi.transfer(0);
    Serial.println(regres,BIN);
    Serial.println(regres,HEX);
    delay(1);
  }
  digitalWrite(MOUSE_SS,HIGH);
}


