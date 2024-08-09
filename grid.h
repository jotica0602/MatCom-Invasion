#ifndef GRID_H
#define GRID_H
#include <stdlib.h>
#include <math.h>
#include "enemy.h"
#include "player.h"

// Grid Constructor.
char **new_grid(int *pRows, int *pCols);
// Grid destructor/
void free_grid(char ** grid, int *pRows, int *pCols);
// Place player and enemy/
void fill_grid(int *pRows, int *pCols, char ** grid, Enemy ** enemies, Player * player);
#endif