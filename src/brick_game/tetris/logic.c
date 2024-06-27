#include "logic.h"

void init_shapes(TetrisShapes* ts) {
  int shape1[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int shape2[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  int shape3[4][4] = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  int shape4[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  int shape5[4][4] = {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  int shape6[4][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  int shape7[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ts->shapes[0][i][j] = shape1[i][j];
      ts->shapes[1][i][j] = shape2[i][j];
      ts->shapes[2][i][j] = shape3[i][j];
      ts->shapes[3][i][j] = shape4[i][j];
      ts->shapes[4][i][j] = shape5[i][j];
      ts->shapes[5][i][j] = shape6[i][j];
      ts->shapes[6][i][j] = shape7[i][j];
    }
  }
}

int collision(int field[HEIGHT][WIDTH], Piece piece, int dx, int dy) {
  int isCollision = 0;
  for (int i = 0; i < 4 && !isCollision; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece.shape[i][j] && piece.y + i + dy >= 0) {
        int new_x = piece.x + j + dx;
        int new_y = piece.y + i + dy;
        if (new_x < 0 || new_x >= WIDTH || new_y >= HEIGHT ||
            field[new_y][new_x]) {
          isCollision = 1;
          break;
        }
      }
    }
  }
  return isCollision;
}

void copy_shape(int dest[4][4], const int src[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void initialize_game(GameInfo_t* gi) {
  srand(time(NULL));
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      gi->field[i][j] = 0;
    }
  }
  spawn_piece(gi);
  copy_shape(gi->next_piece.shape, gi->ts.shapes[rand() % 7]);
  gi->next_piece.x = WIDTH / 2 - 2;
  gi->next_piece.y = -2;
}

void rotate_piece(GameInfo_t* gi) {
  Piece rotated = gi->current_piece;
  int new_shape[4][4];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      new_shape[j][3 - i] = gi->current_piece.shape[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotated.shape[i][j] = new_shape[i][j];
    }
  }

  if (!collision(gi->field, rotated, 0, 0)) {
    erase_piece(gi->current_piece);
    gi->current_piece = rotated;
    draw_piece(gi->current_piece);
  }
}

void drop_piece(GameInfo_t* gi) {
  while (!collision(gi->field, gi->current_piece, 0, 1)) {
    move_piece(&gi->current_piece, 0, 1);
  }
  place_piece(gi);
}

void place_piece(GameInfo_t* gi) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gi->current_piece.shape[i][j]) {
        gi->field[gi->current_piece.y + i][gi->current_piece.x + j] = 1;
      }
    }
  }
}

void check_lines(GameInfo_t* gi) {
  int full_line;
  int combo = 0;
  for (int i = 0; i < HEIGHT; i++) {
    full_line = 1;
    for (int j = 0; j < WIDTH; j++) {
      if (gi->field[i][j] == 0) {
        full_line = 0;
        break;
      }
    }
    if (full_line) {
      combo++;
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < WIDTH; j++) {
          gi->field[k][j] = gi->field[k - 1][j];
        }
      }
      for (int j = 0; j < WIDTH; j++) {
        gi->field[0][j] = 0;
      }
    }
  }
  gi->score += combo_to_score(combo);
}

int combo_to_score(int combo) {
  int score = 0;
  switch (combo) {
    case 1:
      score = 100;
      break;
    case 2:
      score = 300;
      break;
    case 3:
      score = 700;
      break;
    case 4:
      score = 1500;
      break;
  }
  return score;
}

void spawn_piece(GameInfo_t* gi) {
  gi->current_piece = gi->next_piece;
  copy_shape(gi->next_piece.shape, gi->ts.shapes[rand() % 7]);
  gi->next_piece.x = WIDTH / 2 - 2;
  gi->next_piece.y = -2;
  if (collision(gi->field, gi->current_piece, 0, 0)) {
    gi->game_over = 1;
  }
}

void move_piece(Piece* current_piece, int dx, int dy) {
  erase_piece(*current_piece);
  current_piece->x += dx;
  current_piece->y += dy;
  draw_piece(*current_piece);
}