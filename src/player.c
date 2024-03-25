#include <ncurses.h>
#include "app.h"

void render_player(struct App *app) {
    switch (app->p.dir) {
        case UP:
            app->p.p = '^';
            break;
        case DOWN:
            app->p.p = 'v';
            break;
        case LEFT:
            app->p.p = '<';
            break;
        case RIGHT:
            app->p.p = '>';
            break;
        case NONE:
            app->p.p = 'x';
            break;
    }
    mvprintw(app->p.y, app->p.x, "%c", app->p.p);
}
