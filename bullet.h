#ifndef BULLET_H
#define BULLET_H
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int x;
    int y;
    int is_active;
} Bullet;

// Enemy bullet generator.
void generate_enemy_bullet();

// Enemy bullet movement handler.
void move_enemy_bullets();

// Enemy bullet movement thread.
void *enemy_bullet_routine(void *params);

// Player bullet movement handler
void move_player_bullet();

// Player bullet movement thread
void *player_bullet_movement_routine(void *params);

// Player bullet collision handler.
int check_player_bullet_collision();

// Checks for enemy bullets collision. If there was any, player will lose 1 live.
int check_enemy_bullet_collision();
#endif