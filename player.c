#include "player.h"
#include "globals.h"
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *read_input(void *params){ // Event
    char key;
    while (!terminate){
        key = getchar();
        if (key == '\033'){
            getchar();
            switch(getchar()){
            // LEFT
            case 'D':
                player.moved_left = true;
                break;
            // RIGHT
            case 'C':
                player.moved_right = true;
                break;
        }
        }
        else if(key == ' '){
            player.can_shoot = true;
        }
        else if(key == 'q'){
            g_is_over = true;    
            terminate = 1;      
            exit(0);
        }
    }
    return NULL;
}

void process_player_inputs(){
    int temp = player.y;
    grid[player.x][temp] = ' ';
    if(player.moved_left){
        player.moved_left = false;
        player.y--;
        if(player.y<1){                // player in left bound
            player.y = 1;
        }
    }
    else if(player.moved_right){
        player.moved_right = false;
        player.y++;
        if(player.y > COLS - 2){     // player in right bound
            player.y = COLS - 2;
        }
    }
    grid[player.x][player.y] = 'A';
}