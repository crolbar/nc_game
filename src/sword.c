#include <ncurses.h>
#include "app.h"


void idle(struct App *app) {
    enum Direction player_dir = app->player.dir;
    struct Sword *s = &app->sword;

    if (app->player.dir == UP || app->player.dir == DOWN) {
        int dY = player_dir == UP ? -1 : 1;
        s->p1 = "|";

        s->size = 4;

        for (int i = 0; i < s->size; i++) {
            s->POS[i].y = app->player.y + dY * (i + 1);
            s->POS[i].x = (player_dir == UP) ? app->player.x - 1 : app->player.x + 1;
        }

    } else if (player_dir == RIGHT || player_dir == LEFT) {
        int dX = player_dir == RIGHT ? 1 : -1;
        s->p1 = "_";

        s->size = 6;

        for (int i = 0; i < s->size; i++) {
            s->POS[i].x = app->player.x + dX * (i + 1);
            s->POS[i].y = (player_dir == RIGHT) ? app->player.y - 1 : app->player.y;
        }
    }
}

void swing(struct App *app) {
    enum Direction player_dir = app->player.dir;
    struct Sword *s = &app->sword;
    int px = app->player.x;
    int py = app->player.y;

    int n0, n1, n2, n3, n4, n5;

    switch (s->swing_stage) {
        case 0:
        case 15: 
            n0 = 2; n1 = 4; n2 = 6; n3 = 8; n4 = 6; n5 = 8;
            break;
        case 1:
        case 14: 
            n0 = 2; n1 = 4; n2 = 5; n3 = 7; n4 = 5; n5 = 7;
            break;
        case 2:
        case 13: 
            n0 = 2; n1 = 3; n2 = 4; n3 = 6; n4 = 4; n5 = 6;
            break;
        case 3:
        case 12: 
            n0 = 2; n1 = 2; n2 = 3; n3 = 5; n4 = 3; n5 = 5;
            break;
        case 4:
        case 11: 
            n0 = 1; n1 = 2; n2 = 3; n3 = 4; n4 = 3; n5 = 4;
            break;
        case 5:
        case 10: 
            n0 = 1; n1 = 1; n2 = 2; n3 = 3; n4 = 2; n5 = 3;
            break;
        case 6:
        case 9: 
            n0 = 1; n1 = 1; n2 = 2; n3 = 2; n4 = 2; n5 = 2;
            break;
        case 7:
        case 8: 
            n0 = 1; n1 = 1; n2 = 1; n3 = 1; n4 = 1; n5 = 1;
            break;
    }

    if (player_dir == UP || player_dir == DOWN) {
        int dY = player_dir == UP ? -1 : 1;

        s->POS[0].y = py + dY * 1;
        s->POS[1].y = py + dY * 2;
        s->POS[2].y = py + dY * 3;
        s->POS[3].y = py + dY * 4;

        if (s->swing_stage < 7) {
            s->p1 = "\\";
        } else if (s->swing_stage == 7) {
            s->p1 = "|";
        } else {
            s->p1 = "/";
        }

        int dX;
        if (s->swing_stage < 8) {
            dX = player_dir == UP ? -1 : 1;
        } else {
            dX = player_dir == UP ? 1 : -1;
        }

        s->POS[0].x = px + dX * n0;
        s->POS[1].x = px + dX * n1;
        s->POS[2].x = px + dX * n2;
        s->POS[3].x = px + dX * n3;

        s->size = 4;
    } else if (player_dir == RIGHT || player_dir == LEFT) {
        int dX = player_dir == RIGHT ? 1 : -1;

        s->POS[0].x = px + dX * 1;
        s->POS[1].x = px + dX * 2;
        s->POS[2].x = px + dX * 3;
        s->POS[3].x = px + dX * 4;
        s->POS[4].x = px + dX * 5;
        s->POS[5].x = px + dX * 6;

        if (s->swing_stage < 7) {
            s->p1 = "\\";
        } else if (s->swing_stage == 7) {
            s->p1 = "_";
        } else {
            s->p1 = "/";
        }

        int dY;
        if (s->swing_stage < 8) {
            dY = player_dir == RIGHT ? 1 : -1;
        } else {
            dY = player_dir == RIGHT ? -1 : 1;
        }

        s->POS[0].y = py + dY * n0;
        s->POS[1].y = py + dY * n1;
        s->POS[2].y = py + dY * n2;
        s->POS[3].y = py + dY * n3;
        s->POS[4].y = py + dY * n4;
        s->POS[5].y = py + dY * n5;

        s->size = 6;
    }


    if (s->swing_stage < 15) {
        s->swing_stage++;
    } else {
        s->swing_stage = 0;
        s->is_swinging = false;
    }
}

void mvrender_sword(struct App *app) {
    struct Sword *s = &app->sword;

    if (s->is_swinging) {
        swing(app);
    } else {
        idle(app);
    }

    attron(COLOR_PAIR(app->colors.green));
    
    for (int i = 0; i < s->size; i++) {
        mvprintw(s->POS[i].y, s->POS[i].x , "%s", s->p1);
    }

    attroff(COLOR_PAIR(app->colors.green));
}

void chech_for_sword_colosion(struct App *app) {
    for (int enemy_i = 0; enemy_i < app->num_enemies; enemy_i++) {
        struct Enemy *e = &app->enemies[enemy_i];

        for (int i = 0; i < app->sword.size; i++) {
            int sx = app->sword.POS[i].x;
            int sy = app->sword.POS[i].y;

            if (
                    (sx == e->x && sy == e->y) ||
                    (sx == e->x + 1 && sy == e->y) ||
                    (sx == e->x + 2 && sy == e->y) ||

                    (sx == e->x && sy == e->y - 1) ||
                    (sx == e->x + 1 && sy == e->y - 1) ||
                    (sx == e->x + 2 && sy == e->y - 1)
               )
            {
                e->alive = false;
                app->player.kills++;
                start_death_anim(&app->deathanim, e->y, e->x);
            }
        }
    }
}
