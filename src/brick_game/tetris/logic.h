#ifndef TETRIS_LOGIC_H_
#define TETRIS_LOGIC_H_

#include <stdlib.h>
#include <time.h>

#include "../../gui/cli/interface.h"
#include "tetris_settings.h"

void initialize_game(GameInfo_t* gi);
void move_piece(Piece* current_piece, int dx, int dy);
void rotate_piece(GameInfo_t* gi);
void drop_piece(GameInfo_t* gi);
void place_piece(GameInfo_t* gi);
void check_lines(GameInfo_t* gi);
void spawn_piece(GameInfo_t* gi);
int collision(int field[HEIGHT][WIDTH], Piece piece, int dx, int dy);
void copy_shape(int dest[4][4], const int src[4][4]);
void init_shapes(TetrisShapes* ts);
int combo_to_score(int combo);
#endif