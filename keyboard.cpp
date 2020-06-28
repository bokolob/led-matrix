#include "keyboard.h"

Keyboard::Keyboard() {
   keys = (key_state*) malloc(sizeof(struct key_state) * 5);
   memset(keys, 0,  sizeof(struct key_state) * 5);

   keys[0].mask = K_LEFT;
   keys[0].state = RELEASED;
   keys[1].mask = K_RIGHT;
   keys[1].state = RELEASED;
   keys[2].mask = K_UP;
   keys[2].state = RELEASED;
   keys[3].mask = K_DOWN;
   keys[3].state = RELEASED;
   keys[4].mask = K_ENTER;
   keys[4].state = RELEASED;

  value = 0;
  prev_value = 0;
  last_read_time = 0;

  DDRC = 0x0; //C is input
  PORTC = 0xff; //enabling pull up
}

void Keyboard::update() {
  uint8_t input = PINC;
  uint8_t result = 0;
  unsigned long t1 = millis();

  for (int i = 0; i < 5; i++) {
      if (t1 - keys[i].last_read_time < TIMEOUT/SAMPLES) {
         continue;
      }
      
      keys[i].last_read_time = t1;
      boolean button_value = !!(input & keys[i].mask);

      switch ( keys[i].state) {
         case RELEASED:
              if (!button_value) {
                keys[i].last_state = false;
                keys[i].count = 0;
                keys[i].state = COUNTING_0;
              }
              break;
          case COUNTING_1:
              result |= keys[i].mask;
          case COUNTING_0:
              if (button_value != keys[i].last_state) {
                keys[i].last_state = button_value;
                keys[i].count = 0;
              }
              else {
                keys[i].count++;

                if (keys[i].count == SAMPLES - 1) {
                   keys[i].count = 0;
                   keys[i].state = button_value ? RELEASED : PRESSED;
                }

              }

              keys[i].last_state = button_value;
              
              break;
          case PRESSED:
                result |= keys[i].mask;
                if (button_value) {
                  keys[i].count = 0;
                  keys[i].last_state = true;
                  keys[i].state=COUNTING_0;
                }
                else {
                  keys[i].state=SKIPPING;
                }
                
                break;
           case SKIPPING:
              if (button_value) {
                 keys[i].count = 0;
                 keys[i].last_state = true;
                 keys[i].state=COUNTING_0;
              }
              else {
                keys[i].count++;

                if (keys[i].count == 10*(SAMPLES) - 1) {
                   keys[i].count = 0;
                   keys[i].state = PRESSED;
                }
                
              }
              break;   
      }
  }

  value = result;
}

uint8_t Keyboard::read() {
    return value;
}
