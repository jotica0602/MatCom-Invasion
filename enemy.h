#ifndef ENEMY_H
#define ENEMY_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "globals.h"

typedef struct Enemy{
    int y;
    int x;
    int is_alive;
} Enemy;

typedef struct EnemiesMovementParams{
    Enemy **enemies;
    int num_enemies;
    int *pRows;
    int *pCols;
    char **grid;
    int *direction;
    int *terminate; // termination flag
} EnemiesMovementParams;

enum Collision{
    LEFT_COLLISION = 0,
    RIGHT_COLLISION = 1,
    NO_COLLISION = 2,
};

// Enemy constructor.
Enemy *new_enemy(int row, int col);

// Enemy destructor.
void free_enemies(Enemy * enemies[]);

// Enemy movement thread params.
EnemiesMovementParams * new_Enemy_Movement_Params(struct Enemy * enemies[], int *pRows, int *pCols, char **grid, int *direction, int * terminate);

// Single Enemy movement handling.
void handle_enemy_movement(Enemy *enemy, int *pRows, int *pCols, char ** grid, int *direction, enum Collision * collision);

// Multiple Enemies movement handling.
void handle_enemies_movement(Enemy * enemies[], int num_enemies, int *pRows, int *pCols, char **grid, int * direction);

// Enemy left and right collision handler.
void handle_enemy_collision(Enemy * enemies[],int num_enemies, int *pRows, int *pCols, char **grid, int * direction, enum Collision * collision);

// Enemies movement handler thread.
void *enemies_movement_thread(void *params);

#endif