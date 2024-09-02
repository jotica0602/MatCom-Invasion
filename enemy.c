#include "enemy.h"
#include "globals.h"
#include "sounds.h"
#include <stdbool.h>
#include <unistd.h>

void handle_enemies_movement() {
    int boundary_flag = false;
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(!enemies[i].is_alive) continue;
        grid[enemies[i].x][enemies[i].y] = ' ';
        enemies[i].y = direction == LEFT ? enemies[i].y - 1 : enemies[i].y + 1;
        
        if(enemies[i].y == 1 || enemies[i].y == COLS - 2){
            boundary_flag = true;
        }
        grid[enemies[i].x][enemies[i].y] = enemies[i].type; 
    }

    if(boundary_flag){
        direction = !direction;
        for(int i = 0; i < NUM_ENEMIES; i++){
            if(!enemies[i].is_alive) continue;
            grid[enemies[i].x][enemies[i].y] = ' ';
            enemies[i].x++;
            enemies[i].y = direction == LEFT ? enemies[i].y - 1 : enemies[i].y + 1;
            grid[enemies[i].x][enemies[i].y] = enemies[i].type; 
            g_max = g_max < enemies[i].x ? enemies[i].x : g_max;
        }
    }

    play_sound(ENEMY_MOVEMENT);
}

void *enemies_movement_routine(void *params){
    while(!terminate){
        pthread_mutex_lock(&grid_lock);
        handle_enemies_movement();
        pthread_mutex_unlock(&grid_lock);
        usleep(500000);
    }
    return NULL;
}