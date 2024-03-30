#include "app.h"

void move_proj_in_circ(struct App *app, struct Proj *proj, bool move) {
    proj->move_delay += 1;
    proj->p = 'X';
    int p_y = app->player.y;
    int p_x = app->player.x;


    switch (proj->circle_stage) {
        case 0: 
            // UP
            proj->y = p_y - 4;
            proj->x = p_x;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 1: 
            // UP - UP RIGHT 1
            proj->y = p_y - 3;
            proj->x = p_x + 1;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 2: 
            // UP - UP RIGHT 2
            proj->y = p_y - 3;
            proj->x = p_x + 2;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 3: 
            // UP RIGHT
            proj->y = p_y - 2;
            proj->x = p_x + 3;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 4: 
            // RIGHT - UP RIGHT
            proj->y = p_y - 1;
            proj->x = p_x + 4;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 5:
            // RIGHT
            proj->y = p_y;
            proj->x = p_x + 5;

            if (move) {
                proj->circle_stage++;
            }
            break;
        case 6: 
            // RIGHT - DOWN RIGHT
            proj->y = p_y + 1;
            proj->x = p_x + 4;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 7: 
            // DOWN RIGHT
            proj->y = p_y + 2;
            proj->x = p_x + 3;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 8: 
            // DOWN - DOWN RIGHT 2
            proj->y = p_y + 3;
            proj->x = p_x + 2;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 9: 
            // DOWN - DOWN RIGHT 1
            proj->y = p_y + 3;
            proj->x = p_x + 1;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 10: 
            // DOWN
            proj->y = p_y + 4;
            proj->x = p_x;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 11: 
            // DOWN - DOWN LEFT 1
            proj->y = p_y + 3;
            proj->x = p_x - 1;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 12: 
            // DOWN - DOWN LEFT 2
            proj->y = p_y + 3;
            proj->x = p_x - 2;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 13: 
            // DOWN LEFT
            proj->y = p_y + 2;
            proj->x = p_x - 3;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 14: 
            // LEFT - DOWN LEFT
            proj->y = p_y + 1;
            proj->x = p_x - 4;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 15: 
            // LEFT
            proj->y = p_y;
            proj->x = p_x - 5;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 16: 
            // LEFT - UP LEFT
            proj->y = p_y - 1;
            proj->x = p_x - 4;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 17: 
            // UP LEFT
            proj->y = p_y - 2;
            proj->x = p_x - 3;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 18: 
            // UP - UP LEFT 2
            proj->y = p_y - 3;
            proj->x = p_x - 2;

            if (move) {
                proj->circle_stage++;
            }
            break;

        case 19: 
            // UP - UP LEFT 1
            proj->y = p_y - 3;
            proj->x = p_x - 1;

            if (move) {
                proj->circle_stage = 0;
            }
            break;
    }
}
