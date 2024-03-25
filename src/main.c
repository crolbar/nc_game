#include <ncurses.h>
#include <unistd.h>
#include "update.c"
#include "player.c"
#include "proj.c"

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE); // Enable non-blocking input

    struct App app = {
        .p = {
            .x = COLS/2,
            .y = LINES/2,
            .dir = NONE,
            .p = 'x' 
        },
        .projs = NULL,
        .num_projs = 0,
        .prev_char = NULL,
        .exit = false,
    };

    while (!app.exit) {
        mvprintw(0, 1, "p.y: %d p.x: %d", app.p.y, app.p.x);
        mvprintw(1, 1, "max y: %d max x: %d", LINES, COLS);
        mvprintw(2, 1, "projs: %d", app.num_projs);

        update(&app);

        clear();

        render_player(&app);

        remove_dead_projs(&app);
        render_projs(&app);

        usleep(20000);
    }

    endwin();
    return 0;
}

