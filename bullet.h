#ifndef BULLET_H
#define BULLET_H
#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"
#include "globals.h"
#include "player.h"

typedef struct Bullet{
    int x;
    int y;
}Bullet;

typedef struct EnemyBulletThreadParams{
    char **grid;
    Bullet ** bullets;
    int *pRows;
    int *bullets_count;
    int *terminate;
}EnemyBulletThreadParams;

// Player bullet constructor.
Bullet *new_player_bullet(int *pPlayerx, int *pPlayery);

// Enemy bullet constructor.
Bullet *new_enemy_bullet(int *pEnemyx, int *pEnemyy);

// Enemy bullets destructor
void free_enemy_bullets(Bullet *bullets[]);

// Enemy bullet generator.
void generate_enemy_bullet(Enemy * enemies[], char **grid, int *pRows, int *pCols, Bullet *bullets[], int *bullet_index, int *bullets_count);

// Enemy bullet movement handler.
void move_enemy_bullets(char **grid, Bullet *bullets[], int *pRows, int *bullets_count);

// Enemy bullet thread parameters.
EnemyBulletThreadParams *new_enemy_bullet_thread_params(char **grid, Bullet *bullets[], int *pRows, int *bullets_count, int *terminate);

// Enemy bullet movement thread.
void *enemy_bullet_thread_function(void *params);

// Player bullet collision handler.
int check_player_bullet_collision(char ** grid, int * pRows, int * pCols, Enemy * enemies[]);

// Player bullet movement handler
void move_player_bullet(Bullet **player_bullet, char **grid, Player *player);

// Checks for enemy bullets collision. If there was any, player will lose 1 live.
int check_enemy_bullet_collision(char **grid, Player *player);
#endif