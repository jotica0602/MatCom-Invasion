#include "globals.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "grid.h"
#include "terminal.h"
#include "visuals.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

int check_game_state(int * pRows){
    if(g_max == *pRows - 3){
        return true;
    }
    if(g_score == NUM_ENEMIES) {
        g_win_flag = true;
        return g_win_flag;
    }
    return false;
}

int main(){
    // initializing random number generator. time(NULL) is the seed.
    srand(time(NULL));  // time(NULL) returns the current time
    set_conio_mode(0);
    int rows, cols;
    rows = 16; 
    cols = 53;
    int direction = rand() % 2;
    // get_terminal_size(&rows, &cols);
    // printf("%d x %d", rows, cols);

    Enemy * enemies[NUM_ENEMIES];
    Player *player = new_player(&rows, &cols);
    char **grid = new_grid(&rows,&cols);
    fill_grid(&rows, &cols, grid, enemies, player);
    // draw_screen(&rows,&cols,grid);
    // exit(0);

    // starts here
    // system("clear");
    welcome();

    // Initializing mutex
    if (pthread_mutex_init(&grid_lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return 1;
    }
    else{
        printf("Mutex thread succesfully created");
    }
    
    // Creating Key Reader Thread
    pthread_t player_movement_thread;
    if (pthread_create(&player_movement_thread, NULL, read_input, (void *)player) != 0){
        fprintf(stderr, "Error creating the keyreader thread.");
        restore_terminal();
        free(player);
        return 1;
    }
    else{
        printf("Keyreader thread successfully created.");
    }



    // Creating Enemy movement Thread
    int terminate = 0; // initializing flag
    EnemiesMovementParams *aemp = new_Enemy_Movement_Params(enemies, &rows, &cols, grid, &direction, &terminate);
    pthread_t enemy_movement_thread;
    if (pthread_create(&enemy_movement_thread, NULL, enemies_movement_thread, (void *)aemp)) {
        fprintf(stderr, "Error creating Enemy Movement thread\n");
        return 1;
    }
    else{
        printf("Enemy movement thread successfully created.");
    }

    Bullet *player_bullet = NULL;
    Bullet *bullets[11];
    int bullet_index = 0;
    int bullets_count = 0;
    for(int i = 0; i<11;i++){
        bullets[i] = NULL;
    }

    pthread_t enemy_bullet_thread;
    EnemyBulletThreadParams *ebtp = new_enemy_bullet_thread_params(grid, bullets, &rows, &bullets_count);
    if(pthread_create(&enemy_bullet_thread, NULL, enemy_bullet_thread_function, (void*)ebtp)){
        fprintf(stderr, "Error creating Enemy Bullet Movement thread\n");
        return 1;
    }
    else{
        printf("Enemy Bullet Movement thread successfully created.");
    }


    // Main Loop
    while (!g_is_over){
        system("clear");

        // draw grid
        pthread_mutex_lock(&grid_lock);
        draw_screen(&rows,&cols,grid);
        pthread_mutex_unlock(&grid_lock);

        // handling player movement
        pthread_mutex_lock(&grid_lock);
        handle_player_movement(player, &rows, &cols, grid);
        pthread_mutex_unlock(&grid_lock);
        
        // player shots trigger
        if(player->can_shoot && player_bullet == NULL){
            pthread_mutex_lock(&grid_lock);
            player_bullet = new_player_bullet(&player->x,&player->y);
            grid[player_bullet->x][player_bullet->y] = '^';
            player->can_shoot = false;
            pthread_mutex_unlock(&grid_lock);
        }

        // enemy bullet generator
        pthread_mutex_lock(&grid_lock);
        generate_enemy_bullet(enemies, grid, &rows, &cols, bullets, &bullet_index, &bullets_count);
        pthread_mutex_unlock(&grid_lock);

        
        pthread_mutex_lock(&grid_lock);
        if(check_player_bullet_collision(grid, &rows, &cols, enemies)){
            grid[player_bullet->x][player_bullet->y] = ' ';
            free(player_bullet);
            player_bullet = NULL;
        }
        pthread_mutex_unlock(&grid_lock);
        
        pthread_mutex_lock(&grid_lock);
        if(player_bullet != NULL && player_bullet->x != 0){
            grid[player_bullet->x][player_bullet->y] = ' ';
            player_bullet->x--;
            grid[player_bullet->x][player_bullet->y] = '^';
        }
        pthread_mutex_unlock(&grid_lock);

        pthread_mutex_lock(&grid_lock);
        if(player_bullet != NULL && player_bullet->x == 0){
            grid[player_bullet->x][player_bullet->y] = ' ';
            free(player_bullet);
            player_bullet = NULL;
        }
        pthread_mutex_unlock(&grid_lock);

        if(grid[player->x-1][player->y] == '*'){
            grid[player->x-1][player->y] = ' ';
            g_lives--;
        }
        if(grid[player->x][player->y] == '*'){
            g_lives--;
        }
        
        g_is_over = check_game_state(&rows);
        if(g_is_over) break;
        usleep(22000);
    }

    terminate = 1;
    pthread_join(enemy_movement_thread, NULL);
    pthread_cancel(enemy_bullet_thread);
    pthread_cancel(player_movement_thread);   // the input handler thread can be safely cancelled
    pthread_mutex_destroy(&grid_lock);
    set_conio_mode(1);
    free_enemy_bullets(bullets);
    free(player_bullet);
    free(player);
    free_enemies(enemies);
    free(aemp);
    free(ebtp);
    free_grid(grid, &rows, &cols);
    printf("end");
    return 0;
}