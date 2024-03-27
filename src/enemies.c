#include <ncurses.h>
#include <stdlib.h>
#include "app.h"

#include <stdlib.h>
#include <time.h>

void change_dir_rand(enum Direction *dir) {
    int random_number = (rand() % (3 - 0 + 1)) + 0;
    switch (random_number) {
        case 0:
            *dir = RIGHT;
            break;
        case 1:
            *dir = LEFT;
            break;
        case 2:
            *dir = DOWN;
            break;
        case 3:
            *dir = UP;
            break;
    }
}

void change_dir_player(struct Enemy *e, struct Player *p) {
    int x_dist = abs(e->x - p->x);
    int y_dist = abs(e->y - p->y);


    if (x_dist > y_dist) {
        if (e->x > p->x) {
            e->dir = LEFT;
        } else {
            e->dir = RIGHT;
        }
    } else {
        if (e->y > p->y) {
            e->dir = UP;
        } else {
            e->dir = DOWN;
        }
    }
}

void move_in_dir(struct Enemy *e) {
    int *x = &e->x;
    int *y = &e->y;

    switch (e->dir) {
        case RIGHT:
            if (*x < COLS - 2) *x += 1;
            break;
        case LEFT:
            if (*x > 0) *x -= 1;
            break;
        case DOWN:
            if (*y < LINES - 1) *y += 1;
            break;
        case UP:
            if (*y > 1) *y -= 1;
            break;
        case NONE: break;
    }
}

void mvrender_enemies(struct App *app) {
    attron(COLOR_PAIR(app->colors.red));

    for (int i = 0; i < app->num_enemies; i++) {
        struct Enemy *e = &app->enemies[i];


        // move
        if (get_elapsed(e->last_enemie_move_time) >= 0.8) {
            //change_dir_rand(&e->dir);
            change_dir_player(e, &app->player);
            move_in_dir(e);
            
            gettimeofday(&e->last_enemie_move_time, NULL);
        }

        mvprintw(e->y, e->x, "%s", e->p1);
        mvprintw(e->y - 1, e->x, "%s", e->p1);
    }

    attroff(COLOR_PAIR(app->colors.red));
}

void remove_dead_enemies(struct App *app) {
    struct Enemy *new_enemies = (struct Enemy *)malloc(app->num_enemies * sizeof(struct Enemy));
    int new_num_enemies = 0;

    for (int i = 0; i < app->num_enemies; i++) {
        if (app->enemies[i].alive) {
            new_enemies[new_num_enemies] = app->enemies[i];

            new_num_enemies++;
        }
    }

    app->num_enemies = new_num_enemies;
    free(app->enemies);
    app->enemies = new_enemies;
}

void spawn_enemie(struct App *app) {
    app->enemies = (struct Enemy *)realloc(app->enemies, (app->num_enemies + 1) * sizeof(struct Enemy));

    struct Enemy *e = &app->enemies[app->num_enemies];
    int player_y = app->player.y;
    int player_x = app->player.x;

    int y = player_y; 
    int x = player_x;
    while 
        (
         (
          y - 1 == player_y ||
          y - 1 == player_y - 1 ||
          y - 1 == player_y - 2 ||
          y - 1 == player_y - 3 ||
          y - 1 == player_y + 1 ||
          y - 1 == player_y + 2 ||
          y - 1 == player_y + 3 ||
          y == player_y ||
          y == player_y - 1 ||
          y == player_y - 2 ||
          y == player_y - 3 ||
          y == player_y + 1 ||
          y == player_y + 2 ||
          y == player_y + 3
         ) &&
         (
          x == player_x ||
          x == player_x - 1 ||
          x == player_x - 2 ||
          x == player_x - 3 ||
          x == player_x + 1 ||
          x == player_x + 2 ||
          x == player_x + 3 ||
          x + 1 == player_x ||
          x + 1 == player_x - 1 ||
          x + 1 == player_x - 2 ||
          x + 1 == player_x - 3 ||
          x + 1 == player_x + 1 ||
          x + 1 == player_x + 2 ||
          x + 1 == player_x + 3 ||
          x + 2 == player_x ||
          x + 2 == player_x - 1 ||
          x + 2 == player_x - 2 ||
          x + 2 == player_x - 3 ||
          x + 2 == player_x + 1 ||
          x + 2 == player_x + 2 ||
          x + 2 == player_x + 3
         )
        ) 
    {
        x = (rand() % ((COLS - 4) - 2 + 1)) + 2;
        y = (rand() % ((LINES - 2) - 2 + 1)) + 2;
    }


    *e = (struct Enemy){
        .alive = true,
        .dir = NONE,
        .p1 = "###",
        .x = x,
        .y = y,
    };
    gettimeofday(&e->last_enemie_move_time, NULL);

    app->num_enemies++;
}

void enemy_spawn_proj(struct App *app) {
    int time_since_last_shot = get_elapsed(app->last_enemie_shoot_time);
    if (time_since_last_shot >= 0.5) {
        for (int i = 0; i < NUM_ENEMIES_SHOOT_AT_A_TIME; i++) {
            int sei = app->shooter_enemies_indexes[i];

            struct Enemy *e = &app->enemies[sei];
            render_entity_dir_traject(e->y, e->x, e->dir);

            if (time_since_last_shot >= 1.5) {
                spawn_proj(app, false, sei);
            }
        }

        if (time_since_last_shot >= 1.5) {
            for (int i = 0; i < NUM_ENEMIES_SHOOT_AT_A_TIME; i++) {
                int rn = (rand() % (app->num_enemies - 0 + 1)) + 0;
                app->shooter_enemies_indexes[i] = rn;
            }

            gettimeofday(&app->last_enemie_shoot_time, NULL);
        }
    }
}
