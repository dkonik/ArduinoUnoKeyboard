
// Amount of time to wait between sending keys when key is being held down
// in micros
const unsigned long PAUSE_BETWEEN_HOLD = 25000;
// Amount of time to wait after sending first key before sending a bunch more
const unsigned long PAUSE_AFTER_PRESS = 400000;
// The amount of time to wait until sending character from initial
// press. Don't want this to be too quick in case there is a combo
// which is coming up
const unsigned long INITIAL_HOLDOFF = 25000;
const unsigned NUM_FINGERS = 10;

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

// Button values will be stored here
uint8_t button_vals[NUM_FINGERS];
// Used to store value of the current press in each loop
uint16_t current_key = 0;
// Used for the logic of what the last key press was
uint16_t last_key = 0;

//  the loop function runs over and over again forever
void loop() {
  for(int index = 0; index < NUM_FINGERS; ++index){
    button_vals[index] = digitalRead(index+2);
  }

  current_key = 0;
  for(int index = NUM_FINGERS - 1; index >= 0; --index){
    current_key = current_key | ((button_vals[NUM_FINGERS-index-1]
     << index));
  }

  //No key is being pushed
  if(current_key == 0){
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
    if(current_key == 0b0001000000){
      buf[2] = 0b00010101;
      Serial.write(buf, 8);
    }
    if(current_key == 0b0001001000){
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
  for(unsigned index = 2; index < 8; ++index){
    buf[index] = 0;
  }
  Serial.write(buf, 8); //  Release key  
}
