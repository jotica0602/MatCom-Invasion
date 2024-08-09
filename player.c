#include "player.h"

Player *new_player(int *rows, int *cols){
    Player *player = (Player *)malloc(sizeof(Player));
    player->y = *cols / 2;
    player->x = *rows - 1;
    player->can_shoot = false;
    player->proyectile_is_alive = false;
    player->is_alive = true;
    player->moved_LEFT = false;
    player->moved_RIGHT = false;
    return player;
}

void *read_input(void *params){
    Player *player = (Player *)params;
    char key;
    while (1){
        key = getchar();
        if (key != EOF){
            switch (key){
            // LEFT
            case 'a':
                player->moved_LEFT = true;
                // printf("%d",player->y);
                // printf("LEFT");
                break;
            // RIGHT
            case 'd':
                player->moved_RIGHT = true;
                // printf("%d",player->y);
                // printf("RIGHT");
                break;
            // shot
            case 'j':
                player->can_shoot = true;
                break;
            // quit
            case 'q': 
                g_is_over = true;          
                break;
            default:
                break;
            }
        }
    }
    return NULL;
}

void handle_player_movement(Player * player, int *pRows, int *pCols, char ** grid){
    pthread_mutex_lock(&grid_lock);
    int temp = player->y;
    grid[player->x][temp] = ' ';
    if(player->moved_LEFT){
        player->moved_LEFT = false;
        player->y--;
        if(player->y<1){                // player in left bound
            player->y = 1;
        }
    }
    else if(player->moved_RIGHT){
        player->moved_RIGHT = false;
        player->y++;
        if(player->y > *pCols - 2){     // player in right bound
            player->y = *pCols - 2;
        }
    }
    grid[player->x][player->y] = 'A';
    pthread_mutex_unlock(&grid_lock);
}