#include <stdlib.h>
#include <ncurses.h>
#include "app.h"

void update(struct App *app) {
        int ch = getch();

        switch (ch) {
            case 'k':
                if (app->p.y > 0) app->p.y--;
                
                if (app->prev_char == 'k') {
                    if (app->p.y > 1) app->p.y -= 2;
                }

                app->p.dir = UP;
                app->prev_char = ch;

                break;
            case 'j':
                if (app->p.y < LINES - 1) app->p.y++;

                if (app->prev_char == 'j') {
                    if (app->p.y < LINES - 2) app->p.y += 2;
                }

                app->p.dir = DOWN;
                app->prev_char = ch;

                break;
            case 'h':
                if (app->p.x > 0) app->p.x--;

                if (app->prev_char == 'h') {
                    if (app->p.x > 2) app->p.x -= 3;
                }

                app->p.dir = LEFT;
                app->prev_char = ch;

                break;
            case 'l':
                if (app->p.x < COLS - 1) app->p.x++;

                if (app->prev_char == 'l') {
                    if (app->p.x < COLS - 3) app->p.x += 3;
                }

                app->p.dir = RIGHT;
                app->prev_char = ch;

                break;
            case '0':
                app->p.x = 0;
                break;
            case '$':
                app->p.x = COLS - 1;
                break;
            case 'g':
                app->p.y = 0;
                break;
            case 'G':
                app->p.y = LINES - 1;
                break;
            case ' ': 
            {
                app->projs = (struct Proj *)realloc(app->projs,(app->num_projs + 1) * sizeof(struct Proj));

                app->projs[app->num_projs].x = app->p.x;
                app->projs[app->num_projs].y = app->p.y;
                app->projs[app->num_projs].dir = app->p.dir;
                app->projs[app->num_projs].p = app->p.p;
                app->projs[app->num_projs].alive = true;

                app->num_projs++;
            } break;
            case 'q': // Quit if 'q' is pressed
                app->exit = true;
        }
}
