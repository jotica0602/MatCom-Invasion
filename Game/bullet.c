#include "bullet.h"
#include "globals.h"
#include "sounds.h"
#include "visuals.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void new_enemy_bullet(Bullet *bullet, int enemy_index){ // Next Fit Strat
    bullet->is_active = true;
    bullet->x = enemies[enemy_index].x + 1;
    bullet->y = enemies[enemy_index].y;
    if(overlaps_enemy(bullet->x + 1, bullet->y)) { grid[bullet->x + 1][bullet->y] = grid[bullet->x + 1][bullet->y]; } 
    else { grid[bullet->x + 1][bullet->y] = '*'; }
    enemy_bullet_index = (enemy_bullet_index + 1) % NUM_ENEMIES;
    active_enemy_bullets++;
}

int overlaps_enemy(int x, int y){
    return grid[x][y] == '1' || grid[x][y] == '2' || grid[x][y] == '3' || grid[x][y] == '4';
}

void generate_enemy_bullet(){
    if(active_enemy_bullets == MAX_ENEMY_BULLETS){ // only NUM_ENEMIES bullets are allowed
        return;
    } 
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++){
        if(enemies[i].is_alive && (rand() % 607 == 0)){ // only alive enemies can shoot
            new_enemy_bullet(&enemy_bullets[enemy_bullet_index], i);
            break;
        }
    }
}

void move_enemy_bullets(){
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++){
        if(enemy_bullets[i].is_active && enemy_bullets[i].x == ROWS - 1){
            grid[enemy_bullets[i].x][enemy_bullets[i].y] = ' ';
            enemy_bullets[i].is_active = false;
            active_enemy_bullets--;
            continue;
        } 
        else if(enemy_bullets[i].is_active && enemy_bullets[i].x < ROWS - 1) {
            if(overlaps_enemy(enemy_bullets[i].x, enemy_bullets[i].y)){
                enemy_bullets[i].x++;
                continue;
            } else {
                grid[enemy_bullets[i].x][enemy_bullets[i].y] = overlaps_enemy(enemy_bullets[i].x, enemy_bullets[i].y) ? grid[enemy_bullets[i].x][enemy_bullets[i].y] : ' ';
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
            grid[enemies[i].x][enemies[i].y] = 'X';             // set X for animation
            play_sound(ENEMY_KILLED);
            update_score(enemies[i].type);
            living_enemy_count--;
            draw_screen();                                      // show animation
            usleep(50000);                                      // wait to make it visible
            grid[enemies[i].x][enemies[i].y] = ' ';             // clean it
            return true;
        }
    }

    if(mothership.x == player_bullet.x && mothership.y == player_bullet.y && mothership.is_alive){
        grid[mothership.x][mothership.y] = 'X';                 // set X for animation
        play_sound(ENEMY_KILLED);
        mothership.is_alive = false;
        update_score('5');  
        draw_screen();                                          // set animation
        usleep(50000);                                          // wait to make it visible
        grid[mothership.x][mothership.y] = ' ';                 // clean it
        return true;
    }
    return false;
}

int check_enemy_bullet_collision(){
    int collision = false;
    if(grid[player.x-1][player.y] == '*'){
        grid[player.x-1][player.y] = ' ';
        collision = true;
        active_enemy_bullets--;
    }
    return collision;
}