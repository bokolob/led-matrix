#include "keyboard.h"

Keyboard::Keyboard() {
   keys = (key_state*) malloc(sizeof(struct key_state) * 5);
   memset(keys, 0,  sizeof(struct key_state) * 5);

   keys[0].mask = K_LEFT;
   keys[1].mask = K_RIGHT;
   keys[2].mask = K_UP;
   keys[3].mask = K_DOWN;
   keys[4].mask = K_ENTER;

  value = 0;
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
         result |= keys[i].mask;
         continue;
      }

      keys[i].last_read_time = t1;
      
      if (!!(input & keys[i].mask) == keys[i].last_state) {
          if ( keys[i].count < SAMPLES)  {
             keys[i].count++;
          }
      }
      else {
         keys[i].last_state = !keys[i].last_state;
         keys[i].count = 0;
      }

      if (keys[i].count == SAMPLES) {
        result |= (input & keys[i].mask);
      }
  }

  value = ~result;
}

uint8_t Keyboard::read() {
   unsigned long t1 = millis();

   if (t1 - last_read_time >= 100) {
      last_read_time = t1;
      return value;
   }

   return 0; 
}
