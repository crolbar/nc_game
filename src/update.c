#include <stdlib.h>
#include <ncurses.h>
#include "app.h"

void update(struct App *app) {
    int ch = getch();

    switch (ch) {
        case 'k':
        case KEY_UP:
            if (app->player.y > 0) app->player.y--;

            if (app->prev_char == ch) {
                if (app->player.y > 1) app->player.y -= 2;
            }

            app->player.dir = UP;
            app->prev_char = ch;

            break;
        case 'j':
        case KEY_DOWN:
            if (app->player.y < LINES - 1) app->player.y++;

            if (app->prev_char == ch) {
                if (app->player.y < LINES - 2) app->player.y += 2;
            }

            app->player.dir = DOWN;
            app->prev_char = ch;

            break;
        case 'h':
        case KEY_LEFT:
            if (app->player.x > 0) app->player.x--;

            if (app->prev_char == ch) {
                if (app->player.x > 2) app->player.x -= 3;
            }

            app->player.dir = LEFT;
            app->prev_char = ch;

            break;
        case 'l':
        case KEY_RIGHT:
            if (app->player.x < COLS - 1) app->player.x++;

            if (app->prev_char == ch) {
                if (app->player.x < COLS - 3) app->player.x += 3;
            }

            app->player.dir = RIGHT;
            app->prev_char = ch;

            break;
        case '0':
            app->player.x = 0;
            break;
        case '$':
            app->player.x = COLS - 1;
            break;
        case 'g':
            app->player.y = 0;
            break;
        case 'G':
            app->player.y = LINES - 1;
            break;
        case ' ': 
            {
                app->projs = (struct Proj *)realloc(app->projs,(app->num_projs + 1) * sizeof(struct Proj));

                app->projs[app->num_projs].x = app->player.x;
                app->projs[app->num_projs].y = app->player.y;
                app->projs[app->num_projs].dir = app->player.dir;
                app->projs[app->num_projs].p = app->player.p;
                app->projs[app->num_projs].alive = true;

                app->num_projs++;
            } break;
        case 'P':
            app->show_stats = !app->show_stats;
            break;
        case 'q': // Quit if 'q' is pressed
            app->exit = true;
    }
}

void update_gameover(struct App *app) {
    int ch = getch();

    switch (ch) {
        case 'r' | 'R' | ' ':
            app->player.alive = true;
            *app = init_app();
            break;
        case 'P':
            app->show_stats = !app->show_stats;
            break;
        case 'q': // Quit if 'q' is pressed
            app->exit = true;
    }
}
