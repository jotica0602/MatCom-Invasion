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
int *terminate;
}EnemyExplosionParams;

// Display welcome screen.
void welcome();
// Draw screen.
void draw_screen(int *pRows, int *pCols, char ** grid);

void print_game_title();
void print_game_over();
void print_level_completed();

EnemyExplosionParams *new_enemy_explosion_params(char **grid, int *pRows, int *pCols, int *terminate);
void clean_enemy_explosions(char **grid, int *pRows, int *pCols);
void *enemy_explosions_cleaner_thread(void *params);

#endif