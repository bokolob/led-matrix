#include "display.h"

Display::Display() {
    memset(screen,0, sizeof(screen));
    memset(output_ports, 0, sizeof(output_ports));
  
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    digitalWrite(latchPin, HIGH);

    for (int i = FIRST_PIN; i <= LAST_PIN; i++) {
      pinMode(i, OUTPUT);
      digitalWrite(i, LOW);
    }

    disable_rows();
    out_byte(0, 0xff);
    out_byte(1, 0xff); 
}

 bool* Display::getScreen() {
  return &(screen[0][0]);
 }

 void Display::putPoint(int col, int row, boolean value) {
        screen[col][row] = value;
 }

  boolean Display::getPoint(int col, int row) {
     return screen[col][row];
  }

void Display::clear() {
      memset(screen,0, sizeof(screen));
}
  
  volatile static int inside = 0;
  
  void Display::printLine(int row_num) {
    if (inside == 1) {
      return;
    }

      inside=1;
    byte binary[MEMORY_BLOCKS];

    memset(binary,0, sizeof(binary));


    for (int i = 0; i < COLS; i++) {
      byte mask = (1 << (7 - (i % 8)));
            
      if (screen[row_num][i] == true) {
        binary[i / 8] |= mask;
      }
      else {
        binary[i / 8] &= ~mask;
      }        
    }

    disable_rows();

    for (int i = 0; i < MEMORY_BLOCKS; i++) {
        out_byte(i, ~(binary[i])); // надо поставить 0 чтобы зажечь светодиод, т.к тут мы управляем катодом (-)     
    }    
    
    enable_row(row_num);
    inside=0;
  }

  void Display::disable_rows() {
    for (int i = 0; i < 3; i++) {
        out_byte(FIRST_ROW_ADDR + i, 0xff);       
    }  
  }

  void Display::enable_row(int r) {
    int desired_block = r / ROW_BLOCK_SIZE;
    out_byte(FIRST_ROW_ADDR + desired_block, ~(1 << (7-(r % ROW_BLOCK_SIZE)) ));       
  }

  byte Display::reverse(byte b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
  }
 
  void Display::out_byte(int bank_number, byte b) {
     // Serial.print(b, BIN);
     // Serial.println(" to bank "+ String(bank_number));

      if (output_ports[bank_number] == b) {
        return;
      }

      output_ports[bank_number] = b;
      
      set_addr_line(0);

      b = reverse(b);
      
   /*   PORTB = (b >> 2);
      PORTD &= B00111111;
      PORTD |= (b & B11000000);
     */ 
      
      for (int i = 0; i < 8; i++) {
        digitalWrite( FIRST_PIN + i, (b & (1 << i)) ? HIGH : LOW);
      }
      
     
      set_addr_line(1 << bank_number);
  }

  void Display::set_addr_line(int addr_line) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, addr_line);   
      digitalWrite(latchPin, HIGH);
  }
