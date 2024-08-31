#ifndef ENEMY_H
#define ENEMY_H

typedef struct{
    int y;
    int x;
    int is_alive;
} Enemy;

enum Collision{
    LEFT_COLLISION = 0,
    RIGHT_COLLISION = 1,
    NO_COLLISION = 2,
};

// Multiple Enemies movement handling.
void handle_enemies_movement();

// Enemies movement handler routine
void *enemies_movement_routine(void *params);


#endif