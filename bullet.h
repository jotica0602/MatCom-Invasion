#ifndef BULLET_H
#define BULLET_H
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int x;
    int y;
    int is_active;
} Bullet;

                                
void generate_enemy_bullet();                       // Enemy bullet generator.
int overlaps_enemy(int x, int y);                   // Checks for enemy overlapping
void move_enemy_bullets();                          // Enemy bullet movement handler.
void *enemy_bullet_routine(void *params);           // Enemy bullet movement thread.
void move_player_bullet();                          // Player bullet movement handler
void *player_bullet_movement_routine(void *params); // Player bullet movement thread
int check_player_bullet_collision();                // Player bullet collision handler.
int check_enemy_bullet_collision();                 // Checks for enemy bullets collision. If there was any, player will lose 1 live
#endif