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
int living_enemy_count = NUM_ENEMIES;

void set_up_grid(){
    int index = 0;
    int lbx = ceil((10 * ROWS)/100);        // lower bound for x (10%)
    int ubx = lbx + 5;                      // upper bound for x
    int lby = ceil((23 * COLS)/100);        // lower bound for y (23%)
    int uby = lby + 22;                     // upper bound for y
    int used = false;
    
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            if (j == 0 || j == COLS - 1){
                grid[i][j] = '|';
            }
            else if(i == 0 && j >= 1 && j < COLS - 1){
                grid[i][j] = '-';
            }
            else{
                grid[i][j] = ' ';
            }
        }
    }

    // Place and initialize enemies
    for(int i = lbx ; i < ubx; i++){
        for(int j = lby; j < uby; j++){
            if(!used && index < NUM_ENEMIES){
                if(index <= 10){
                    enemies[index].type = '1';
                    grid[i][j] = enemies[index].type;
                } else if(index > 10 && index <= 32) {
                    enemies[index].type = '2';
                    grid[i][j] = enemies[index].type;
                } else if(index > 32 && index <= 43) {
                    enemies[index].type = '3';
                    grid[i][j] = enemies[index].type;
                } else {
                    enemies[index].type = '4';
                    grid[i][j] = enemies[index].type;
                }
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
    player.moved_left = false;
    player.moved_right = false;
}

void initialize_player_bullet(){
    player_bullet.is_active = false;
}

void initialize_enemy_bullets(){
    for(int i = 0; i < NUM_ENEMIES; i++){
        enemy_bullets[i].is_active = false;
    }
}

void update_score(char enemy_type){
    switch (enemy_type){
        case '1':
            g_score += 30;
            break;
        case '2':
            g_score += 25;
            break;
        case '3':
            g_score += 20;
            break;
        case '4':
            g_score += 10;
            break;
    }
}

void reset_globals(){
    g_max = -__INT32_MAX__;
    g_is_over = false;
    g_lives = 3;
    g_win_flag = false;
    living_enemy_count = NUM_ENEMIES;
    terminate = 0;
}

void level_up(){
    g_current_level++;
}