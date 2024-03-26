#include <ncurses.h>
#include "app.h"

void render_player(struct App *app) {
    switch (app->player.dir) {
        case UP:
            app->player.p = '^';
            break;
        case DOWN:
            app->player.p = 'v';
            break;
        case LEFT:
            app->player.p = '<';
            break;
        case RIGHT:
            app->player.p = '>';
            break;
        case NONE:
            app->player.p = 'x';
            break;
    }

    //render_entity_dir_traject(app->player.y, app->player.x, app->player.dir);
    mvprintw(app->player.y, app->player.x, "%c", app->player.p);
}

void check_if_player_alive(struct App *app) {
    for (int i = 0; i < app->num_enemies; i++) {
        struct Enemy e = app->enemies[i];
        int player_x = app->player.x;
        int player_y = app->player.y;

        if (
                (
                 e.x == player_x ||
                 e.x + 1 == player_x ||
                 e.x + 2 == player_x
                ) &&
                (
                 e.y == player_y ||
                 e.y - 1 == player_y
                )
           )
        {
            app->player.alive = false;
            gettimeofday(&app->end_time, NULL);
        }
    }
}

