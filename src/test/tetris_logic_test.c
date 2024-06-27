#include "tetris_logic_test.h"

START_TEST(tetris_logic_test_1) {
  TetrisShapes *ts = (TetrisShapes *)malloc(sizeof(TetrisShapes));
  init_shapes(ts);
  int shape0[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int shape2[4][4] = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  int shape5[4][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(ts->shapes[0][i][j], shape0[i][j]);
      ck_assert_int_eq(ts->shapes[2][i][j], shape2[i][j]);
      ck_assert_int_eq(ts->shapes[5][i][j], shape5[i][j]);
    }
  }
  free(ts);
}
END_TEST

START_TEST(tetris_logic_test_2) {
  Piece piece = {
      .x = 0,
      .y = 0,
      .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}};
  int field[HEIGHT][WIDTH] = {{0}};
  ck_assert_int_eq(collision(field, piece, HEIGHT, 2), 1);

  int field2[HEIGHT][WIDTH] = {{0}};
  ck_assert_int_eq(collision(field2, piece, 2, 2), 0);
}
END_TEST

START_TEST(tetris_logic_test_3) {
  GameInfo_t *gi = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  initialize_game(gi);
  ck_assert_int_eq(gi->field[0][5], 0);
  if (gi != NULL) {
    free(gi);
  }
}
END_TEST

START_TEST(tetris_logic_test_4) {
  GameInfo_t *gi = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  initialize_game(gi);
  int shape_before_rotation[4][4] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gi->current_piece.shape[i][j] = shape_before_rotation[i][j];
    }
  }
  int shape_after_rotation[4][4] = {
      {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}};
  rotate_piece(gi);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(gi->current_piece.shape[i][j],
                       shape_after_rotation[i][j]);
    }
  }
}
END_TEST

START_TEST(tetris_logic_test_5) {
  Piece piece = {
      .x = 0,
      .y = 0,
      .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}};
  move_piece(&piece, 5, 10);
  ck_assert_int_eq(piece.x, 5);
  ck_assert_int_eq(piece.y, 10);
  move_piece(&piece, -4, -7);
  ck_assert_int_eq(piece.x, 1);
  ck_assert_int_eq(piece.y, 3);
}
END_TEST

START_TEST(tetris_logic_test_6) {
  GameInfo_t *gi = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  initialize_game(gi);
  for (int i = 0; i < WIDTH; i++) {
    gi->field[HEIGHT - 2][i] = 1;
  }
  check_lines(gi);
  ck_assert_int_eq(gi->score, 100);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      gi->field[HEIGHT - i - 2][j] = 1;
    }
  }
  check_lines(gi);
  ck_assert_int_eq(gi->score, 400);
  if (gi != NULL) {
    free(gi);
  }
}
END_TEST

START_TEST(tetris_logic_test_7) {
  GameInfo_t *gi = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  int shape[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  initialize_game(gi);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gi->current_piece.shape[i][j] = shape[i][j];
    }
  }
  drop_piece(gi);
  ck_assert_int_eq(gi->current_piece.y, HEIGHT - 2);
  if (gi != NULL) {
    free(gi);
  }
}
END_TEST

Suite *tetris_logic_suite(void) {
  Suite *suite = suite_create("tetris_logic");
  TCase *tc_core = tcase_create("core_of_tetris_logic");
  tcase_add_test(tc_core, tetris_logic_test_1);
  tcase_add_test(tc_core, tetris_logic_test_2);
  tcase_add_test(tc_core, tetris_logic_test_3);
  tcase_add_test(tc_core, tetris_logic_test_4);
  tcase_add_test(tc_core, tetris_logic_test_5);
  tcase_add_test(tc_core, tetris_logic_test_6);
  tcase_add_test(tc_core, tetris_logic_test_7);
  suite_add_tcase(suite, tc_core);
  return suite;
}

void s21_suit_executer(Suite *suite, int *fail_counter, char *log_name) {
  SRunner *suite_runner = srunner_create(suite);
  srunner_set_log(suite_runner, log_name);
  srunner_run_all(suite_runner, CK_NORMAL);
  *fail_counter += srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
}

int main() {
  int fail_counter = 0;

  s21_suit_executer(tetris_logic_suite(), &fail_counter,
                    "test/tetris_logic.log");

  return fail_counter == 0 ? 0 : 1;
}
