// A quick run that checks to make sure the hardware is 
// working correctly
const int NUM_FINGERS = 10;
void setup() {
  Serial.begin(9600);
  delay(200);
}

uint16_t current_key = 0;

// Button values will be stored here
uint8_t button_vals[NUM_FINGERS];

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

  String to_print = "";
  for(int index = 0; index < NUM_FINGERS; ++index){
    to_print += String(button_vals[index]);
  }
  

 
  Serial.println(to_print);

}

