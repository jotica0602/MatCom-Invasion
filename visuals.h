#ifndef VISUALS_H
#define VISUALS_H
#include <stdio.h>
#include <unistd.h>
#include "globals.h"

// Display welcome screen.
void welcome();
// Draw screen.
void draw_screen(int *pRows, int *pCols, char ** grid);
#endif