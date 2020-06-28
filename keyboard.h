#ifndef __KEYS_H__
#define __KEYS_H__

#define K_LEFT   B00000010
#define K_RIGHT  B00000100
#define K_UP     B00001000
#define K_DOWN   B00010000
#define K_ENTER  B00100000

#define TIMEOUT 10
#define SAMPLES 5

#include <Arduino.h>

class Keyboard {
    private:

        enum State {RELEASED, PRESSED, COUNTING_1, COUNTING_0, SKIPPING};
        
        struct key_state {
            unsigned long last_read_time;
            int count;
            State state;
            boolean last_state;
            uint8_t mask;
        };

        volatile uint8_t value;
        uint8_t prev_value;
        unsigned long last_read_time;
        struct key_state *keys;
      
    public:
       Keyboard();
       uint8_t read();
       void update();
};


#endif
