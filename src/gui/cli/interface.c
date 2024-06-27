#include "interface.h"

void draw_field(GameInfo_t gi) {
  clear();
  draw_preview(gi);
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      if (i == 0 || i == HEIGHT + 1)
        mvprintw(i, j * 2, "--");
      else if (j == 0)
        mvprintw(i, j * 2 + 1, "|");
      else if (j == WIDTH + 1)
        mvprintw(i, j * 2, "|");
      else if (gi.field[i - 1][j - 1]) {
        mvprintw(i, j * 2, "[]");
      } else {
        mvprintw(i, j * 2, "  ");
      }
    }
  }

  draw_piece(gi.current_piece);

  if (!gi.start) {
    mvprintw(6, WIDTH / 2, "Press \"ENTER\"");
    mvprintw(7, WIDTH / 2, "to start game");
  }

  if (gi.pause) {
    mvprintw(6, WIDTH / 2 - 1, "   Press \"p\"");
    mvprintw(7, WIDTH / 2 - 1, "to continue game");
  }
  refresh();
}

void draw_preview(GameInfo_t gi) {
  clear();
  int top_frame = 3;
  int bottom_frame = 8;
  int left_frame = WIDTH + 2;
  int right_frame = WIDTH + 7;
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = WIDTH + 2; j < WIDTH + 10; j++) {
      if (i == 0 || i == HEIGHT + 1 ||
          (i == top_frame && (j >= left_frame && j <= right_frame)) ||
          (i == bottom_frame && (j >= left_frame && j <= right_frame)))
        mvprintw(i, j * 2, "--");
      else if (j == WIDTH + 9 ||
               (j == left_frame && (i >= top_frame && i <= bottom_frame)))
        mvprintw(i, j * 2, "|");
      else if ((j == right_frame && (i >= top_frame && i <= bottom_frame)))
        mvprintw(i, j * 2 + 1, "|");
    }
  }

  mvprintw(2, left_frame * 2 + 1, "NEXT PIECE");
  mvprintw(HEIGHT - 2, left_frame * 2, "LEVEL: %d", gi.level);
  mvprintw(HEIGHT - 3, left_frame * 2, "SCORE: %d", gi.score);
  mvprintw(HEIGHT - 5, left_frame * 2, "TOP:%d", gi.high_score);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (gi.next_piece.shape[i][j]) {
        mvprintw(gi.next_piece.y + i + 1 + 5,
                 (gi.next_piece.x + j + 1 + (WIDTH - 1)) * 2, "[]");
      }
    }
  }
  refresh();
}

void draw_piece(const Piece piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece.shape[i][j] && piece.y + i + 1) {
        mvprintw(piece.y + i + 1, (piece.x + j + 1) * 2, "[]");
      }
    }
  }
}

void erase_piece(const Piece piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece.shape[i][j]) {
        mvprintw(piece.y + i + 1, (piece.x + j + 1) * 2, "  ");
      }
    }
  }
}

void init_ncurses() {
  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
}

void cleanup_ncurses() { endwin(); }