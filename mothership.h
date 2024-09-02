#ifndef MOTHERSHIP_H
#define MOTHERSHIP_H

typedef struct {
    int x;
    int y;
    int is_alive;
    int direction;
} Mothership;

void spawn_mothership();    // Place the mothership at a random boundary
void move_mothership();     // Moves the mothership across the grid
void *mothership_routine(); // Mothership 

#endif