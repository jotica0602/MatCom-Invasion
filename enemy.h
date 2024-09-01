#ifndef ENEMY_H
#define ENEMY_H

typedef struct{
    int y;
    int x;
    int is_alive;
    char type;
} Enemy;

enum Collision{
    LEFT_COLLISION = 0,
    RIGHT_COLLISION = 1,
    NO_COLLISION = 2,
};


void handle_enemies_movement();                 // Multiple Enemies movement handling.
void *enemies_movement_routine(void *params);   // Enemies movement handler routine


#endif