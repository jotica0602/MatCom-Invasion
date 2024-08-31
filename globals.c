// #include "globals.h"
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "globals.h" 

pthread_mutex_t grid_lock;
thread enemy_movement_thread;
thread player_input_thread;
thread enemy_bullet_movement_thread;
thread enemy_explosion_cleaner_thread;
thread player_bullet_movement_thread;

char grid[ROWS][COLS];
Player player;
Bullet player_bullet;
Enemy enemies[NUM_ENEMIES];
Bullet enemy_bullets[NUM_ENEMIES];

int terminate = 0;
int direction;
int g_max = -__INT32_MAX__;
int g_is_over = false;
int g_score = 0;
int g_lives = 3;
int g_win_flag = false;
int g_current_level = 1;
int active_enemy_bullets = 0;
int enemy_bullet_index = 0;

void set_up_grid(){
    int index = 0;
    int lbx = ceil((10 * ROWS)/100);        // lower bound for x (10%)
    int ubx = lbx + 5;                      // upper bound for x
    int lby = ceil((32 * COLS)/100);        // lower bound for y (32%)
    int uby = lby + 22;                     // upper bound for y
    int used = false;
    // printf("\n");
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            if (j == 0 || j == COLS - 1){
                // printf("filled");
                grid[i][j] = '|';
                // printf("%2c", grid[i][j]);
            }
            else if(i == 0 && j >= 1 && j < COLS - 1){
                grid[i][j] = '-';
            }
            else{
                grid[i][j] = ' ';
                // printf("%2c",grid[i][j]);
            }
        }
    }

    // Place and initialize enemies
    for(int i = lbx ; i < ubx; i++){
        for(int j = lby; j < uby; j++){
            if(!used && index < NUM_ENEMIES)
            {
                grid[i][j] = 'M';
                enemies[index].x = i;
                enemies[index].y = j;
                enemies[index].is_alive = true;
                index++;
                used = true;
            }
            else{used = false;}
        }
    }
    grid[player.x][player.y] = 'A';
}

void initialize_player(){
    player.x = ROWS - 1;
    player.y = COLS / 2;
    player.can_shoot = false;
    player.is_alive = true;
    player.moved_left = false;
    player.moved_right = false;
}

void reset_globals(){
    g_max = -__INT32_MAX__;
    g_is_over = false;
    g_score = 0;
    g_lives = 3;
    g_win_flag = false;
    terminate = 0;
}

void level_up(){
    g_current_level++;
}