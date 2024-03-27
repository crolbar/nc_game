#include <ncurses.h>
#include "app.h"

void start_death_anim(struct DeathAnim *da, int y, int x) {
    da->x_UL = x;
    da->y_UL = y;
    da->x_UR = x + 2;
    da->y_UR = y;
    da->x_DL = x;
    da->y_DL = y + 2;
    da->x_DR = x + 2;
    da->y_DR = y + 2;

    da->is = true;
}

void mvrender_death_anim(struct DeathAnim *da) {
    if (da->is) {
        attron(COLOR_PAIR(2));

        da->y_UL--;
        da->x_UL--;
        mvprintw(da->y_UL, da->x_UL, "%c", da->p1);

        da->y_UR--;
        da->x_UR++;
        mvprintw(da->y_UR, da->x_UR, "%c", da->p1);

        da->y_DL++;
        da->x_DL--;
        mvprintw(da->y_DL, da->x_DL, "%c", da->p1);

        da->y_DR++;
        da->x_DR++;
        mvprintw(da->y_DR, da->x_DR, "%c", da->p1);

        if (da->y_UL < 0 && da->x_UL < 0) {
            da->is = false;
        }

        attroff(COLOR_PAIR(2));
    }
}
