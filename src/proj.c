#include <ncurses.h>
#include <stdlib.h>
#include "app.h"
#include "book.c"

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
    bool move_circ = (app->book_delay_count >= app->book_delay_target) ? true : false;

    for (int i = 0; i < app->num_projs; i++) {
        struct Proj *proj = &app->projs[i];

        if (proj->x || proj->y) {
            if (app->player.wpn == BOOK && proj->is_from_player) {
                move_proj_in_circ(app, proj, move_circ);
            } else {
                move_proj_in_dir(proj);
            }

            int color = (proj->is_from_player) ? app->colors.green : app->colors.red;

            attron(COLOR_PAIR(color));
            mvprintw(proj->y, proj->x, "%c", proj->p);
            attroff(COLOR_PAIR(color));
        }
    }

    if (!move_circ) {
        app->book_delay_count++;
    } else {
        app->book_delay_count = 0;
    }
}

void check_for_dead_projs(struct App *app) {
    for (int i = 0; i < app->num_projs; i++) {
        struct Proj *proj = &app->projs[i];

        if (proj->x || proj->y) {
            if (
                    (proj->x > COLS || 
                    proj->x < 0 || 
                    proj->y > LINES ||
                    proj->y < 0) && proj->circle_stage == -1
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
                        if (proj->circle_stage == -1) {
                            proj->alive = false;
                        }

                        e->alive = false;
                        app->player.kills++;
                        start_death_anim(&app->deathanim, e->y, e->x);
                    }
                }
            } else if (proj->x == app->player.x && proj->y == app->player.y) {
                player_die(app);
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

bool contains(int *arr, int arr_size, int e) {
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] == e) {
            return true;
        }
    }
    return false;
}

void spawn_proj_player(struct App *app) {
    bool full = false;
    int circle_stage = (app->player.wpn == BOOK) ? 0 : -1;

    if (app->player.wpn == BOOK) {
        int current_cirle_stages[app->num_projs + 1];
        int current_cirle_stages_len = 0;

        for (int i = 0; i < app->num_projs; i++) {
            int circle_stage = app->projs[i].circle_stage;

            if (circle_stage != -1) {
                current_cirle_stages[current_cirle_stages_len] = circle_stage;
                current_cirle_stages_len++;
            }
        }

        for (int i = 0; i < 19; i++) {
            if (!contains(current_cirle_stages, current_cirle_stages_len, i)) {
                circle_stage = i;
                break;
            }
        }

        if (circle_stage == 0 && contains(current_cirle_stages, current_cirle_stages_len, 0)) {
            full = true;
        } else {
            app->book_count++;
        }
    } 

    if (!full) {
        app->projs = (struct Proj *)realloc(app->projs, (app->num_projs + 1) * sizeof(struct Proj));

        app->projs[app->num_projs] = (struct Proj){
            .x = app->player.x,
                .y = app->player.y,
                .dir = app->player.dir,
                .alive = true,
                .p = app->player.p,
                .is_from_player = true,
                .move_delay = 0,
                .circle_stage = circle_stage,
        };

        app->num_projs++;
    }
}
