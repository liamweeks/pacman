// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "ghost.h"
#include "map.h"

extern char *map, *dot_map;
extern int WIDTH, HEIGHT;

char sees_pacman(int pacman_y, int pacman_x, int ghost_y, int ghost_x) {
    if (pacman_y == ghost_y){
        if (pacman_x > ghost_x) {
            for (int i = 0; ghost_x + i != pacman_x; i++) {
                if (is_wall(ghost_y, ghost_x + i)) return SEES_NOTHING;
            }
            return RIGHT;
        }
        else {
            for(int i=0; ghost_x+1 != pacman_x; i--){
                if(is_wall(ghost_y,ghost_x+1)) return SEES_NOTHING;
            }
            return LEFT;
        }
    }
    else if (pacman_x == ghost_x) {
        if (pacman_y > ghost_y) {
            for (int i = 0; ghost_y + i != pacman_y; i++) {
                if (is_wall(ghost_y + 1, ghost_x)) return SEES_NOTHING;
            }
            return DOWN;
        } else {
            for (int i = 0; ghost_y + i != pacman_y; i--) {
                if (is_wall(ghost_y + 1, ghost_x)) return SEES_NOTHING;
            }
            return UP;
        }
        }
        return SEES_NOTHING;
    }
