#include <ncurses.h>
#include <stdlib.h>
#include "app.h"

void render_projs(struct App *app) {
    for (int i = 0; i < app->num_projs; i++) {
        struct Proj *proj = &app->projs[i];

        if (proj->x || proj->y) {
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
                    break;
            }

            attron(COLOR_PAIR(app->colors.green));

            mvprintw(proj->y, proj->x, "%c", proj->p);

            attroff(COLOR_PAIR(app->colors.green));
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
        } 
    }
}

void remove_dead_projs(struct App *app) {
    check_for_dead_projs(app);

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
