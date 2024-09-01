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


void welcome();                                 // Display welcome screen.
void draw_screen();                             // Draw screen.
void print_game_title();                        // Prints game title
void print_game_over();                         // Prints game over
void print_level_completed();                   // Prints level completed
void clean_enemy_explosions();                  // Enemy explosions cleaner
void *enemy_explosions_routine(void *params);   // Enemy explosions cleanear routine

#endif