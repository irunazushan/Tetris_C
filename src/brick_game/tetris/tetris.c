#include "tetris.h"

GameInfo_t *get_game_info() {
  static GameInfo_t *gi = NULL;
  if (gi == NULL) {
    gi = (GameInfo_t *)malloc(sizeof(GameInfo_t));
    if (gi == NULL) {
      fprintf(stderr, "Failed to allocate memory for GameInfo_t\n");
    } else {
      memset(gi->field, 0, sizeof(gi->field));
      memset(gi->current_piece.shape, 0, sizeof(gi->current_piece));
      memset(gi->next_piece.shape, 0, sizeof(gi->next_piece));
      gi->score = 0;
      gi->high_score = read_high_score("brick_game/tetris/top_score.txt");
      gi->level = 1;
      gi->speed = 1000;
      gi->pause = 0;
      gi->start = 0;
      gi->game_over = 0;
    }
  }
  return gi;
}

int read_high_score(const char *filename) {
  int score = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Failed to open file for reading");
  } else if (fscanf(file, "%d", &score) != 1) {
    perror("Failed to read high score from file");
  }
  if (file != NULL) fclose(file);
  return score;
}

void write_score(const char *filename, GameInfo_t *gi) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Failed to open file for writing");
    return;
  }

  fprintf(file, "%d\n", gi->score);

  fclose(file);
}

UserAction_t get_action(int ch) {
  UserAction_t userAction;
  switch (ch) {
    case KEY_LEFT:
      userAction = Left;
      break;
    case KEY_RIGHT:
      userAction = Right;
      break;
    case KEY_DOWN:
      userAction = Down;
      break;
    case ' ':
      userAction = Action;
      break;
    case 'p':
      userAction = Pause;
      break;
    case 'q':
      userAction = Terminate;
      break;
    case KEY_UP:
      userAction = Up;
      break;
    case '\n':
      userAction = Start;
      break;
  }
  return userAction;
}

void userInput(UserAction_t action) {
  switch (action) {
    case Start:
      if (!get_game_info()->start) {
        get_game_info()->start = 1;
        init_shapes(&get_game_info()->ts);
        initialize_game(get_game_info());
        initialize_game(get_game_info());
      }
      break;
    case Pause:
      get_game_info()->pause = !get_game_info()->pause;
      break;
    case Terminate:
      get_game_info()->game_over = 1;
      break;
    case Left:
      if (!collision(get_game_info()->field, get_game_info()->current_piece, -1,
                     0) &&
          !get_game_info()->pause) {
        move_piece(&get_game_info()->current_piece, -1, 0);
      }
      break;
    case Right:
      if (!collision(get_game_info()->field, get_game_info()->current_piece, 1,
                     0) &&
          !get_game_info()->pause) {
        move_piece(&get_game_info()->current_piece, 1, 0);
      }
      break;
    case Up:
      if (!get_game_info()->pause) {
        rotate_piece(get_game_info());
      }
      break;
    case Down:
      if (!collision(get_game_info()->field, get_game_info()->current_piece, 1,
                     0) &&
          !get_game_info()->pause) {
        move_piece(&get_game_info()->current_piece, 0, 1);
      }
      break;
    case Action:
      if (!get_game_info()->pause) {
        drop_piece(get_game_info());
      }
      break;
  }
}

void run_tetris() {
  GameInfo_t *gi = get_game_info();
  if (gi == NULL) return;
  init_ncurses();
  timeout(gi->speed);
  while (!gi->game_over) {
    if (!gi->pause) {
      if (!collision(gi->field, gi->current_piece, 0, 1)) {
        move_piece(&gi->current_piece, 0, 1);
      } else {
        place_piece(gi);
        check_lines(gi);
        spawn_piece(gi);
      }
    }
    if (gi->score / 600 >= gi->level && (gi->level != 10)) {
      gi->level++;
      gi->speed = gi->speed * 0.80;
      timeout(gi->speed);
    }
    draw_field(*gi);
    int ch = getch();
    if (ch != ERR) {
      UserAction_t action = get_action(ch);
      userInput(action);
    }
    *gi = updateCurrentState();
  }
  if (gi->score > gi->high_score) {
    write_score("brick_game/tetris/top_score.txt", gi);
  }
  mvprintw(8, WIDTH / 2 + 3, "GAME OVER");
  getch();
  timeout(2000);
  cleanup_ncurses();
  if (gi != NULL) {
    free(gi);
  }
}

GameInfo_t updateCurrentState() { return *get_game_info(); }
