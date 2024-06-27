#ifndef TETRIS_INERFACE_H_
#define TETRIS_INERFACE_H_

#include "../../brick_game/tetris/tetris_settings.h"

void draw_field(GameInfo_t gi);
void draw_piece(const Piece piece);
void draw_preview(GameInfo_t gi);
void erase_piece(const Piece piece);
void init_ncurses();
void cleanup_ncurses();

#endif