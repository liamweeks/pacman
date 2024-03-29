// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

#include "defines.h"
#include "game.h"
#include "colours.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern char * map, * dot_map;
extern int HEIGHT;
extern int WIDTH;

void printMap(int height, int width, char *map){

    change_text_colour(WHITE); // white walls
    srand(time(NULL));

    for (int i=0; i < width + 2; i++) { printf("W  "); }
    printf("\n");

    for(int y=0; y<height; y++) {
        printf("W");
        for (int x=0; x < width; x++) {
            char symbol = map[(y * width) + x];

            switch (symbol) {
                case PACMAN:
                    change_text_colour(YELLOW);
                    break;
                case WALL:
                    change_text_colour(WHITE);
                    break;
                case GHOST:
                    // Randomly choose between pink and blue
                    if (rand() % 2 == 0) {
                        change_text_colour(PINK);
                    } else {
                        change_text_colour(BLUE);
                    }
                    break;
                case DOT:
                    change_text_colour(YELLOW);
                    break;
                default:
                    // Handle any other symbols here
                    break;
            }


            printf("%3c", map[(y * width) + x]);
        }
        change_text_colour(WHITE); // walls will always be white
        printf("  W");
        printf("\n");
    }

    for (int i=0; i < width + 2; i++) { printf("W  "); }
    printf("\n");
}



int check_win(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    int win =1;
    for(int i=0; i < WIDTH * HEIGHT; i++){
        if (dot_map[i]==DOT){
            win =0;
        }
    }
    //return (win) ? YOU_WIN: KEEP_GOING;


    if (win) {
        return YOU_WIN;
    } else {
        return KEEP_GOING;
    }
}

int check_loss(int pacman_y, int pacman_x, int ghosts_y[NUM_GHOSTS], int ghosts_x[NUM_GHOSTS]) {
    for(int i=0; i<NUM_GHOSTS; ++i){
        if(pacman_x == ghosts_x[i] && pacman_y == ghosts_y[i]){
            return YOU_LOSE;
        }
    }
    return KEEP_GOING;
}