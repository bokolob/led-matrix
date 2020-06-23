#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "display.h"
#include "keyboard.h"

const int8_t tetrominoes[5][2][4] = {
    {
        {1,1,1,1},
        {0,0,0,0}
    },
    {
        {1,0,0,0},
        {1,1,1,0}
    },
    {
        {1,1,0,0},
        {1,1,0,0},
    },
    {
        {0,1,1,0},
        {1,1,0,0}
    },
    {
        {0,1,0,0},
        {1,1,1,0}
    }
};

const int8_t matricies[4][3][3]={
    {
     {1, 0, 0},
     {0, 1, 0},
     {0, 0, 1}
    },
   {
     {0,-1,2},
     {1,0,0},
     {0,0,1},
   },
   { 
    {-1,0,2 },
    {0,-1,2},
    {0,0,1}
   },
   {
     {0,1,0},
     {-1,0,2},
     {0,0,1}
   },
};

class Tetris 
{
    private:
        Display *display;
        Keyboard *keyboard;
        
        int figure = -1;
        int x = 5;
        int y = 0;
        int rotate = 0;
        unsigned long ts = millis();
        unsigned long prev_turn;


        unsigned long speed_ = 5000;
        
        int next_tetromino();
        void put_on_board();
        void delete_from_board();
        void check_lines();
    
        bool check_place(int x, int y, int rotate);
        void put_on_board( boolean val);
        void game_over();

    public:
        Tetris(Display *display, Keyboard *keyboard);
        void turn();
};

#endif
