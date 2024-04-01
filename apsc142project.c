// APSC 142 Engineering Programming Project Starter Code
// Copyright Sean Kauffman 2024

// don't forget to update your project configuration to select "Emulate terminal in the output console"

// Make sure to include all relevant libraries
#include <stdio.h>
#include <stdlib.h>

// colours.h contains functions to change text colour and read single characters without requiring an enter
#include "colours.h"
// defines.h contains useful definitions to keep your code readable
#include "defines.h"
// map.h, game.h, and ghost.h contain prototypes of functions you must implement
#include "map.h"
#include "game.h"
#include "ghost.h"


// Required for random number generator
#include <time.h>

// These global variables must be used to store map information.
// Almost every function needs these variables, so keeping them as globals helps keep things organized.
// map is a pointer to a dynamically allocated map for displaying to the user
// dot_map is a pointer to a dynamically allocated map for keeping track of what dots are left
char *map = NULL, *dot_map = NULL;
// WIDTH and HEIGHT store the WIDTH and HEIGHT of map, NOT counting outer walls
int WIDTH, HEIGHT;



/**
 * Main entry point into your program.
 * Make sure that main returns appropriate status codes depending on what
 * happens.  The codes you must use are:
 *   NO_ERROR when no error occurs
 *   ERR_NO_MAP when no map file is found
 *   ERR_NO_PACMAN when no pacman is found on the map
 *   ERR_NO_GHOSTS when fewer than 2 ghosts are found on the map
 *
 * Make sure that any allocated memory is freed before returning.
 * @return a status code
 */
int main(void) {
    setbuf(stdout, NULL);
    srand(time(NULL)); // set a seed for the random number generator

    int random1 = rand() % 4;
    int random2 = rand() % 4;


    map = load_map(MAP_NAME, &HEIGHT, &WIDTH);
    dot_map = dotMapMake(map);

    //printf("%d\n", random1);
    //printf("%d\n", random2);
    int pacman_x,pacman_y;
    int ghost_x[NUM_GHOSTS];
    int ghost_y[NUM_GHOSTS];

    //extra variable
    int j = 0;
    int k = -1;
    int l = 0;

    //goes through the map to find pacman and the ghosts position as the index of the map
    for(int i =0; i < WIDTH * HEIGHT; i++){
        if(i % WIDTH == 0){
            j=0;
            k++;
        }
        if(map[i] == GHOST){
            ghost_x[l] = j;
            ghost_y[l] =k;
            l++;
        }
        else if(map[i] == PACMAN){
            pacman_x = j;
            pacman_y = k;
        }
        j++;
    }

    char pacman_direction;
    char ghostdirection[NUM_GHOSTS];

    while(1) {
        printMap(HEIGHT, WIDTH, map);
        printf("\n");
        if(check_win(pacman_y, pacman_x, ghost_y,ghost_x)) {
            break;
        }
        if(check_loss(pacman_y,pacman_x,ghost_y,ghost_x)){
            break;
        }
        pacman_direction = getch();
        //printf("%s", pacman_direction);

/*
        if (pacman_direction == 'w') {
            printf("Going up!");
        } else if (pacman_direction == 's') {
            printf("Going down!");
        } else if (pacman_direction == 'a') {
            printf("Going left!");
        } else if (pacman_direction == 'd') {
            printf("Going right!");
        } else {
            printf("key not mapped to movement");
        }
*/

        //fgetc(stdin);

        /* This function, fgetc() gets the next input character from the provided stream
         * An "input stream" is a place where a user can get input some character(s) and
         * the program will respond to that input.
         *
         * In this example, we are passing in the standard input stream that is available
         * with any C file.
         */

        int temp_x = pacman_x;
        int temp_y = pacman_y;

        move_actor(&pacman_y,&pacman_x,pacman_direction,1);
        mapUpdatePositions(temp_x,temp_y,pacman_x,pacman_y);
        for(int i=0;i<NUM_GHOSTS;++i){

            if(sees_pacman(pacman_y,pacman_x,ghost_y[i],ghost_x[i]) != SEES_NOTHING){
                //ghostdirection[i] = sees_pacman(pacman_y,pacman_x,ghost_y[i],ghost_x[i]);
                // Ghost can see Pacman, so move towards Pacman
                ghostdirection[i] = sees_pacman(pacman_y, pacman_x, ghost_y[i], ghost_x[i]);

                // Move the ghost towards Pacman
                int temp_x = ghost_x[i];
                int temp_y = ghost_y[i];
                if (move_actor(&temp_y, &temp_x, ghostdirection[i], 0) == MOVED_OKAY) {
                    // Check if the ghost has moved onto Pacman's spot
                    if (check_loss(pacman_y, pacman_x, ghost_y, ghost_x)) {
                        // Ghost has eaten Pacman
                        printf("Pacman has been eaten by a ghost!\n");
                        // replace P with G
                        map[pacman_y * WIDTH + pacman_x] = GHOST;
                        break;
                    }

                    // Update the ghost's position
                    ghost_x[i] = temp_x;
                    ghost_y[i] = temp_y;
                    mapUpdatePositions(temp_x, temp_y, ghost_x[i], ghost_y[i]);

                }
            } else {

                // 1. find a random direction in which the ghost travels
                const char possible_directions[] = {UP, DOWN, LEFT, RIGHT};
                char random_dir = possible_directions[rand() % 4];

                // 2. Check that the ghost can go in that direction
                int temp_x = ghost_x[i];
                int temp_y = ghost_y[i];

                int move_dir = move_actor(&temp_y, &temp_x, random_dir, 0);

               while (move_dir == MOVED_WALL || move_dir == MOVED_INVALID_DIRECTION) {

                    random_dir = possible_directions[rand() % 4];

                    move_dir = move_actor(&temp_y, &temp_x, random_dir, 0);
               }
            }




            temp_x = ghost_x[i];
            temp_y = ghost_y[i];
            //move_actor(&ghost_y[i],&ghost_x[i],ghostdirection[i],0);
            move_ghost_randomly(&ghost_x[i], &ghost_y[i], map, WIDTH, HEIGHT);
            mapUpdatePositions(temp_x,temp_y,ghost_x[i],ghost_y[i]);
        }

        if (check_win(pacman_y, pacman_x, ghost_y, ghost_x)) {
            printf("Congratulations! You win!");
            break;
        }

        if (check_loss(pacman_y, pacman_x, ghost_y, ghost_x)) {
            printf("Sorry, you lose.");
            break;
        }
    }
    return NO_ERROR;
}

/**
 * Moves the ghost in a random direction, avoiding walls.
 * @param ghost_x The x-coordinate of the ghost.
 * @param ghost_y The y-coordinate of the ghost.
 * @param map The map data.
 * @param width The width of the map.
 * @param height The height of the map.
 */
