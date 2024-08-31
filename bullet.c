#include "bullet.h"
#include "globals.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void new_enemy_bullet(Bullet *bullet, int enemy_index){
    bullet->is_active = true;
    bullet->x = enemies[enemy_index].x + 1;
    bullet->y = enemies[enemy_index].y;
    grid[bullet->x + 1][bullet->y] = grid[bullet->x + 1][bullet->y] == 'M' ? 'M' : '*';
    enemy_bullet_index = (enemy_bullet_index + 1) % NUM_ENEMIES;
    active_enemy_bullets++;
}

void generate_enemy_bullet(){
    if(active_enemy_bullets == NUM_ENEMIES){ // only NUM_ENEMIES bullets are allowed
        return;
    } 
    for(int i = NUM_ENEMIES - 1; i >= 0; i--){
        if(enemies[i].is_alive && (rand() % 607 == 0)){ // only alive enemies can shoot
            new_enemy_bullet(&enemy_bullets[enemy_bullet_index], i);
            break;
        }
    }
}

void move_enemy_bullets(){
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(enemy_bullets[i].is_active && enemy_bullets[i].x == ROWS - 1){
            grid[enemy_bullets[i].x][enemy_bullets[i].y] = ' ';
            enemy_bullets[i].is_active = false;
            active_enemy_bullets--;
            continue;
        } 
        else if(enemy_bullets[i].is_active && enemy_bullets[i].x < ROWS - 1) {
            if(grid[enemy_bullets[i].x][enemy_bullets[i].y] == 'M'){
                enemy_bullets[i].x++;
                continue;
            } else {
                grid[enemy_bullets[i].x][enemy_bullets[i].y] = grid[enemy_bullets[i].x][enemy_bullets[i].y] == 'M' ? 'M' : ' ';
                enemy_bullets[i].x++;
                grid[enemy_bullets[i].x][enemy_bullets[i].y] = '*';
            }
        }
    }
}   

void *enemy_bullet_routine(void *params){
    while(!terminate){
        pthread_mutex_lock(&grid_lock);
        move_enemy_bullets();
        pthread_mutex_unlock(&grid_lock);
        usleep(100000);  // wait 0.01 secs before moving again
    }

    return NULL;
}

void move_player_bullet(){
    if (player_bullet.is_active && player_bullet.x == 1) {
        grid[player_bullet.x][player_bullet.y] = ' ';
        player_bullet.is_active = false;
        player.can_shoot = false;
    }
    else if (player_bullet.is_active && player_bullet.x > 1) {
        grid[player_bullet.x][player_bullet.y] = ' ';
        player_bullet.x--;
        grid[player_bullet.x][player_bullet.y] = '^';
    }   
}

void *player_bullet_movement_routine(void *params){
    while(!terminate){
        pthread_mutex_lock(&grid_lock);
        move_player_bullet();
        pthread_mutex_unlock(&grid_lock);
        usleep(50000);
    }
}

int check_player_bullet_collision(){
    
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(player_bullet.is_active && player_bullet.x == enemies[i].x && player_bullet.y == enemies[i].y && enemies[i].is_alive){
            enemies[i].is_alive = false;
            grid[enemies[i].x][enemies[i].y] = 'X';
            g_score++;
            return true;
        }
    }
    return false;
}

int check_enemy_bullet_collision(){
    int collision = false;
    if(grid[player.x-1][player.y] == '*'){
        grid[player.x-1][player.y] = ' ';
        collision = true;
    }
    return collision;
}