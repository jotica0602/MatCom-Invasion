#include "enemy.h"

Enemy *new_enemy(int row, int col){
    Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
    enemy->x = row;
    enemy->y = col;
    enemy->is_alive = true;
    return enemy;
}

void free_enemies(Enemy * enemies[]){
    for(int i = 0; i < NUM_ENEMIES; i++){
        free(enemies[i]);
    }
}

EnemiesMovementParams * new_Enemy_Movement_Params(Enemy * enemies[], int *pRows, int *pCols, char **grid, int *direction, int * terminate){
    EnemiesMovementParams *aemp = (EnemiesMovementParams*)malloc(sizeof(EnemiesMovementParams));
    aemp->direction = direction;
    aemp->enemies = enemies;
    aemp->grid = grid;
    aemp->num_enemies = NUM_ENEMIES;
    aemp->pCols = pCols;
    aemp->pRows = pRows;
    aemp->terminate = terminate;
    return aemp;
}

void handle_enemy_movement(Enemy *enemy, int *pRows, int *pCols, char ** grid, int *direction, enum Collision * collision) {
    // enemy->is_alive = false;
    if(!enemy->is_alive) return;    
    g_max = g_max < enemy->x ? enemy->x : g_max;
    grid[enemy->x][enemy->y] = ' ';
    
    // Check for boundary conditions
    if (enemy->y == 1 || enemy->y == *pCols -2 && enemy->is_alive){
        *collision = enemy->y == 1 ? LEFT_COLLISION : RIGHT_COLLISION;
        return;
    }

    enemy->y = *direction == LEFT ? enemy->y - 1 : enemy->y + 1;
    // Place the enemy in the new position
    grid[enemy->x][enemy->y] = enemy->is_alive ? 'M' : ' ';   
}

void handle_enemies_movement(Enemy * enemies[], int num_enemies, int *pRows, int *pCols, char **grid, int * direction) {
    enum Collision collision = NO_COLLISION;
    if(*direction == RIGHT){
        // loop backwards
        for (int i = NUM_ENEMIES - 1; i >= 0; i--){
            handle_enemy_movement(enemies[i], pRows, pCols, grid, direction, &collision);
            if(collision != NO_COLLISION){
                handle_enemy_collision(enemies, num_enemies, pRows, pCols, grid, direction, &collision);
                break;
            }
        }
    }

    else{
        // loop forward 
        for (int i = 0; i < NUM_ENEMIES; i++){
            handle_enemy_movement(enemies[i], pRows, pCols, grid, direction, &collision);
            if(collision != NO_COLLISION){
                handle_enemy_collision(enemies, num_enemies, pRows, pCols, grid, direction, &collision);
                break;
            }
        }
    }
}

void handle_enemy_collision(Enemy * enemies[],int num_enemies, int *pRows, int *pCols, char **grid, int * direction, enum Collision * collision){
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(!enemies[i]->is_alive) continue;
        grid[enemies[i]->x][enemies[i]->y] = ' ';
        enemies[i]->x++;
        enemies[i]->y = *collision == LEFT_COLLISION ? enemies[i]->y + 1 : enemies[i]->y - 1;
        grid[enemies[i]->x][enemies[i]->y] = 'M';
        g_max = enemies[i]->is_alive && g_max < enemies[i]->x ? enemies[i]->x : g_max;
    }
    *direction = (*direction + 1) % 2;
    *collision = NO_COLLISION;
}

void *enemies_movement_thread(void *params){
    EnemiesMovementParams *aemp = (EnemiesMovementParams*)params;
    while(!(*aemp->terminate)){
        pthread_mutex_lock(&grid_lock);
        handle_enemies_movement(aemp->enemies, aemp->num_enemies, aemp->pRows, aemp->pCols, aemp->grid, aemp->direction);
        pthread_mutex_unlock(&grid_lock);
        usleep(500000);
    }
    return NULL;
}