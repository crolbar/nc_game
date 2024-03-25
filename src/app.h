#ifndef APP
#define APP

#include <stdbool.h>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
};

struct Proj {
    int x;
    int y;
    enum Direction dir;
    char p;
    bool alive;
};

struct Player {
    int x;
    int y;
    enum Direction dir;
    char p;
};

struct App {
    struct Player p;
    struct Proj *projs;
    int num_projs;
    char prev_char;
    bool exit;
};

#endif
