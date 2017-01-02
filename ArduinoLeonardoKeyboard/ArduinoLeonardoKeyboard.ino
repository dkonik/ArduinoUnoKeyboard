#include "Keyboard.h"
// TODO:
// 2) Make it so that immediately after removing fingers from combo, key can't be pressed
// 3) Make hashmap class to make updates easier?

// Amount of time to wait between sending keys when key is being held down
// in micros
const unsigned long PAUSE_BETWEEN_HOLD = 25000;
// Amount of time to wait after sending first key before sending a bunch more
const unsigned long PAUSE_AFTER_PRESS = 400000;
// The amount of time to wait until sending character from initial
// press. Don't want this to be too quick in case there is a combo
// which is coming up
const unsigned long INITIAL_HOLDOFF = 55000;

const unsigned long HOLDOFF_BETWEEN_COMBOS = 15000;
const unsigned NUM_FINGERS = 10;

bool first_time = false;

unsigned long time;

// CHARACTER COMBO DEFINITIONS **********************************
const uint16_t NINE = 0b0000101110;
const uint16_t SLASH = 0b0110100000;
const uint16_t CTRL = 0b0000000001;
const uint16_t SHIFT = 0b1000000000;
const uint16_t ENTER = 0b0001100000;
const uint16_t TWO = 0b0110100100;
const uint16_t UP = 0b0001101000;
const uint16_t DOWN = 0b0010100100;
const uint16_t LESS_THAN = 0b0000100100;
const uint16_t BACK_SPACE = 0b0000000010;
const uint16_t SEMICOLON = 0b0000100010;
const uint16_t OPENING_CURLY = 0b0100100000;
const uint16_t QUOTE = 0b0000100110;
const uint16_t ONE = 0b0110101000;
const uint16_t COMMA = 0b0000100100;
const uint16_t TAB = 0b0100000000;
const uint16_t EIGHT = 0b0001101010;
const uint16_t LEFT = 0b0011100000;
const uint16_t FUNCTION = 0b1000010001;
const uint16_t SEVEN = 0b0001101100;
const uint16_t SIX = 0b0011100010;
const uint16_t SPACE = 0b0010000000;
const uint16_t PERIOD = 0b0000101000;
const uint16_t THREE = 0b0110100010;
const uint16_t ZERO = 0b0111100000;
const uint16_t RIGHT = 0b0000101100;
const uint16_t FIVE = 0b0011100100;
const uint16_t CLOSING_CURLY = 0b0010100000;
const uint16_t ESCAPE = 0b0011101100;
const uint16_t SPECIAL_ACTIVATOR = 0b0000100000;
const uint16_t a = 0b0000000110;
const uint16_t FOUR = 0b0011101000;
const uint16_t i = 0b0010001000;
const uint16_t b = 0b0011000010;
const uint16_t e = 0b0001001000;
const uint16_t o = 0b0001000100;
const uint16_t g = 0b0011001000;
const uint16_t f = 0b0001001100;
const uint16_t c = 0b0110000000;
const uint16_t h = 0b0101000000;
const uint16_t k = 0b0110001000;
const uint16_t j = 0b0001000110;
const uint16_t m = 0b0010000010;
const uint16_t l = 0b0001000010;
const uint16_t d = 0b0011000000;
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

uint8_t buf[8] = { 0 }; /* Keyboard report buffer */


// the setup function runs once when you press reset or power the board
void setup() {
  //  initialize digital pin LED_BUILTIN as an output.
  Keyboard.begin();
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
      Keyboard.releaseAll();
    }
    last_key = 0b00000000;
    time = micros();
    return;
  }

  //Asynchronous characters
  if((FUNCTION & current_key) == FUNCTION){
    // TODO: Figure out function key
    // Potential fix: Double click shift and delete?
  } 
  else{
    if((CTRL & current_key) == CTRL){
     Keyboard.press(KEY_LEFT_CTRL);
    }
    if((SHIFT & current_key) == SHIFT){
      Keyboard.press(KEY_LEFT_SHIFT);
    }
    if((ALT & current_key) == ALT){
      Keyboard.press(KEY_LEFT_ALT);
    }
  }

  press_key();
  
  last_key = current_key;
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
    Keyboard.press('a');
    Keyboard.release('a');
  }
  else if(current_key_removed == b){
    Keyboard.press('b');
    Keyboard.release('b');
  }
  else if(current_key_removed == c){
    Keyboard.press('c');
    Keyboard.release('c');
  }
  else if(current_key_removed == d){
    Keyboard.press('d');
    Keyboard.release('d');
  }
  else if(current_key_removed == e){
    Keyboard.press('e');
    Keyboard.release('e');
  }
  else if(current_key_removed == f){
    Keyboard.press('f');
    Keyboard.release('f');
  }
  else if(current_key_removed == g){
    Keyboard.press('g');
    Keyboard.release('g');
  }
  else if(current_key_removed == h){
    Keyboard.press('h');
    Keyboard.release('h');
  }
  else if(current_key_removed == i){
    Keyboard.press('i');
    Keyboard.release('i');
  }
  else if(current_key_removed == j){
    Keyboard.press('j');
    Keyboard.release('j');
  }
  else if(current_key_removed == k){
    Keyboard.press('k');
    Keyboard.release('k');
  }
  else if(current_key_removed == l){
    Keyboard.press('l');
    Keyboard.release('l');
  }
  else if(current_key_removed == m){
    Keyboard.press('m');
    Keyboard.release('m');
  }
  else if(current_key_removed == n){
    Keyboard.press('n');
    Keyboard.release('n');
  }
  else if(current_key_removed == o){
    Keyboard.press('o');
    Keyboard.release('o');
  }
  else if(current_key_removed == p){
    Keyboard.press('p');
    Keyboard.release('p');
  }
  else if(current_key_removed == q){
    Keyboard.press('q');
    Keyboard.release('q');
  }
  else if(current_key_removed == r){
    Keyboard.press('r');
    Keyboard.release('r');
  }
  else if(current_key_removed == s){
    Keyboard.press('s');
    Keyboard.release('s');
  }
  else if(current_key_removed == t){
    Keyboard.press('t');
    Keyboard.release('t');
  }
  else if(current_key_removed == u){
    Keyboard.press('u');
    Keyboard.release('u');
  }
  else if(current_key_removed == v){
    Keyboard.press('v');
    Keyboard.release('v');
  }
  else if(current_key_removed == w){
    Keyboard.press('w');
    Keyboard.release('w');
  }
  else if(current_key_removed == x){
    Keyboard.press('x');
    Keyboard.release('x');
  }
  else if(current_key_removed == y){
    Keyboard.press('y');
    Keyboard.release('y');
  }
  else if(current_key_removed == z){
    Keyboard.press('z');
    Keyboard.release('z');
  }
  else if(current_key_removed == TAB){
    Keyboard.press(KEY_TAB);
    Keyboard.release(KEY_TAB);
  } 
  else if(current_key_removed == SPACE){
    Keyboard.press(' ');
    Keyboard.release(' ');
  }
  else if(current_key_removed == ENTER){
    Keyboard.press(KEY_RETURN);
    Keyboard.release(KEY_RETURN);
  }
  else if(current_key_removed == BACK_SPACE){
    Keyboard.press(KEY_BACKSPACE);
    Keyboard.release(KEY_BACKSPACE);
  }
  else if(current_key_removed == PERIOD){
    Keyboard.press('.');
    Keyboard.release('.');
  }
  else if(current_key_removed == COMMA){
    Keyboard.press(',');
    Keyboard.release(',');
  }
  else if(current_key_removed == DOWN){
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);
  }
  else if(current_key_removed == LEFT){
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);
  }
  else if(current_key_removed == UP){
    Keyboard.press(KEY_UP_ARROW);
    Keyboard.release(KEY_UP_ARROW);
  }
  else if(current_key_removed == RIGHT){
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
  }
  else if(current_key_removed == SEMICOLON){
    Keyboard.press(';');
    Keyboard.release(';');
  }
  else if(current_key_removed == OPENING_CURLY){
    Keyboard.press('[');
    Keyboard.release('[');
  }
  else if(current_key_removed == CLOSING_CURLY){
    Keyboard.press(']');
    Keyboard.release(']');
  }
  else if(current_key_removed == QUOTE){
    Keyboard.press(39);
    Keyboard.release(39);
  }
  else if(current_key_removed == SLASH){
    Keyboard.press(92);
    Keyboard.release(92);
  }
  else if(current_key_removed == ZERO){
    Keyboard.press('0');
    Keyboard.release('0');
  }
  else if(current_key_removed == ONE){
    Keyboard.press('1');
    Keyboard.release('1');
  }
  else if(current_key_removed == TWO){
    Keyboard.press('2');
    Keyboard.release('2');
  }
  else if(current_key_removed == THREE){
    Keyboard.press('3');
    Keyboard.release('3');
  }
  else if(current_key_removed == FOUR){
    Keyboard.press('4');
    Keyboard.release('4');
  }
  else if(current_key_removed == FIVE){
    Keyboard.press('5');
    Keyboard.release('5');
  }
  else if(current_key_removed == SIX){
    Keyboard.press('6');
    Keyboard.release('6');
  }
  else if(current_key_removed == SEVEN){
    Keyboard.press('7');
    Keyboard.release('7');
  }
  else if(current_key_removed == EIGHT){
    Keyboard.press('8');
    Keyboard.release('8');
  }
  else if(current_key_removed == NINE){
    Keyboard.press('9');
    Keyboard.release('9');
  }
}
