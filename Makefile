CC = gcc
CFLAGS = -Wall -Wextra

SRCS = src/main.c src/player.c src/proj.c src/update.c src/enemies.c src/app.h

EXEC = build/m

.PHONY: all clean

all: $(EXEC)


build: $(SRCS)
	$(CC) $(CFLAGS) -o $(EXEC) src/main.c -l ncurses

run:
	./$(EXEC)

clean:
	rm -f build/m
