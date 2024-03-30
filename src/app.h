#ifndef APP
#define APP

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/time.h>

#define NUM_ENEMIES_SHOOT_AT_A_TIME 2

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
};

enum Weapon {
    BLAST,
    BOOK,
    SWORD,
    NO,
};

struct Proj {
    int x;
    int y;
    enum Direction dir;
    char p;
    bool alive;
    bool is_from_player;
    int move_delay;
    int circle_stage;
};

struct Player {
    int x;
    int y;
    enum Direction dir;
    char p;
    int kills;
    bool alive;
    enum Weapon wpn;
};

struct Enemy {
    int x;
    int y;
    enum Direction dir;
    char *p1;
    bool alive;
    struct timeval last_enemie_move_time;
};

struct Pos {
    int x;
    int y;
};

struct Sword {
    int size;
    struct Pos POS[6];
    char *p1;
    bool is_swinging;
    int swing_stage;
};

struct DeathAnim {
    struct Pos UL;
    struct Pos UR;
    struct Pos DL;
    struct Pos DR;
    char p1;
    bool is;
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
    struct DeathAnim deathanim;
    struct Sword sword;
    int num_projs;
    int num_enemies;
    int prev_char;
    bool exit;
    struct timeval last_enemie_spawn_time;
    struct timeval last_enemie_shoot_time;
    struct timeval start_time;
    struct timeval end_time;
    bool show_stats;
    int shooter_enemies_indexes[NUM_ENEMIES_SHOOT_AT_A_TIME];
    int book_delay_count;
    int book_delay_target;
    int book_count;
    double enemy_spawn_delay;
    int next_target_kills;
};

struct App init_app();
double get_elapsed(struct timeval);
void render_entity_dir_traject(int entity_y, int entity_x, enum Direction entity_dir);
void spawn_proj_player(struct App *app);
void player_die(struct App *app);
void start_death_anim(struct DeathAnim *da, int y, int x);

struct Selector {
    WINDOW *windows[3];
    int selected;
};

#endif
