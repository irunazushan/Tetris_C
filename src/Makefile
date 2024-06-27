CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -Wall -Werror -Wextra --coverage

BACKEND_DIR = brick_game/tetris/
FRONTEND_DIR = gui/cli/
LOGIC_SRC = $(BACKEND_DIR)logic.c $(BACKEND_DIR)tetris.c
INTERFACE_SRC = $(FRONTEND_DIR)interface.c

LOGIC_OBJS = $(LOGIC_SRC:.c=.o)
INTERFACE_OBJS = $(INTERFACE_SRC:.c=.o)

OS := $(shell uname -s)

ifeq ($(OS),Linux)
  LIBS =  -pthread -lcheck -lsubunit -lm -lncurses
else
  LIBS = -lcheck -lm -lncurses
endif

all: clean install

install: $(FRONTEND_DIR)interface.a $(BACKEND_DIR)logic.a
	$(CC) $(CFLAGS) main/main.c $^ -o tetris $(LIBS)
	./tetris

uninstall:
	rm -rf tetris

dvi:
	open README.md

dist:clean
	mkdir tetris_game
	mkdir tetris_game/src
	cp -R brick_game tetris_game/src
	cp -R gui tetris_game/src
	cp -R diagram.png tetris_game/src
	cp -R main tetris_game/src
	cp -R Makefile tetris_game/src
	cp -R README.md tetris_game/src
	tar cvzf tetris_game.tgz tetris_game/
	rm -rf tetris_game

test: clean
	$(CC) $(CFLAGS) test/tetris_logic_test.c $(FRONTEND_DIR)interface.c $(BACKEND_DIR)logic.c -o tetris_logic_test $(LIBS)
	./tetris_logic_test

gcov_report: clean $(FRONTEND_DIR)interface.a
	$(CC) $(CFLAGS) $(GCOVFLAGS) test/tetris_logic_test.c $(BACKEND_DIR)logic.c $(FRONTEND_DIR)interface.c -o tetris_logic_test $(LIBS)
	./tetris_logic_test
	lcov -t "test" -o test.info --no-external -c -d .
	lcov --remove test.info '$(FRONTEND_DIR)interface.c' --output-file filtered_coverage.info
	genhtml filtered_coverage.info --output-directory report
	open ./report/index.html

lcov:
	brew install lcov

$(FRONTEND_DIR)interface.a: $(INTERFACE_OBJS)
	ar rcs $(FRONTEND_DIR)interface.a $(INTERFACE_OBJS)

$(BACKEND_DIR)logic.a: $(LOGIC_OBJS)
	ar rcs $(BACKEND_DIR)logic.a $(LOGIC_OBJS) $(INTERFACE_OBJS)

leaks:
	leaks -atExit -- ./tetris

valg:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./tetris


clang:
	clang-format --style=Google -i **/**/*.c **/**/*.h **/*.c **/*.h

clean:
	rm -rf tetris a.out tetris_logic_test **/**/*.o *.o **/**/*.a *.gcno *.gcda *.info report