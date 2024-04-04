// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "colours.h"
#include "map.h"

extern char *map, *dot_map;
extern int WIDTH, HEIGHT;

void mapUpdatePositions(int old_x, int old_y, int new_x, int new_y) {
    // Since we are using a 1D array, we must use an index instead of (x,y)
    int old_coordinate_index = ((old_y) * WIDTH) + (old_x); // for the old (x,y)
    int new_coordinate_index = ((new_y) * WIDTH) + (new_x);

    char entity = map[old_coordinate_index];
    if (entity == PACMAN) {

        dot_map[old_coordinate_index] = EMPTY;
    }

    map[old_coordinate_index] = dot_map[old_coordinate_index];
    map[new_coordinate_index] = entity;
}

void place_actor(int x, int y, char actor) {
    int index = (y * WIDTH) + x;

    map[index] = actor;
}

char *dotMapMake(char *Map) {
    int size = WIDTH * HEIGHT;
    char *dotMap = (char *) malloc(size * sizeof(char));
    for (int i = 0; i < size; ++i) {

        dotMap[i] = (map[i] == DOT) ? DOT : EMPTY;

    }
    return dotMap;
}


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
}

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