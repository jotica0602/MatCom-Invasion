#ifndef VISUALS_H
#define VISUALS_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "globals.h"
#include <stdbool.h>


typedef struct EnemyExplosionParams{
char **grid;
int *pRows;
int *pCols;
}EnemyExplosionParams;

// Display welcome screen.
void welcome();
// Draw screen.
void draw_screen(int *pRows, int *pCols, char ** grid);

void my_print();
void my_print2();
void print_ascii_art();

EnemyExplosionParams *new_enemy_explosion_params(char **grid, int *pRows, int *pCols);
void clean_enemy_explosions(char **grid, int *pRows, int *pCols);
void *enemy_explosions_cleaner_thread(void *params);

#endif