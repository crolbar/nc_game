#include <ncurses.h>
#include <stdlib.h>
#include "app.h"

void render_projs(struct App *app) {
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

            switch (proj->dir) {
                case UP:
                    proj->y--;
                    break;
                case DOWN:
                    proj->y++;
                    break;
                case LEFT:
                    proj->x -= 2;
                    break;
                case RIGHT:
                    proj->x += 2;
                    break;
                case NONE:
                    break;
            }

            mvprintw(proj->y, proj->x, "%c", proj->p);
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
