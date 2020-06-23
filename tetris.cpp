#include "tetris.h"

int Tetris::next_tetromino() {
   return   random(0, 5);
}

void Tetris::put_on_board() {
    put_on_board(true);
}

void Tetris::delete_from_board() {
    put_on_board(false);
}

void do_rotation(int matrix, int x0, int y0, int *dst_x, int *dst_y) {  
    *dst_x = matricies[matrix][0][0] * (x0) + matricies[matrix][0][1] * (y0) + matricies[matrix][0][2];
    *dst_y = matricies[matrix][1][0] * (x0) + matricies[matrix][1][1] * (y0)  + matricies[matrix][1][2];
}

bool Tetris::check_place(int x, int y, int rotate) {

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[figure][i][j]==1) {
                int rx, ry;
                do_rotation(rotate, j, i, &rx, &ry);

                if (rx+x < 0 || rx+x >= 11 || ry+y < 0 || ry+y >= 20) {
                  return false;
                }
  
                
                if (display->getPoint(ry+y, rx + x)) {
                    return false;
                }
            }
        }
    }

    return true;
}

void Tetris::check_lines() {
    for (int i = 0; i < 2; i++) {
       int rx, ry;
       do_rotation(rotate, 0, i, &rx, &ry);  
  
       bool filled = true;    
       for (int j = 0; j < 11; j++) {
          if (!display->getPoint(ry+y, j)) {
            filled = false;
            break; 
          }
       }

       if (filled) {
        for (int n = 0; n < 11; n++) {
           display->putPoint(ry+y, n, false);    
        }
       }

    }
}


void Tetris::put_on_board( boolean val) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[figure][i][j]==1) {
                int rx, ry;
                do_rotation(rotate, j, i, &rx, &ry);
              
                display->putPoint(ry+y, rx + x, val);
            }
        }
    }
}

void 
Tetris::game_over() {
  display->clear();
  delay(200);
}

Tetris::Tetris(Display *display, Keyboard *keyboard) {
    this->display=display;
    this->keyboard=keyboard;

         figure = -1;
         x = 4;
         y = 10;
         rotate = 0;
         ts = millis();
         prev_turn = millis();
         speed_ = 200;
}


void Tetris::turn() {
    //Serial.println("turn "+ String(millis() - prev_turn)+" "+String(y));

    prev_turn = millis();

    if (figure == -1) {
        figure = next_tetromino();
        x = 4;
        y = 0;
        rotate = 0;

        if (!check_place(x,y,rotate)) {
            game_over();
        }
    }

    int key = keyboard->read();
  
    int dx = 0;
    int dr = 0;
    int dy = 0;
    
    if (key & K_LEFT) {dx--;}
    if (key & K_RIGHT) {dx++;}
    if (key & K_UP) {dr++;}

     if (prev_turn - ts >= speed_) {
      //  Serial.println("down");
        ts = prev_turn;
        dy++;
     }
    
    if (dr != 0 || dx != 0 || dy != 0) {
      delete_from_board();
          
      if (check_place(x+dx, y, (rotate+dr)%4)) {    
        x += dx;
        rotate = (rotate+dr) % 4;
      }
      
      if(dy != 0)  {
        if (!check_place(x, y+dy, rotate)) {
          put_on_board();
          check_lines();            
          figure = -1;
        }
        else {
          y+=dy;
        }
      }
    }
    
    if (figure != -1) {
      put_on_board();
    }
}
