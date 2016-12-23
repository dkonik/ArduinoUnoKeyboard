// Notes
// 1) 1000 is about the perfect value for holding a character down
// 2) Figure out how to deal with initial delay for waiting if there is a combo coming

// Amount of time to wait between sending keys when key is being held down
// in micros
const unsigned long PAUSE_BETWEEN_HOLD = 25000;
// Amount of time to wait after sending first key before sending a bunch more
const unsigned long PAUSE_AFTER_PRESS = 500000;
// The amount of time to wait until sending character from initial
// press. Don't want this to be too quick in case there is a combo
// which is coming up
const uint16_t INITIAL_HOLDOFF = 1000;

bool first_time = false;

unsigned long time;

// CHARACTER COMBO DEFINITIONS **********************************
const uint8_t TAB = 0b01000000;
const uint8_t SPACE = 0b00010000;
const uint8_t BACK_SPACE = 0b00001000;
const uint8_t SPECIAL_ACTIVATOR = 0b10000000;

// END CHARACTER COMBO DEFINITIONS ******************************

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

  //No key is being pushed
  if(current_key == 0b00000000){
    last_key = 0b00000000;
    time = micros();
    return;
  }

  press_key();
  
//  if(current_key == 0b00010000 and not press_and_hold){
//    press_and_hold = 1000;
//    buf[2] = 0b00010101;
//    Serial.write(buf, 8);
//    releaseKey();
//  }
  
  last_key = current_key;
  release_all_keys();
}

void press_key(){
  //Has been holding key down
  if(last_key == current_key){
    //Wait after hold
    if(micros() < time){
      return;
    }
    if(first_time){
      time = micros() + PAUSE_AFTER_PRESS;
      first_time = false;
    } else{
      time = micros() + PAUSE_BETWEEN_HOLD; 
    }
  }
  else{
    // Don't send character immediately after push as there could
    // be a possible combo coming up, so wait
    time = micros() + INITIAL_HOLDOFF;
    first_time = true;
    return;
  }
  send_key();
}

//This function actually figures out what key to push
void send_key(){
  // Special characters
  if((current_key & SPECIAL_ACTIVATOR) == SPECIAL_ACTIVATOR){
    if(current_key == SPECIAL_ACTIVATOR){
      return;
    }
  }
  // "Regular" characters
  else{
    if(current_key == 0b00010000){
      buf[2] = 0b00010101;
      Serial.write(buf, 8);
    }
    if(current_key == 0b00011000){
      buf[2] = 0b00010110;
      Serial.write(buf, 8);
    }
  }
}

// Makes computer stop writing character, should be called
// after every loop since taking care of repeated characters
// elsewhere
void release_all_keys() 
{
  buf[0] = 0;
  for(unsigned i = 2; i < 8; ++i){
    buf[i] = 0;
  }
  Serial.write(buf, 8); //  Release key  
}
