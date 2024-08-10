#include "bullet.h"
#include <stdio.h>
#include <unistd.h>

Bullet * new_player_bullet(int *pPlayerx, int *pPlayery){
    Bullet *player_bullet = (Bullet*)malloc(sizeof(Bullet));
    player_bullet->x = *pPlayerx - 1;
    player_bullet->y = *pPlayery;
    return player_bullet;
}

Bullet * new_enemy_bullet(int *pEnemyx, int *pEnemyy){
    Bullet *player_bullet = (Bullet*)malloc(sizeof(Bullet));
    player_bullet->x = *pEnemyx + 1;
    player_bullet->y = *pEnemyy;
    return player_bullet;
}

void free_enemy_bullets(Bullet *bullets[]){
    for(int i = 0; i < NUM_ENEMIES; i++){
        free(bullets[i]);
    }
}

void generate_enemy_bullet(Enemy * enemies[], char **grid, int *pRows, int *pCols, Bullet *bullets[], int *bullet_index, int *bullets_count){
    pthread_mutex_lock(&grid_lock);
    if(*bullets_count == NUM_ENEMIES) {
        pthread_mutex_unlock(&grid_lock);
        return;} // only NUM_ENEMIES bullets are allowed
    int random;                     // declaring random number
    for(int i = NUM_ENEMIES - 1; i >= 0; i--){
        if(enemies[i]->is_alive && (rand()%607 == 0)){ // only alive enemies can shoot
            grid[enemies[i]->x+1][enemies[i]->y] = grid[enemies[i]->x+1][enemies[i]->y] == 'M' ? 'M' : '*';
            bullets[*bullet_index] = new_enemy_bullet(&enemies[i]->x,&enemies[i]->y);
            *bullet_index = (*bullet_index + 1)%NUM_ENEMIES;
            *bullets_count++;
            // printf("%d", *bullets_count);
            break;
        }
    }
    pthread_mutex_unlock(&grid_lock);
}

void move_enemy_bullets(char **grid, Bullet *bullets[], int *pRows, int *bullets_count){
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(bullets[i] != NULL){
            if(bullets[i]->x == *pRows - 1){
                grid[bullets[i]->x][bullets[i]->y] = ' ';
                bullets[i] = NULL;
                *bullets_count--;
                continue;
            }else{
                grid[bullets[i]->x][bullets[i]->y] = grid[bullets[i]->x][bullets[i]->y] == 'M' ? 'M' : ' ';
                bullets[i]->x++;
                grid[bullets[i]->x][bullets[i]->y] = '*';

            }
        }
    }
}

EnemyBulletThreadParams *new_enemy_bullet_thread_params(char **grid, Bullet *bullets[], int *pRows, int *bullets_count, int *terminate){
    EnemyBulletThreadParams *ebtp = (EnemyBulletThreadParams*)malloc(sizeof(EnemyBulletThreadParams));
    ebtp->grid = grid;
    ebtp->bullets = bullets;
    ebtp->pRows = pRows;
    ebtp->bullets_count = bullets_count;
    ebtp->terminate = terminate;
    return ebtp;
}

void *enemy_bullet_thread_function(void *params){
    EnemyBulletThreadParams *ebtp = (EnemyBulletThreadParams*)params;
    while(!(*ebtp->terminate)){
        pthread_mutex_lock(&grid_lock);
        move_enemy_bullets(ebtp->grid, ebtp->bullets, ebtp->pRows, ebtp->bullets_count);
        pthread_mutex_unlock(&grid_lock);
        usleep(100000);  // wait 0.01 secs before moving again
    }

    return NULL;
}

// Player bullet thread parameters
PlayerBulletThreadParams *new_player_bullet_thread_params(char **grid, Bullet **player_bullet, int *terminate, Player *player){
    PlayerBulletThreadParams *pbp = (PlayerBulletThreadParams*)malloc(sizeof(PlayerBulletThreadParams));
    pbp->grid = grid;
    pbp->player_bullet = player_bullet;
    pbp->terminate = terminate;
    pbp->player = player;
    return pbp;
}

// When you pass a pointer to a function, you are creating a copy of that pointer, so we need to make a pointer to that pointer. 
// This way we can modify the original data.
void move_player_bullet(Bullet **player_bullet, char **grid, Player *player) {
    // pthread_mutex_lock(&grid_lock);
    
    if (*player_bullet != NULL && (*player_bullet)->x == 1) {
        grid[(*player_bullet)->x][(*player_bullet)->y] = ' ';
        free(*player_bullet);
        *player_bullet = NULL;  // Esto ahora modificará el puntero original
        player->can_shoot = false;
    }
    else if (*player_bullet != NULL && (*player_bullet)->x != 0) {
        grid[(*player_bullet)->x][(*player_bullet)->y] = ' ';
        (*player_bullet)->x--;
        grid[(*player_bullet)->x][(*player_bullet)->y] = '^';
    }   
    else{return;}

    // pthread_mutex_unlock(&grid_lock);
}

void *player_bullet_movement_thread_function(void *params){
    PlayerBulletThreadParams *pbp = (PlayerBulletThreadParams*)params;
    while(!(*pbp->terminate)){
        pthread_mutex_lock(&grid_lock);
        move_player_bullet(pbp->player_bullet, pbp->grid, pbp->player);
        pthread_mutex_unlock(&grid_lock);
        usleep(50000);
    }
}

int check_player_bullet_collision(char ** grid, int * pRows, int * pCols, Enemy * enemies[]){
    pthread_mutex_lock(&grid_lock);
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(grid[enemies[i]->x + 1][enemies[i]->y] == '^' && enemies[i]->is_alive){
            enemies[i]->is_alive = false;
            grid[enemies[i]->x][enemies[i]->y] = 'X';
            g_score++;
            pthread_mutex_unlock(&grid_lock);
            return true;
        }
    }
    pthread_mutex_unlock(&grid_lock);
    return false;
}


int check_enemy_bullet_collision(char **grid, Player *player){
    pthread_mutex_lock(&grid_lock);
    int collision = false;
    if(grid[player->x-1][player->y] == '*'){
        grid[player->x-1][player->y] = ' ';
        collision = true;
    }
    if(grid[player->x][player->y] == '*'){
        grid[player->x][player->y] = ' ';
        g_lives--;
        collision = true;
    }
    pthread_mutex_unlock(&grid_lock);
    return collision;
}