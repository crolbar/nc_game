#include <ncurses.h>
#include <stdlib.h>
#include "app.h"

void move_proj_in_dir(struct Proj *proj) {
    if (proj->dir == LEFT || proj->dir == RIGHT) {
        if (proj->is_from_player) {
            proj->move_delay += 20;
        } else {
            proj->move_delay += 5;
        }
    } else {
        if (proj->is_from_player) {
            proj->move_delay += 10;
        } else {
            proj->move_delay += 3;
        }
    }


    if (proj->move_delay >= 20) {
        proj->move_delay = 0;
        switch (proj->dir) {
            case UP:
                proj->y--;
                break;
            case DOWN:
                proj->y++;
                break;
            case LEFT:
                proj->x--;
                break;
            case RIGHT:
                proj->x++;
                break;
            case NONE:
                proj->y--;
                break;
        }
    }
}

void mvrender_projs(struct App *app) {
    for (int i = 0; i < app->num_projs; i++) {
        struct Proj *proj = &app->projs[i];

        if (proj->x || proj->y) {
            move_proj_in_dir(proj);

            int color = (proj->is_from_player) ? app->colors.green : app->colors.red;

            attron(COLOR_PAIR(color));
            mvprintw(proj->y, proj->x, "%c", proj->p);
            attroff(COLOR_PAIR(color));
        }
    }
}

void check_for_dead_projs(struct App *app) {
    for (int i = 0; i < app->num_projs; i++) {
        struct Proj *proj = &app->projs[i];

        if (proj->x || proj->y) {
            if (
                    proj->x > COLS || 
                    proj->x < 0 || 
                    proj->y > LINES ||
                    proj->y < 0
                ) 
            {
                proj->alive = false;
            }

            if (proj->is_from_player) {
                for (int i = 0; i < app->num_enemies; i++) {
                    struct Enemy *e = &app->enemies[i];

                    if (
                            (proj->x == e->x && proj->y == e->y) ||
                            (proj->x == e->x + 1 && proj->y == e->y) ||
                            (proj->x == e->x + 2 && proj->y == e->y) ||

                            (proj->x == e->x && proj->y == e->y - 1) ||
                            (proj->x == e->x + 1 && proj->y == e->y - 1) ||
                            (proj->x == e->x + 2 && proj->y == e->y - 1)
                       )
                    {
                        proj->alive = false;
                        e->alive = false;
                        app->player.kills++;
                    }
                }
            } else {
                if (proj->x == app->player.x && proj->y == app->player.y) {
                    player_die(app);
                }
            }
        } 
    }
}

void remove_dead_projs(struct App *app) {
    struct Proj *new_projs = (struct Proj *)malloc(app->num_projs * sizeof(struct Proj));
    int new_num_projs = 0;

    for (int i = 0; i < app->num_projs; i++) {
        if (app->projs[i].alive) {
            new_projs[new_num_projs] = app->projs[i];

            new_num_projs++;
        }
    }

    app->num_projs = new_num_projs;
    free(app->projs);
    app->projs = new_projs;
}

void spawn_proj(struct App *app, bool is_from_player, int enemy_index) {
    app->projs = (struct Proj *)realloc(app->projs, (app->num_projs + 1) * sizeof(struct Proj));

    if (is_from_player) {
        app->projs[app->num_projs] = (struct Proj){
            .x = app->player.x,
            .y = app->player.y,
            .dir = app->player.dir,
            .alive = true,
            .p = app->player.p,
            .is_from_player = true,
            .move_delay = 0,
        };
    } else {
        app->projs[app->num_projs] = (struct Proj){
            .x = app->enemies[enemy_index].x,
            .y = app->enemies[enemy_index].y,
            .dir = app->enemies[enemy_index].dir,
            .p = '#',
            .alive = true,
            .is_from_player = false,
            .move_delay = 0,
        };
    }

    app->num_projs++;
}
