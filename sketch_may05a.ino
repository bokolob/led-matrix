#include <FrequencyTimer2.h>

#include <FrequencyTimer2.h>
#include <avr/interrupt.h>
#include "font.h"
#include "display.h"
#include "keyboard.h"
#include "tetris.h"

void fill(char ** argv);
void blank(char ** argv);
void effect3(char ** argv);
void effect2(char ** argv);
void font_test(char ** argv);
void line(char ** argv);
void dot(char ** argv);

Display *display1;
Tetris *tetris;
Keyboard *keyboard;

struct callable {
  String fname;
  void (*effect)( char** argv );
};

struct callable calls[] = {
    {"fill", fill},
    {"blank", blank},
    {"points", effect3},
    {"zig-zag", effect2},
    {"font-test", font_test},
    {"line", line},
    {"dot", dot},
  };

void setup() {
  Serial.begin(19200);
  randomSeed(analogRead(0));
  Serial.println("XXX");
  display1 = new Display();
  keyboard = new Keyboard();
  tetris = new Tetris(display1, keyboard);
  
   // Turn off toggling of pin 11 and 3
   FrequencyTimer2::disable();
   // Set refresh rate (interrupt timeout period)
   FrequencyTimer2::setPeriod(2000);//2000
   // Set interrupt routine to be called
   FrequencyTimer2::setOnOverflow(draw_screen);
}

ISR(ADC_vect) {
  keyboard->interrupt();
}


void draw(unsigned int i) {
      Serial.println("printing " + String(i, BIN));

      for (int j = 0; j < COLS; j++) {
        if (i & (1 << j)) {
           display1->putPoint(0, j, true);
        }
        else {
           display1->putPoint(0, j, false);
        }

         //Serial.println(String(i) + " "+ ((i & (1 << j) > 0) ? "LOW" : "HIGH"));
         //Serial.println("" + String(i) +" "+ String(j) +" "+ String(i & (1 << j)) );
    }
    
}

volatile int current_row = 0;

void loop() {
  tetris->turn();
 
  /*while(Serial.available() == 0) {
    Serial.println("A1="+String(analogRead(1)));  
  }
  
  String command = Serial.readStringUntil('\n');
  Serial.println(command);

  char buffer[30];

  command.toCharArray(buffer, sizeof(buffer));
  
  char *words[10] = {};

  split_args(buffer, words);
  
  for (int i = 0; i < sizeof(calls)/sizeof(struct callable); i++) {
    struct callable item = calls[i];

    if (!strcmp(item.fname.c_str(), words[0])) {
      // blank();
       item.effect(words);
       Serial.println("finished");
       break;
    }
    
  }
    */
}

void split_args(char* input, char *words[]) {
  int pos = 0;

  char * initial = input;
  
  while (*input != '\0') {
    while (*input == ' ' && *input != '\0') {input++;};

    if (input != initial) {
       *(input-1) = '\0';
    }
    
    words[pos]=input;
    pos++;
    while (*input != ' ' && *input != '\0') {input++;};
  }

}

void line(char ** argv) {  
  int l = atoi(argv[1]);

  if (l < 0 || l >= ROWS) {
    return;
  }
  
  Serial.println("line at "+String(l));

  for (int i = 0; i < COLS; i++) {
    display1->putPoint(l,i,true);
  }
  
}


void dot(char ** argv) {
  int y = atoi(argv[1]);
  int x = atoi(argv[2]);
  display1->putPoint(y,x, !display1->getPoint(y,x));
}



void font_test(char ** argv) {
  for (int i = 48; i <= 255; i++) {
      put_char(i,8,4);
      delay(3000);
  }
}

void effect3(char ** argv) {
  for (int y = ROWS-1; y >=0; y--) {
    for (int x = COLS-1; x >= 0; x--) {
         display1->putPoint(y,x, true);
        delay(50);
    }
  }
}

void effect2(char ** argv) {
  int width = COLS;
  int height = ROWS;

  int x = 0;
  int y = 0;

  int dx = 1;
  int dy = 1;
  
  while (width > 0 && height > 0) {
     for (int i = 0; i < width; i++) {
        display1->putPoint(y,x, true);
        x = x + dx;
        delay(50);
     }

     x-=dx; 
  
     for (int i = 0; i < height; i++) {
        display1->putPoint(y,x, true);
        y = y + dy;
        delay(50);
     }

     y-=dy;
     x-=dx; 
     dx *= -1;
     dy *= -1;
     
     width--;
     height--;
  }
}


void effect1(char ** argv) {

  int y = 0;
  int x = 0;

  while ((y < ROWS) || (x < COLS)) {
    for(int i = 0; i <= x && y < ROWS; i++) {
      display1->putPoint(y,i, true);
    }

    for(int i = 0; i <= y && x < COLS; i++) {
      display1->putPoint(i,x, true);
    }

//Serial.println("x="+String(x)+" y="+String(y));
    
    if (y < ROWS) y++;
    if (x < COLS) x++;
    
    delay(200);
  }

 // serial_print_screen();
  blank(NULL);

  /*
  long st = micros();
  draw_screen();
  long d = micros() - st;
  Serial.println("d: " +String(current_row)+" "+ String(d));
  */
  /*
//fill();

  while(true) {
    for (int i = 0; i< 17; i++) {
      //draw(i); 
     // delay(250);
    }
  }
  */

}

void put_char(int code, int y, int x) {
  const uint8_t *line = font[code];
  
  for (int i = 0; i < 8; i++) {
      uint8_t character = pgm_read_byte(line+i);
      uint8_t mask = B00010000;
      for (int j = 0; j < 5; j++, mask >>= 1 ) {
         if ((character & mask) != 0) {
           display1->putPoint(y+i,x+j, true); 
         }
         else {
           display1->putPoint(y+i,x+j, false);
         } 
      }
  }  
}

void draw_screen() {         
       display1->printLine(current_row);
       current_row = (current_row +1) % ROWS;
}


void serial_print_screen() {
  
    for (int i = 0; i < ROWS; i++) {
      Serial.print(""+String(i)+": ");
      for (int j = 0; j < COLS; j++) {
        Serial.print( display1->getPoint(i,j) );
      }
      Serial.println("");
     
    }
}


void fill(char ** argv) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      display1->putPoint(i,j, true);
    }

  }
}

void blank(char ** argv) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      display1->putPoint(i,j, false);
    }
  }
}
