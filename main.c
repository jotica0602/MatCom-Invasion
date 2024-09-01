#include "globals.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "terminal.h"
#include "visuals.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

int init();                 // Initialize game
void finish_threads();      // Finish all threads
int check_game_state();     // Checks for game over condition or level completion
int initialize_threads();   // Initialize all threads
int ask_user(int mode);     // Asks user 

void print_menu(int mode, int selected_option) {
    draw_screen();
    if (mode) {
        print_level_completed();
        if (selected_option == 1)
            printf("\t\t\t\033[1;37m->\033[0m \033[1;33mPlay Next Level\033[0m\n");
        else
            printf("\t\t\t   \033[1;37mPlay Next Level\033[0m\n");

        if (selected_option == 2)
            printf("\t\t\t\033[1;37m->\033[0m \033[1;31mQuit\033[0m\n");
        else
            printf("\t\t\t   \033[1;310mQuit\033[m\n");
    } else {
        print_game_over();
        if (selected_option == 1)
            printf("\t\t\t\033[1;37m->\033[0m \033[1;33mPlay Again\033[0m\n");
        else
            printf("\t\t\t   Play Again\n");

        if (selected_option == 2)
            printf("\t\t\t\033[1;37m->\033[0m \033[1;31mQuit\033[0m\n");
        else
            printf("\t\t\t   Quit\n");
    }
}

int main(){
    system("clear");
    set_conio_mode(0);  // Disabling console canonical mode
    welcome();              
    while(init()); 
    set_conio_mode(1);  // Enabling console canonical mode
    return 0;
}

int check_game_state(){
    if(g_max == ROWS - 3 || g_lives == 0){
        terminate = 1;
        return true;
    }
    if(living_enemy_count == 0) {
        g_win_flag = true;
        return g_win_flag;
    }
    return false;
}

int ask_user(int mode) {
    int selected_option = 1;
    char key;
    print_menu(mode, selected_option);

    while (1) {
        key = getchar();

        if (key == '\033') {  
            getchar();        
            switch (getchar()) {
                case UP_ARROW:
                    if (selected_option > 1)
                        selected_option--;
                    break;
                case DOWN_ARROW:
                    if (selected_option < 2)
                        selected_option++;
                    break;
            }
        } else if (key == ENTER_KEY) {
            break;
        }

        print_menu(mode, selected_option);
    }
    reset_globals();
    if(selected_option == 1 && mode) { level_up(); }
    else if(selected_option == 1 && !mode) { 
        g_score = 0; 
        g_current_level = 1;    
    }
    return (selected_option == 1) ? 1 : 0;
}

int init(){
    srand(time(NULL));          // Initializing random number generator. time(NULL) is the seed.
    direction = rand() % 2;     // Stablishing a random direction for enemy movement
    initialize_player();        // Initializing player properties
    initialize_player_bullet();
    initialize_enemy_bullets();
    set_up_grid();              // Placing enemies and player
    
    initialize_threads();
    // get_terminal_size(&rows, &cols);

    // Everything starts here
    // Main Loop
    while (!g_is_over){
        pthread_mutex_lock(&grid_lock);     // Entering critical zone
        draw_screen();                      // Draw grid
        process_player_inputs();            // Handle player movement
        
        if(player.can_shoot && !player_bullet.is_active){
            player_bullet.x = player.x - 1;
            player_bullet.y = player.y;
            player_bullet.is_active = true;
            grid[player_bullet.x][player_bullet.y] = '^';
            player.can_shoot = false;
        }

        generate_enemy_bullet();            // Enemy bullet generator
        
        if(check_player_bullet_collision()) { player_bullet.is_active = false; }
        
        if(check_enemy_bullet_collision()) { g_lives--;}
        g_is_over = check_game_state();
        pthread_mutex_unlock(&grid_lock);   // Leaving critical zone
        usleep(30000);
    }

    finish_threads();                       // Finishing threads

    if(g_win_flag){
        return ask_user(1);
    } else {
        return ask_user(0);
    }
}

int initialize_threads(){   // Concurrency

    if(pthread_mutex_init(&grid_lock, NULL)){
        printf("Error initializing mutex thread.\n");
        return 1;
    } else {
        printf("Mutex thread successfully initialized.\n");
    }

    if(pthread_create(&player_input_thread, NULL, read_input, NULL)){
        printf("Error creating player input thread.\n");
        return 1;
    } else {
        printf("Player input thread successfully created.\n");
    }

    if(pthread_create(&enemy_movement_thread, NULL, enemies_movement_routine, NULL)){
        printf("Error creating enemy movement thread.\n");
        return 1;
    } else {
        printf("Enemy movement thread successfully created.\n");
    }

    if(pthread_create(&player_bullet_movement_thread, NULL, player_bullet_movement_routine, NULL)){
        printf("Error creating player bullet movement thread.\n");
        return 1;
    } else {
        printf("Player bullet movement thread successfully created.\n");
    }

    if(pthread_create(&enemy_bullet_movement_thread, NULL, enemy_bullet_routine, NULL)){
        printf("Error creating enemy bullet movement thread.\n");
        return 1;
    } else {
        printf("Enemy bullet movement thread successfully created.\n");
    }

    if(pthread_create(&enemy_explosion_cleaner_thread, NULL, enemy_explosions_routine, NULL)){
        printf("Error creating enemy explosion cleaner thread.\n");
        return 1;
    } else {
        printf("Enemy explosion cleaner thread successfully created.\n");
    }
}

void finish_threads(){
    terminate = 1;
    pthread_cancel(player_input_thread);
    pthread_join(enemy_movement_thread, NULL);
    pthread_join(enemy_bullet_movement_thread, NULL);
    pthread_join(enemy_explosion_cleaner_thread, NULL);
    pthread_join(player_bullet_movement_thread, NULL);
    pthread_mutex_destroy(&grid_lock);
}
