#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <Arduino.h>

#define clockPin 3
#define dataPin  5
#define latchPin 4

#define ROWS 20
#define COLS 11
#define FIRST_PIN  6
#define LAST_PIN  13

#define FIRST_ROW_ADDR 2
#define ROW_BLOCK_SIZE 8
#define MEMORY_BLOCKS  (COLS % 8 > 0 ? 1 + COLS/8 : COLS/8)

class Display {

  public:
    Display();
    void putPoint(int col, int row, boolean value);
    void clear();
    boolean getPoint(int col, int row);

    void printLine(int row_num);

  private:
    bool screen[ROWS][COLS];
    byte output_ports[6];

    void disable_rows();
    void enable_row(int r);
    byte reverse(byte b);
    void out_byte(int bank_number, byte b);
    void set_addr_line(int addr_line);
};

#endif
