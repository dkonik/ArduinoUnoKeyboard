// Notes
// 1) 1000 is about the perfect value for holding a character down
// 2) 

const uint16_t PAUSE_BETWEEN_HOLD = 1000;
const uint16_t PAUSE_AFTER_PRESS = 5000;

uint8_t buf[8] = { 0 }; /* Keyboard report buffer */


// the setup function runs once when you press reset or power the board
void setup() {
  //  initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  delay(200);
}

uint8_t val1 = 0, val2 = 0, val3 = 0, val4 = 0;
uint8_t val5 = 0, val6 = 0, val7 = 0, val8 = 0;
// Used to store value of the current press in each loop
uint8_t current_key = 0;
// Used for the logic of what the last key press was
uint8_t last_key = 0;
// Used as the pauses for holding a key down, should
// use value PAUSE_BETWEEN_HOLD, or for pause after initial
// key press, should use PAUSE_AFTER_PRESS
uint16_t pause = 0;

//  the loop function runs over and over again forever
void loop() {
  if(press_and_hold > 0){
    --press_and_hold;
  }
  val1 = digitalRead(2);
  val2 = digitalRead(3);
  val3 = digitalRead(4);
  val4 = digitalRead(5);
  val5 = digitalRead(6);
  val6 = digitalRead(7);
  val7 = digitalRead(8);
  val8 = digitalRead(9);

  current_key = (val1 << 7) | (val2 << 6) | (val3 << 5) | (val4 << 4)
                | (val5 << 3) | (val6 << 2) | (val7 << 1) | val8;

  if(current_key == 0b00000000){
    last_key = 0b00000000;
    pause = 0;
    release_all_keys();
  }
  
  if(current_key == 0b00010000 and not press_and_hold){
    press_and_hold = 1000;
    buf[2] = 0b00010101;
    Serial.write(buf, 8);
    releaseKey();
  }
//  else if(val5){
//    buf[2] = 0xE1;
//    buf[3] = 0x15;
//    Serial.write(buf, 8);
//    releaseKey();
//  }
  last_key = current_key;
}

void press_key(){
  //Has been holding key down
  if(last_key == curent_key){
    if(pause > 0){
      --pause;
      return;
    }
    pause = PAUSE_BETWEEN_HOLD;
  }
  else{
    pause = PAUSE_AFTER_PRESS;
  }
  send_key();
}


//TODO: Shouldn't need this function
void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); //  Release key  
}
