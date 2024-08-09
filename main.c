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
    if(g_max == *pRows - 3 || g_lives == 0){
        return true;
    }
    if(g_score == NUM_ENEMIES) {
        g_win_flag = true;
        return g_win_flag;
    }
    return false;
}

int initialize_threads(thread *enemy_movement_thread, thread *player_input_thread, thread *enemy_bullet_thread, thread *enemy_explosion_cleaner_thread, EnemiesMovementParams *emp, EnemyBulletThreadParams *ebp, EnemyExplosionParams *eep, Player *player){

    if (pthread_mutex_init(&grid_lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex.\n");
        return 1;
    }else{printf("Mutex thread succesfully created.\n");}

    if(pthread_create(player_input_thread, NULL, read_input, (void *)player) != 0){
        fprintf(stderr, "Error creating the keyreader thread.\n");
        restore_terminal();
        free(player);
        return 1;
    }else{printf("Keyreader thread successfully created.\n");}

    if(pthread_create(enemy_movement_thread, NULL, enemies_movement_thread, (void *)emp)) {
        fprintf(stderr, "Error creating Enemy Movement thread\n");
        return 1;
    }else{printf("Enemy movement thread successfully created.\n");}

    if(pthread_create(enemy_bullet_thread, NULL, enemy_bullet_thread_function, (void*)ebp)){
        fprintf(stderr, "Error creating Enemy Bullet Movement thread\n");
        return 1;
    }
    else{printf("Enemy Bullet Movement thread successfully created.\n");}

    if(pthread_create(enemy_explosion_cleaner_thread, NULL, enemy_explosions_cleaner_thread, (void*)eep)){
        fprintf(stderr, "Error creating Enemy Explosion Cleaner thread\n");
        return 1;
    }
    else{printf("Enemy Explosion Cleaner thread successfully created.\n");}

    return 1;
}

int ask_user(){
    int option;
    printf("\n1 - Keep playing.\n");
    printf("2 - Quit.\n");
    scanf("%d", &option);
    while(option != 1 && option != 2){
        printf("Invalid input.");
        scanf("%d", &option);

    }
    if(option == 1){return 1;}
    else {return 0;}
}

int launch(){
    // initializing random number generator. time(NULL) is the seed.
    // time(NULL) returns the current time
    srand(time(NULL));  
    
    // Declaring threads
    thread enemy_movement_thread;
    thread player_input_thread;
    thread enemy_bullet_thread;
    thread enemy_explosion_cleaner_thread;

    // Enabling Terminal Canonical Mode
    set_conio_mode(0);
    
    // Declaring grid size
    int rows, cols;
    rows = 23;                              //rows
    cols = 80;                              //columns
    int direction = rand() % 2;             //stablishing a random direction for enemy movement
    // get_terminal_size(&rows, &cols);
    

    // initializing enemy array
    Enemy * enemies[NUM_ENEMIES];
    Player *player = new_player(&rows, &cols);      // initializing player
    char **grid = new_grid(&rows,&cols);            // initializing matrix
    fill_grid(&rows, &cols, grid, enemies, player); // placing enemies and player

    // Everything starts here

    int terminate = 0; // <== termination flag for threads
    // Initializing Enemy movement thread params
    EnemiesMovementParams *emp = new_Enemy_Movement_Params(enemies, &rows, &cols, grid, &direction, &terminate);    
    
    // Initializing enemy bullet thread params
    Bullet *player_bullet = NULL;
    Bullet *bullets[NUM_ENEMIES];
    int bullets_count = 0;
    for(int i = 0; i<NUM_ENEMIES;i++){bullets[i] = NULL;}
    EnemyBulletThreadParams *ebp = new_enemy_bullet_thread_params(grid, bullets, &rows, &bullets_count, &terminate);

    int bullet_index = 0;       // this variable will be used to generate enemy bullets

    // Initializing enemy explosion cleaner params
    EnemyExplosionParams *eep = new_enemy_explosion_params(grid, &rows, &cols, &terminate);
    initialize_threads(&enemy_movement_thread, &player_input_thread, &enemy_bullet_thread, &enemy_explosion_cleaner_thread, emp, ebp, eep, player);
    
    // Main Loop
    while (!g_is_over){
        // draw grid
        draw_screen(&rows,&cols,grid);

        // handling player movement
        handle_player_movement(player, &rows, &cols, grid);
        
        // player shots trigger
        if(player->can_shoot && player_bullet == NULL){
            pthread_mutex_lock(&grid_lock);
            player_bullet = new_player_bullet(&player->x,&player->y);
            grid[player_bullet->x][player_bullet->y] = '^';
            player->can_shoot = false;
            pthread_mutex_unlock(&grid_lock);
        }

        // enemy bullet generator
        generate_enemy_bullet(enemies, grid, &rows, &cols, bullets, &bullet_index, &bullets_count);
        
        
        if(check_player_bullet_collision(grid, &rows, &cols, enemies)){
            grid[player_bullet->x][player_bullet->y] = ' ';
            // free(player_bullet);
            player_bullet = NULL;
        }
        
        move_player_bullet(&player_bullet, grid, player);

        if(check_enemy_bullet_collision(grid,player)){ g_lives--;}
        
        g_is_over = check_game_state(&rows);
        if(g_is_over) break;
        usleep(21000);
    }
    
    // Finishing threads and freeing memory
    terminate = 1;
    pthread_join(enemy_movement_thread, NULL);
    pthread_join(enemy_explosion_cleaner_thread,NULL);
    pthread_join(enemy_bullet_thread, NULL);
    pthread_cancel(player_input_thread);   // the input handler thread can be safely cancelled
    pthread_mutex_destroy(&grid_lock);
    set_conio_mode(1);
    free_enemy_bullets(bullets);
    free(player_bullet);
    free(player);
    free_enemies(enemies);
    free(emp);
    free(ebp);
    free(eep);
    free_grid(grid, &rows, &cols);

    if(g_win_flag){
        print_level_completed();
        usleep(2000000);
        reset_globals();
        g_current_level++;
        return ask_user();
    }else {print_game_over(0);
        return 0;
    }
    // printf("end\n"); 
}

int main(){
    system("clear");
    welcome();
    while(launch());
    return 0;
}
