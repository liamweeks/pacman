// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

extern char *map, *dot_map;
extern int WIDTH, HEIGHT;

/*void mapUpdatePositions(int old_x, int old_y, int new_x, int new_y) {
    // Since we are using a 1D array, we must use an index instead of (x,y)
    int old_coordinate_index = old_y * WIDTH + old_x; // for the old (x,y)
    int new_coordinate_index = new_y * WIDTH + new_x;

    char entity = map[old_coordinate_index];
    if (entity == PACMAN) {

        dot_map[old_coordinate_index] = EMPTY;
    } else {
        dot_map[old_coordinate_index] = DOT;
    }
    map[old_coordinate_index] = dot_map[old_coordinate_index];
    map[new_coordinate_index] = entity;
}*/

/*void mapUpdatePositions(int old_x, int old_y, int new_x, int new_y) {
    // Since we are using a 1D array, we must use an index instead of (x,y)
    int old_coordinate_index = old_y * WIDTH + old_x; // for the old (x,y)
    int new_coordinate_index = new_y * WIDTH + new_x;

    char entity = map[old_coordinate_index];
    map[old_coordinate_index] = dot_map[old_coordinate_index];
    map[new_coordinate_index] = entity;

    if (entity == PACMAN) {
        dot_map[new_coordinate_index] = EMPTY;
    } else {
        dot_map[new_coordinate_index] = DOT;
    }
}*/

void mapUpdatePositions(int old_x, int old_y, int new_x, int new_y) {
    // Since we are using a 1D array, we must use an index instead of (x,y)
    int old_coordinate_index = old_y * WIDTH + old_x; // for the old (x,y)
    int new_coordinate_index = new_y * WIDTH + new_x;

    char entity = map[old_coordinate_index];
    if (entity == PACMAN) {
        dot_map[old_coordinate_index] = EMPTY; // Clear the dot at the old Pacman position
        map[old_coordinate_index] = EMPTY; // Clear Pacman from the old position
    } else {
        dot_map[old_coordinate_index] = DOT; // Update dot map with dot at old position
        map[old_coordinate_index] = dot_map[old_coordinate_index]; // Update main map with dot at old position
    }

    map[new_coordinate_index] = PACMAN; // Update main map with Pacman at the new position
    dot_map[new_coordinate_index] = EMPTY; // Clear the dot at the new Pacman position
}




char *dotMapMake(char *Map) {
    int size = WIDTH * HEIGHT;
    char *dotMap = (char *) malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {

        dotMap[i] = (map[i] == DOT) ? DOT : EMPTY;

    }
    return dotMap;
}

void move_ghost_randomly(int *ghost_x, int *ghost_y, char *map, int width, int height) {
    // Define the possible directions
    const char possible_directions[] = {UP, DOWN, LEFT, RIGHT};

    // Try a random direction until a valid move is found
    while (1) {
        // Select a random direction
        char random_dir = possible_directions[rand() % 4];

        // Check if the ghost can move in that direction
        int temp_x = *ghost_x;
        int temp_y = *ghost_y;
        if (move_actor(&temp_y, &temp_x, random_dir, 0) == MOVED_OKAY) {
            // Update the ghost's position
            *ghost_x = temp_x;
            *ghost_y = temp_y;
            return;
        }
    }
}

int move_actor(int *y, int *x, char direction, int eat_dots) {
    switch (direction) {
        case UP:
            if (is_wall(*y - 1, *x)) {
                return MOVED_WALL;
            } else if (*y == 0) {
                return MOVED_INVALID_DIRECTION;
            } else {
                int old_x = *x;
                int old_y = *y;
                *y = *y - 1;
                mapUpdatePositions(old_x, old_y, *x, *y);
                return MOVED_OKAY;
            }
        case LEFT:
            if (is_wall(*y, *x - 1)) {
                return MOVED_WALL;
            } else if (*x == 0) {
                return MOVED_INVALID_DIRECTION;
            } else {
                int old_x = *x;
                int old_y = *y;
                *x = *x - 1;
                mapUpdatePositions(old_x, old_y, *x, *y);
                return MOVED_OKAY;
            }
        case DOWN:
            if (is_wall(*y + 1, *x)) {
                return MOVED_WALL;
            } else if (*y == HEIGHT - 1) {
                return MOVED_INVALID_DIRECTION;
            } else {
                int old_x = *x;
                int old_y = *y;
                *y = *y + 1;
                mapUpdatePositions(old_x, old_y, *x, *y);
                return MOVED_OKAY;
            }
        case RIGHT:
            if (is_wall(*y, *x + 1)) {
                return MOVED_WALL;
            } else if (*x == WIDTH - 1) {
                return MOVED_INVALID_DIRECTION;
            } else {
                int old_x = *x;
                int old_y = *y;
                *x = *x + 1;
                mapUpdatePositions(old_x, old_y, *x, *y);
                return MOVED_OKAY;
            }
    }
}


/*
int move_actor(int *y, int *x, char direction, int eat_dots) {
    switch (direction) {
        case UP:
            if (is_wall(*y - 1, *x)) {
                return MOVED_WALL;
            } else if (*y == 0) {
                return MOVED_INVALID_DIRECTION;
            } else {
                *y = *y - 1;

                return MOVED_OKAY;
            }
        case LEFT:
            if (is_wall(*y, *x - 1)) {
                return MOVED_WALL;
            } else if (*x == 0) {
                return MOVED_INVALID_DIRECTION;
            } else {
                *x = *x - 1;
                return MOVED_OKAY;
            }
        case DOWN:
            if (is_wall(*y + 1, *x)) {
                return MOVED_WALL;
            } else if (*y == HEIGHT - 1) {
                return MOVED_INVALID_DIRECTION;
            } else {
                *y = *y + 1;
                return MOVED_OKAY;
            }
        case RIGHT:
            if (is_wall(*y, *x + 1)) {
                return MOVED_WALL;
            } else if (*x == WIDTH - 1) {
                return MOVED_INVALID_DIRECTION;
            } else {
                *x = *x + 1;
                return MOVED_OKAY;
            }
    }
}*/

int is_wall(int y, int x) {
    return (map[((y) * WIDTH) + (x)] == WALL) ? YES_WALL : NOT_WALL;
}

char *load_map(char *filename, int *map_height, int *map_width) {
    //opens file
    FILE *mapFile;
    mapFile = fopen(filename, "r");
    if (mapFile == NULL) {
        return NULL;
    }
    int mapSize = 0;
    *map_width = 0;
    *map_height = 0;

    while (1) {
        char current = getc(mapFile);
        if (current == DOT || current == WALL || current == PACMAN || current == GHOST) {
            mapSize++;
            *map_width = *map_width + 1;
        }
        if (current == '\n') {
            *map_height = *map_height + 1;
        } else if (current == EOF) {
            *map_height = *map_height + 1;
            break;
        }
    }

    *map_width = (*map_width / (*map_height));
    char *mapArray = (char *) malloc(mapSize * sizeof(char));

    fclose(mapFile);
    fopen(filename, "r");

    for (int i = 0; i < mapSize; i++) {
        mapArray[i] = getc(mapFile);
        if (mapArray[i] == ' ' || mapArray[i] == '\n')
            i--;
    }
    mapArray[mapSize] = '\0';
    fclose(mapFile);
    return mapArray;
}
