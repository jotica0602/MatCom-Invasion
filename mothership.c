#include "mothership.h"
#include "globals.h"
#include "visuals.h"
#include <stdbool.h>
#include <unistd.h>

void spawn_mothership(){
    mothership.is_alive = true;
    mothership.direction = rand() % 2;
    if(mothership.direction == RIGHT){
        mothership.x = 1;
        mothership.y = 1;
    } else {
        mothership.x = 1;
        mothership.y = COLS - 2;
    }
    grid[mothership.x][mothership.y] = '6';
}

void move_mothership(){
    if(!mothership.is_alive) return;

    grid[mothership.x][mothership.y] = ' ';
    
    mothership.y = mothership.direction == LEFT ? mothership.y - 1 : mothership.y + 1;
    grid[mothership.x][mothership.y] = '6';
    
    if((mothership.direction == LEFT && mothership.y == 2) || (mothership.direction == RIGHT && mothership.y == COLS - 2)){
        grid[mothership.x][mothership.y] = ' ';
        mothership.is_alive = false;
    }
}

void *mothership_routine(){
    while(!terminate){
        pthread_mutex_lock(&grid_lock);
        move_mothership();
        pthread_mutex_unlock(&grid_lock);
        usleep(350000);
    }
}