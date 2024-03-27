#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "app.h"
#include "update.c"
#include "player.c"
#include "proj.c"
#include "enemies.c"
#include "deathanim.c"

void init_colors() {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
}

void render_entity_dir_traject
    (
        int entity_y,
        int entity_x,
        enum Direction entity_dir
    ) 
{
    if (entity_dir == LEFT || entity_dir == RIGHT) {
        int dist_to_end; 

        if (entity_dir == LEFT) {
            dist_to_end = entity_x;
        } else {
            dist_to_end = abs(COLS - 1 - entity_x);
        }

        for (int i = 1; i < dist_to_end; i+=3) {
            int x;

            if (entity_dir == RIGHT) {
                x = entity_x + i;
            } else {
                x = abs(entity_x - i);
            }

            mvprintw(entity_y, x, ".");
        }
    } else if (entity_dir == DOWN || entity_dir == UP) {
        int dist_to_end; 

        if (entity_dir == DOWN) {
            dist_to_end = abs(LINES - entity_y);
        } else {
            dist_to_end = entity_y + 1;
        }

        for (int i = 1; i < dist_to_end; i+=2) {
            int y;

            if (entity_dir == DOWN) {
                y = entity_y + i;
            } else {
                y = abs(entity_y - i);
            }

            mvprintw(y, entity_x, ".");
        }
    }
}

struct App init_app() {
    struct App app = {
        .player = {
            .x = COLS/2,
            .y = LINES/2,
            .dir = NONE,
            .p = 'x',
            .alive = true,
            .wpn = BLAST,
        },
        .deathanim = {
            .p1 = '#',
            .is = false,
        },
        .colors = {
            .red = 1,
            .yellow = 2,
            .green = 3,
            .blue = 4,
            .magenta = 5,
            .cyan = 6,
        },
        .projs = NULL,
        .enemies = (struct Enemy*)malloc(sizeof(struct Enemy)),
        .num_projs = 0,
        .num_enemies = 0,
        .prev_char = ' ',
        .exit = false,
        .show_stats = true,
        .shooter_enemies_indexes = {},
    };

    gettimeofday(&app.start_time, NULL);
    gettimeofday(&app.last_enemie_spawn_time, NULL);
    gettimeofday(&app.last_enemie_shoot_time, NULL);

    return app;
}

double get_elapsed(struct timeval start_time) {
    struct timeval end_time;
    gettimeofday(&end_time, NULL);

    return (double)(end_time.tv_sec - start_time.tv_sec) + 
        (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
}

int main() {
    initscr();
    cbreak();
    noecho();
    start_color();
    curs_set(0);
    keypad(stdscr, TRUE);  // Enable special keys
    nodelay(stdscr, TRUE); // Enable non-blocking input
    srand(time(NULL)); // TIME FOR RANDOM NUMBERS

    init_colors();

    struct App app = init_app();

    while (!app.exit) {
        if (app.show_stats) {
            mvprintw(0, 1, "p.y: %d p.x: %d", app.player.y, app.player.x);
            mvprintw(1, 1, "max y: %d max x: %d", LINES, COLS);
            mvprintw(2, 1, "projs: %d", app.num_projs);
            mvprintw(3, 1, "kills: %d", app.player.kills);
            mvprintw(4, 1, "enemies: %d", app.num_enemies);
            mvprintw(5, 1, "DA: %c", (app.deathanim.is) ? 'y' : 'n');
        }

        if (app.player.alive) {

            update(&app);

            erase();

            // spawn_enemie once a second
            if (get_elapsed(app.last_enemie_spawn_time) >= 1) {
                spawn_enemie(&app);

                gettimeofday(&app.last_enemie_spawn_time, NULL);
            }

            render_player(&app);

            // random `NUM_ENEMIES_SHOOT_AT_A_TIME`enemies shoot once a sec
            enemy_spawn_proj(&app);

            check_for_dead_projs(&app);
            check_for_dead_player(&app);

            remove_dead_projs(&app);
            remove_dead_enemies(&app);

            mvrender_projs(&app);
            mvrender_enemies(&app);
            mvrender_death_anim(&app.deathanim);

        } else {
            update_gameover(&app);

            erase();

            double time = (app.end_time.tv_sec - app.start_time.tv_sec) + 
                (app.end_time.tv_usec - app.start_time.tv_usec) / 1000000.0;

            mvprintw(LINES/2 - 5, COLS/2 - 5, "GAME OVER");
            mvprintw(LINES/2 - 3, COLS/2 - 5, "time: %.0fs", time);
            mvprintw(LINES/2 - 2, COLS/2 - 5, "kills: %d", app.player.kills);
        }

        usleep(10000);
    }

    endwin();
    return 0;
}
