CC = gcc
CFLAGS = -Wall -Wextra

SRCS = src/main.c src/player.c src/proj.c src/update.c src/enemies.c src/app.h src/deathanim.c src/book.c src/sword.c

EXEC = build/nc_game

.PHONY: all clean build

all: build


build: $(SRCS) | buildd
	$(CC) $(CFLAGS) -o $(EXEC) src/main.c -l ncursesw

buildd:
	mkdir -p build

run:
	./$(EXEC)

clean:
	rm -f build/m
