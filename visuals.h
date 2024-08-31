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
void draw_screen();

void print_game_title();
void print_game_over();
void print_level_completed();

void clean_enemy_explosions();
void *enemy_explosions_routine(void *params);

#endif