// TODO:
// 1) Fix that shift,ctrl, and other asyncronous presses aren't always
//    sent when pressed down
// 2) Make it so that immediately after removing fingers from combo, key can't be pressed

// Amount of time to wait between sending keys when key is being held down
// in micros
const unsigned long PAUSE_BETWEEN_HOLD = 25000;
// Amount of time to wait after sending first key before sending a bunch more
const unsigned long PAUSE_AFTER_PRESS = 400000;
// The amount of time to wait until sending character from initial
// press. Don't want this to be too quick in case there is a combo
// which is coming up
const unsigned long INITIAL_HOLDOFF = 35000;

const unsigned long HOLDOFF_BETWEEN_COMBOS = 15000;
const unsigned NUM_FINGERS = 10;

bool first_time = false;

unsigned long time;

// CHARACTER COMBO DEFINITIONS **********************************
const uint16_t CTRL = 0b0000000001;
const uint16_t SHIFT = 0b1000000000;
const uint16_t ENTER = 0b0001100000;
const uint16_t UP = 0b0001101000;
const uint16_t DOWN = 0b0010100100;
const uint16_t LESS_THAN = 0b0000100100;
const uint16_t BACK_SPACE = 0b0000000010;
const uint16_t COMMA = 0b0000100100;
const uint16_t TAB = 0b0100000000;
const uint16_t LEFT = 0b0011100000;
const uint16_t FUNCTION = 0b1000010001;
const uint16_t SPACE = 0b0010000000;
const uint16_t PERIOD = 0b0000101000;
const uint16_t RIGHT = 0b0000101100;
const uint16_t ESCAPE = 0b0011101100;
const uint16_t SPECIAL_ACTIVATOR = 0b0000100000;
const uint16_t a = 0b0000000110;
const uint16_t c = 0b0010001000;
const uint16_t b = 0b0011000010;
const uint16_t e = 0b0001001000;
const uint16_t d = 0b0001000100;
const uint16_t g = 0b0011001000;
const uint16_t f = 0b0001001100;
const uint16_t i = 0b0110000000;
const uint16_t h = 0b0101000000;
const uint16_t k = 0b0110001000;
const uint16_t j = 0b0001000110;
const uint16_t m = 0b0010000010;
const uint16_t l = 0b0001000010;
const uint16_t o = 0b0011000000;
const uint16_t n = 0b0010000100;
const uint16_t q = 0b0010000110;
const uint16_t p = 0b0010001100;
const uint16_t s = 0b0100000010;
const uint16_t r = 0b0000001010;
const uint16_t u = 0b0100001000;
const uint16_t t = 0b0000001100;
const uint16_t w = 0b0100000100;
const uint16_t v = 0b0100001100;
const uint16_t y = 0b0011000100;
const uint16_t x = 0b0110000100;
const uint16_t ALT = 0b0000010000;
const uint16_t z = 0b0101001000;
const uint16_t GRTR_THAN = 0b1000101000;


// END CHARACTER COMBO DEFINITIONS ******************************

// CHARACTER BUFFER VALUES **************************************
const uint8_t CTRL_VAL = 0xE0;
const uint8_t SHIFT_VAL = 0xE1;
const uint8_t ALT_VAL = 0xE2;
const uint8_t PERIOD_VAL = 0x37;
const uint8_t COMMA_VAL = 0x36;
const uint8_t ENTER_VAL = 0x28;
const uint8_t BACK_SPACE_VAL = 0x2A;
const uint8_t SPACE_VAL = 0x2C;
const uint8_t TAB_VAL = 0x2B;
const uint8_t a_VAL = 0x4;
const uint8_t b_VAL = 0x5;
const uint8_t c_VAL = 0x6;
const uint8_t d_VAL = 0x7;
const uint8_t e_VAL = 0x8;
const uint8_t f_VAL = 0x9;
const uint8_t g_VAL = 0xa;
const uint8_t h_VAL = 0xb;
const uint8_t i_VAL = 0xc;
const uint8_t j_VAL = 0xd;
const uint8_t k_VAL = 0xe;
const uint8_t l_VAL = 0xf;
const uint8_t m_VAL = 0x10;
const uint8_t n_VAL = 0x11;
const uint8_t o_VAL = 0x12;
const uint8_t p_VAL = 0x13;
const uint8_t q_VAL = 0x14;
const uint8_t r_VAL = 0x15;
const uint8_t s_VAL = 0x16;
const uint8_t t_VAL = 0x17;
const uint8_t u_VAL = 0x18;
const uint8_t v_VAL = 0x19;
const uint8_t w_VAL = 0x1a;
const uint8_t x_VAL = 0x1b;
const uint8_t y_VAL = 0x1c;
const uint8_t z_VAL = 0x1d;
const uint8_t UP_VAL = 0x52;
const uint8_t DOWN_VAL = 0x51;
const uint8_t LEFT_VAL = 0x50;
const uint8_t RIGHT_VAL = 0x4F;
// END CHARACTER BUFFER VALUES **********************************

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
unsigned remove_index = 2;
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

  remove_index = 2;
  //No key is being pushed
  if(current_key == 0){
    if(last_key != 0){
      release_all_keys();
    }
    last_key = 0b00000000;
    time = micros();
    return;
  }

  //Asynchronous characters
  if((FUNCTION & current_key) == FUNCTION){
    // TODO: Figure out function key
  } 
  else{
    if((CTRL & current_key) == CTRL){
      buf[remove_index++] = CTRL_VAL;
    }
    if((SHIFT & current_key) == SHIFT){
      buf[remove_index++] = SHIFT_VAL;
    }
    if((ALT & current_key) == ALT){
      buf[remove_index++] = ALT_VAL;
    }
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
  unsigned index = remove_index;
  //Remove the ctrl, shift, and alt (or function) characters
  uint16_t current_key_removed = current_key & 0b0111101110;
  if(current_key_removed == a){
    buf[index++] = a_VAL;
  }
  else if (current_key_removed == b){
    buf[index++] = b_VAL;
  }
  else if(current_key_removed == c){
    buf[index++] = c_VAL;
  }
  else if(current_key_removed == d){
    buf[index++] = d_VAL;
  }
  else if(current_key_removed == e){
    buf[index++] = e_VAL;
  }
  else if(current_key_removed == f){
    buf[index++] = f_VAL;
  }
  else if(current_key_removed == g){
    buf[index++] = g_VAL;
  }
  else if(current_key_removed == h){
    buf[index++] = h_VAL;
  }
  else if(current_key_removed == i){
    buf[index++] = i_VAL;
  }
  else if(current_key_removed == j){
    buf[index++] = j_VAL;
  }
  else if(current_key_removed == k){
    buf[index++] = k_VAL;
  }
  else if(current_key_removed == l){
    buf[index++] = l_VAL;
  }
  else if(current_key_removed == m){
    buf[index++] = m_VAL;
  }
  else if(current_key_removed == n){
    buf[index++] = n_VAL;
  }
  else if(current_key_removed == o){
    buf[index++] = o_VAL;
  }
  else if(current_key_removed == p){
    buf[index++] = p_VAL;
  }
  else if(current_key_removed == q){
    buf[index++] = q_VAL;
  }
  else if(current_key_removed == r){
    buf[index++] = r_VAL;
  }
  else if(current_key_removed == s){
    buf[index++] = s_VAL;
  }
  else if(current_key_removed == t){
    buf[index++] = t_VAL;
  }
  else if(current_key_removed == u){
    buf[index++] = u_VAL;
  }
  else if(current_key_removed == v){
    buf[index++] = v_VAL;
  }
  else if(current_key_removed == w){
    buf[index++] = w_VAL;
  }
  else if(current_key_removed == x){
    buf[index++] = x_VAL;
  }
  else if(current_key_removed == y){
    buf[index++] = y_VAL;
  }
  else if(current_key_removed == z){
    buf[index++] = z_VAL;
  } 
  else if(current_key_removed == TAB){
    buf[index++] = TAB_VAL;
  } 
  else if(current_key_removed == SPACE){
    buf[index++] = SPACE_VAL;
  }
  else if(current_key_removed == ENTER){
    buf[index++] = ENTER_VAL;
  }
  else if(current_key_removed == BACK_SPACE){
    buf[index++] = BACK_SPACE_VAL;
  }
  else if(current_key_removed == PERIOD){
    buf[index++] = PERIOD_VAL;
  }
  else if(current_key_removed == COMMA){
    buf[index++] = COMMA_VAL;
  }
  else if(current_key_removed == DOWN){
    buf[index++] = DOWN_VAL;
  }
  else if(current_key_removed == LEFT){
    buf[index++] = LEFT_VAL;
  }
  else if(current_key_removed == UP){
    buf[index++] = UP_VAL;
  }
  else if(current_key_removed == RIGHT){
    buf[index++] = RIGHT_VAL;
  }
  Serial.write(buf, 8);
}

// Makes computer stop writing character, should be called
// after every loop since taking care of repeated characters
// elsewhere
void release_all_keys() 
{
  buf[0] = 0;
  for(unsigned index = remove_index; index < 8; ++index){
    buf[index] = 0;
  }
  Serial.write(buf, 8); //  Release key  
}
