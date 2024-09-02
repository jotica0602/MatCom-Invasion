#ifndef ENEMY_H
#define ENEMY_H

typedef struct{
    int y;
    int x;
    int is_alive;
    char type;
} Enemy;

void handle_enemies_movement();                 // Multiple Enemies movement handling.
void *enemies_movement_routine(void *params);   // Enemies movement handler routine
#endif