// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include <stdio.h>

// make sure not to modify anything in this extern block
extern "C" {
#include "defines.h"
#include "map.h"
#include "ghost.h"
#include "game.h"
char *map = NULL, *dot_map = NULL;
int WIDTH, HEIGHT;
}

void setup(void) {

}

void teardown(void) {

}


TEST_SUITE_BEGIN("Map tests");


TEST_CASE("Test load_map with valid map file") {
    setup();
    int map_height, map_width;
    char *loaded_map = load_map("map.txt", &map_height, &map_width);
    CHECK(loaded_map != NULL);
    CHECK(map_height == 10);
    CHECK(map_width == 10);
    free(loaded_map);
    teardown();
}

TEST_CASE("Test load_map with non-existent map file") {
    setup();
    int map_height, map_width;
    char *loaded_map = load_map("non_existent_map.txt", &map_height, &map_width);
    CHECK(loaded_map == NULL);
    teardown();
}

TEST_CASE("Test is_wall with valid coordinates") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    CHECK(is_wall(0, 0) == YES_WALL);
    CHECK(is_wall(5, 5) == NOT_WALL);
    free(map);
    teardown();
}

TEST_CASE("Test is_wall with out-of-bounds coordinates") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    CHECK(is_wall(-1, 0) == NOT_WALL);
    CHECK(is_wall(0, -1) == NOT_WALL);
    CHECK(is_wall(HEIGHT, 0) == NOT_WALL);
    CHECK(is_wall(0, WIDTH) == NOT_WALL);
    free(map);
    teardown();
}


TEST_CASE("Test move_actor with valid movement") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    dot_map = dotMapMake(map);
    int pacman_y = 5, pacman_x = 5;
    CHECK(move_actor(&pacman_y, &pacman_x, UP, EAT_DOTS) == MOVED_OKAY);
    CHECK(pacman_y == 4);
    CHECK(pacman_x == 5);
    free(map);
    free(dot_map);
    teardown();
}

TEST_CASE("Test move_actor with invalid movement") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    dot_map = dotMapMake(map);
    int pacman_y = 0, pacman_x = 0;
    CHECK(move_actor(&pacman_y, &pacman_x, UP, EAT_DOTS) == MOVED_INVALID_DIRECTION);
    CHECK(pacman_y == 0);
    CHECK(pacman_x == 0);
    free(map);
    free(dot_map);
    teardown();
}

TEST_SUITE_END();


TEST_SUITE_BEGIN("Ghost tests");


TEST_CASE("Test sees_pacman with pacman in line of sight") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    CHECK(sees_pacman(5, 5, 5, 3) == RIGHT);
    CHECK(sees_pacman(5, 5, 5, 7) == LEFT);
    CHECK(sees_pacman(3, 5, 5, 5) == DOWN);
    CHECK(sees_pacman(7, 5, 5, 5) == UP);
    free(map);
    teardown();
}

TEST_CASE("Test sees_pacman with pacman blocked by wall") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    CHECK(sees_pacman(5, 5, 5, 3) == SEES_NOTHING);
    CHECK(sees_pacman(5, 5, 5, 7) == SEES_NOTHING);
    CHECK(sees_pacman(3, 5, 5, 5) == SEES_NOTHING);
    CHECK(sees_pacman(7, 5, 5, 5) == SEES_NOTHING);
    free(map);
    teardown();
}

TEST_SUITE_END();


TEST_SUITE_BEGIN("Game tests");


TEST_CASE("Test check_win with all dots eaten") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    dot_map = dotMapMake(map);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        dot_map[i] = EMPTY;
    }
    int pacman_y = 5, pacman_x = 5;
    int ghosts_y[NUM_GHOSTS] = {3, 7};
    int ghosts_x[NUM_GHOSTS] = {3, 7};
    CHECK(check_win(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_WIN);
    free(map);
    free(dot_map);
    teardown();
}

TEST_CASE("Test check_win with dots remaining") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    dot_map = dotMapMake(map);
    int pacman_y = 5, pacman_x = 5;
    int ghosts_y[NUM_GHOSTS] = {3, 7};
    int ghosts_x[NUM_GHOSTS] = {3, 7};
    CHECK(check_win(pacman_y, pacman_x, ghosts_y, ghosts_x) == KEEP_GOING);
    free(map);
    free(dot_map);
    teardown();
}

TEST_CASE("Test check_loss with pacman eaten") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    int pacman_y = 5, pacman_x = 5;
    int ghosts_y[NUM_GHOSTS] = {5, 5};
    int ghosts_x[NUM_GHOSTS] = {5, 5};
    CHECK(check_loss(pacman_y, pacman_x, ghosts_y, ghosts_x) == YOU_LOSE);
    free(map);
    teardown();
}

TEST_CASE("Test check_loss with pacman not eaten") {
    setup();
    map = load_map("map.txt", &HEIGHT, &WIDTH);
    int pacman_y = 5, pacman_x = 5;
    int ghosts_y[NUM_GHOSTS] = {3, 7};
    int ghosts_x[NUM_GHOSTS] = {3, 7};
    CHECK(check_loss(pacman_y, pacman_x, ghosts_y, ghosts_x) == KEEP_GOING);
    free(map);
    teardown();
}

TEST_SUITE_END();
