#ifndef TETRIS_SETTINGS_H_
#define TETRIS_SETTINGS_H_

#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20

typedef struct {
  int x, y;
  int shape[4][4];
} Piece;

typedef struct {
  int shapes[7][4][4];
} TetrisShapes;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
} UserAction_t;

typedef struct {
  int field[HEIGHT][WIDTH];
  TetrisShapes ts;
  Piece current_piece;
  Piece next_piece;
  int score;
  int high_score;
  int level;
  int speed;
  int start;
  int pause;
  int game_over;
} GameInfo_t;

#endif