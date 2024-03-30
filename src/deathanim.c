#include <ncurses.h>
#include "app.h"

void start_death_anim(struct DeathAnim *da, int y, int x) {
    da->UL.x = x;
    da->UL.y = y;

    da->UR.x = x + 2;
    da->UR.y = y;

    da->DL.x = x;
    da->DL.y = y + 2;

    da->DR.x = x + 2;
    da->DR.y = y + 2;

    da->is = true;
}

void mvrender_death_anim(struct DeathAnim *da) {
    if (da->is) {
        attron(COLOR_PAIR(2));

        da->UL.x--;
        da->UL.y--;
        mvprintw(da->UL.y, da->UL.x, "%c", da->p1);

        da->UR.y--;
        da->UR.x++;
        mvprintw(da->UR.y, da->UR.x, "%c", da->p1);

        da->DL.y++;
        da->DL.x--;
        mvprintw(da->DL.y, da->DL.x, "%c", da->p1);

        da->DR.y++;
        da->DR.x++;
        mvprintw(da->DR.y, da->DR.x, "%c", da->p1);

        if (da->UL.y < 0 && da->UL.x < 0) {
            da->is = false;
        }

        attroff(COLOR_PAIR(2));
    }
}
