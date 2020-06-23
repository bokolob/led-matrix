#include "keyboard.h"

Keyboard::Keyboard() {
  keys = (key_state*) malloc(sizeof(struct key_state) * 5);

   keys[0].last_read_time = 0;
   keys[0].pin = 1;
   keys[0].mask = K_LEFT;

   keys[1].last_read_time = 0;
   keys[1].pin = 2;
   keys[1].mask = K_RIGHT;

   keys[2].last_read_time = 0;
   keys[2].pin = 3;
   keys[2].mask = K_UP;

   keys[3].last_read_time = 0;
   keys[3].pin = 4;
   keys[3].mask = K_DOWN;

   keys[4].last_read_time = 0;
   keys[4].pin = 4;
   keys[4].mask = K_ENTER;

  current_key = 0;
  temp_val = 0;
  value = 0;
  last_read_time = 0;

  ADCSRA |= 
  (1<<ADEN) // Разрешение использования АЦП
  |(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)//Делитель 128 = 64 кГц
  |(1<<ADIE);
   
  ADMUX |=  (0 << REFS1)|(1 << REFS0) 
  //| (1 << ADLAR)
  | (keys[current_key].pin & 0x07);
  ADCSRA |= (1<<ADSC); //Запуск преобразования
}


void Keyboard::interrupt() {
  uint8_t low, high;

  low  = ADCL;
  high = ADCH;

  int result = (high << 8) | low;
    
  if (result < THRESHOLD) {
    value |= keys[current_key].mask;
  }
  
  current_key = (current_key+1) % 5;

 /* if (current_key == 0) {
    value = temp_val;
    temp_val = 0;
  } */
  
  ADMUX = (0 << REFS1)|(1 << REFS0)
 // |(1 << ADLAR)
  |( (keys[current_key].pin) & 0x07 );
  ADCSRA |= (1<<ADSC); //Запуск преобразования
}

uint8_t Keyboard::read() {
    unsigned long tm = millis();

    if (tm - last_read_time < TIMEOUT) {
       return 0;
    }

    last_read_time = tm;
      
    int rc = value;
    value = 0;
    return rc;
}
