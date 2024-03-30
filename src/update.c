#include <ncurses.h>
#include "app.h"

void update(struct App *app) {
    int ch = getch();

    switch (ch) {
        case 'k':
        case 'w':
        case KEY_UP:
            if (app->player.y > 0) app->player.y--;

            if (app->prev_char == ch) {
                if (app->player.y > 1) app->player.y -= 2;
            }

            app->player.dir = UP;
            app->prev_char = ch;

            break;
        case 'j':
        case 's':
        case KEY_DOWN:
            if (app->player.y < LINES - 1) app->player.y++;

            if (app->prev_char == ch) {
                if (app->player.y < LINES - 2) app->player.y += 2;
            }

            app->player.dir = DOWN;
            app->prev_char = ch;

            break;
        case 'h':
        case 'a':
        case KEY_LEFT:
            if (app->player.x > 0) app->player.x--;

            if (app->prev_char == ch) {
                if (app->player.x > 2) app->player.x -= 3;
            }

            app->player.dir = LEFT;
            app->prev_char = ch;

            break;
        case 'l':
        case 'd':
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
            if (app->player.wpn == BLAST) {
                spawn_proj_player(app);
            } else if (app->player.wpn == SWORD) {
                app->sword.is_swinging = true;
                app->sword.swing_stage = 0;
            }
            break;
        case KEY_F(1):
            app->show_stats = !app->show_stats;
            break;
        case 'q':
            app->exit = true;
            break;
            
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
        case 'q':
            app->exit = true;
    }
}

void update_wpn_sel(struct App *app, int *selected) {
    int ch = getch();

    switch (ch) {
        case KEY_F(1):
            app->show_stats = !app->show_stats;
            break;
        case 'h':
        case KEY_LEFT:
            if (*selected > 0) { *selected -= 1; }
            break;
        case 'l':
        case KEY_RIGHT:
            if (*selected < 2) { *selected += 1; }
            break;
        case '\n':
        case ' ':
            switch (*selected) {
                case 0:
                    app->player.wpn = SWORD;
                    break;
                case 1:
                    app->player.wpn = BOOK;
                    spawn_proj_player(app);
                    break;
                case 2:
                    app->player.wpn = BLAST;
                    break;
            }
            break;
        case 'q': 
            app->exit = true;
    }
}
