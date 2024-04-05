// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "colours.h"
#include "map.h"
#include "stdbool.h"

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

char opposite_direction(char direction) {

    switch (direction) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
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

// Function to get a valid random direction that doesn't lead to a wall or out of bounds
char get_valid_random_direction(int ghost_y, int ghost_x) {
    const char possible_directions[] = {UP, DOWN, LEFT, RIGHT};
    char valid_directions[4];
    int valid_count = 0;

    // Check each possible direction for validity
    for (int i = 0; i < 4; i++) {
        int new_y = ghost_y, new_x = ghost_x;
        switch (possible_directions[i]) {
            case UP:    new_y--; break;
            case DOWN:  new_y++; break;
            case LEFT:  new_x--; break;
            case RIGHT: new_x++; break;
        }
        if (is_move_valid(new_y, new_x)) {
            valid_directions[valid_count++] = possible_directions[i];
        }
    }

    if (valid_count == 0) {
        // No valid move found; could return a special value or handle differently
        return 'N'; // Example special value indicating 'No Move'
    }

    // Select a random valid direction
    int random_index = rand() % valid_count;
    return valid_directions[random_index];
}


bool is_move_valid(int y, int x) {
    // Return false if the move is out of bounds
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;

    // Return false if the move would hit a wall
    if (is_wall(y, x)) return false;
    // Otherwise, the move is valid
    return true;
}
