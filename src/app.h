#ifndef APP
#define APP

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/time.h>

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
    int kills;
    bool alive;
};

struct Enemy {
    int x;
    int y;
    enum Direction dir;
    char *p1;
    bool alive;
    struct timeval last_enemie_move_time;
};

struct Colors {
    int red;
    int yellow;
    int green;
    int blue;
    int magenta;
    int cyan;
};

struct App {
    struct Player player;
    struct Proj *projs;
    struct Enemy *enemies;
    struct Colors colors;
    int num_projs;
    int num_enemies;
    int prev_char;
    bool exit;
    struct timeval last_enemie_spawn_time;
    struct timeval start_time;
    struct timeval end_time;
    bool show_stats;
};

struct App init_app();
double get_elapsed(struct timeval);
void render_entity_dir_traject(int entity_y, int entity_x, enum Direction entity_dir);

#endif
