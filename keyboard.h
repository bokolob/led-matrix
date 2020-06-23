#ifndef __KEYS_H__
#define __KEYS_H__

#define K_UP     2
#define K_LEFT   4
#define K_RIGHT  8
#define K_DOWN  16
#define K_ENTER 32

#define TIMEOUT 300
#define THRESHOLD 600

#include <Arduino.h>

class Keyboard {
    private:
        struct key_state {
            long last_read_time;
            int pin;
            uint8_t mask;
        };

        unsigned long last_read_time;
        volatile int current_key;
        int temp_val;
        volatile int value;
        struct key_state *keys;
      
    public:
       Keyboard();
       void interrupt();
       uint8_t read();
};


#endif
