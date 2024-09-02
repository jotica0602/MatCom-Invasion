#ifndef VISUALS_H
#define VISUALS_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "globals.h"
#include <stdbool.h>

void display_welcome_screen();                  // Display display_welcome_screen screen.
void draw_screen();                             // Draw screen.
void print_game_title();                        // Prints game title
void print_game_over();                         // Prints game over
void print_level_completed();                   // Prints level completed
void clean_explosions();                        // Explosions cleaner
void *explosions_cleaner_routine(void *params); // Enemy explosions cleanear routine

#endif