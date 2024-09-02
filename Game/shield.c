#include "shield.h"
#include "globals.h"
#include <stdbool.h>

void handle_shield_collisions(){

    // Enemy bullet collision
    for(int i = 0; i < MAX_ENEMY_BULLETS; i++){
        if(!enemy_bullets[i].is_active) continue;
        for(int j = 0; j < NUM_SHIELDS; j++){
            if(shields[j].hp > 0 && shields[j].x == enemy_bullets[i].x && shields[j].y == enemy_bullets[i].y){
                shields[j].hp--;
                enemy_bullets[i].is_active = false;
                grid[shields[j].x][shields[j].y] = shields[j].hp > 0 ? '5' : ' '; 
            }
        }
    }

    // Player bullet collision
    for(int i = 0; i < NUM_SHIELDS; i++){
        if(shields[i].hp > 0 && player_bullet.x == shields[i].x && player_bullet.y == shields[i].y ){
            grid[shields[i].x][shields[i].y] = '5'; 
            player_bullet.is_active = false;
        }
    }
}
