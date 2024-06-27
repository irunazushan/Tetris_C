
#ifndef TETRIS_H_
#define TETRIS_H_

#include <string.h>

#include "../../gui/cli/interface.h"
#include "logic.h"

void userInput(UserAction_t action);
GameInfo_t updateCurrentState();
UserAction_t get_action(int ch);
int read_high_score(const char *filename);
void write_score(const char *filename, GameInfo_t *gi);
GameInfo_t *get_game_info();
void run_tetris();
#endif