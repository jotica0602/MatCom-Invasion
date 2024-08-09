#ifndef PLAYER_H
#define PLAYER_H
#include "bullet.h"
#include "globals.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdbool.h>


typedef struct Player{
    int x;
    int y;
    int can_shoot;
    int proyectile_is_alive;
    int is_alive;
    int moved_LEFT; 
    int moved_RIGHT;
} Player;


Player *new_player(int *rows, int *cols);
void *read_input(void *params);
void handle_player_movement(Player * player, int *pRows, int *pCols, char ** grid);

#endif