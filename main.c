#include "globals.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "terminal.h"
#include "visuals.h"
#include "shield.h"
#include "sounds.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>


int init();                                     // Initialize game
void finish_threads();                          // Finish all threads
int check_game_state();                         // Checks for game over condition or level completion
int initialize_threads();                       // Initialize all threads
int ask_user(int mode);                         // Asks user according to the actual game state (game over or level completed) to quit or keep playing 
void print_menu(int mode, int selected_option); // Dynamic selection menu
void update_high_score();                       // Update high score
int read_high_score();                          // Read high score from file

int main(){
    system("clear");
    set_conio_mode(0);  // Disabling console canonical mode
    // welcome();              
    while(init()); 
    set_conio_mode(1);  // Enabling console canonical mode
    return 0;
}

int check_game_state(){
    if(g_max == ROWS - 3 || g_player_hp == 0){
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
    if(!mode){ 
        grid[player.x][player.y] = 'X';
        play_sound(FAIL); 
    }
    else { }
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

int init(){
    srand(time(NULL));          // Initializing random number generator. time(NULL) is the seed.
    direction = rand() % 2;     // Stablishing a random direction for enemy movement
    initialize_player();        // Initializing player properties
    initialize_player_bullet();
    initialize_enemy_bullets();
    initialize_shields();
    initialize_mothership();
    g_high_score = read_high_score();
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
            play_sound(PLAYER_SHOT);
            player.can_shoot = false;
            mothership_count = !mothership.is_alive ? mothership_count + 1 : 0;
        }

        if(mothership_count == 10 && !mothership.is_alive) { 
            spawn_mothership(); 
            mothership_count = 0;
        }

        generate_enemy_bullet();            // Enemy bullet generator
        
        if(player_bullet.is_active && check_player_bullet_collision()) { player_bullet.is_active = false; }
        
        handle_shield_collisions();
        
        if(check_enemy_bullet_collision()) { 
            grid[player.x][player.y] = 'X';                                         // Player dead animation
            play_sound(PLAYER_KILLED);                                              // Play player destruction sound 
            g_player_hp--;                                                          // Decrease player hp
            draw_screen();                                                          // Update screen to see the efect
            usleep(100000);                                                         // Wait 0.1 secs
            if(g_player_hp > 0){ player.y = direction == LEFT ? 6 : COLS - 6; }     // If player ship is still alive place it in other position
        }
        
        g_is_over = check_game_state();
        pthread_mutex_unlock(&grid_lock);   // Leaving critical zone
        usleep(30000);
    }

    finish_threads();                       // Finishing threads

    update_high_score();
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

    if(pthread_create(&mothership_thread, NULL, mothership_routine, NULL)){
        printf("Error creating mothership thread.\n");
        return 1;
    } else {
        printf("Mothership thread successfully created.\n");
    }
}

void finish_threads(){
    terminate = 1;
    pthread_cancel(player_input_thread);
    pthread_join(enemy_movement_thread, NULL);
    pthread_join(enemy_bullet_movement_thread, NULL);
    pthread_join(enemy_explosion_cleaner_thread, NULL);
    pthread_join(player_bullet_movement_thread, NULL);
    pthread_join(mothership_thread, NULL);
    pthread_mutex_destroy(&grid_lock);
}

int read_high_score() {
    FILE *file;
    int saved_score = 0;

    // Open the file for reading
    file = fopen(HIGH_SCORE_PATH, "r");

    if (file != NULL) {
        // Try to read the high score from the file
        if (fscanf(file, "%d", &saved_score) != 1) {
            // If reading failed, assume there is no valid high score
            saved_score = 0;
        }
        fclose(file);
    } else {
        // If the file couldn't be opened, assume there is no valid high score
        saved_score = 0;
    }

    return saved_score;
}

void update_high_score() {
    FILE *file;
    int saved_score = 0;

    // Open the file for reading
    file = fopen(HIGH_SCORE_PATH, "r");

    if (file != NULL) {
        // Read the high score from the file, if it exists
        if (fscanf(file, "%d", &saved_score) != 1) {
            saved_score = 0;  // If it could not be read, assume the score is 0
        }
        fclose(file);
    }

    // Compare the existing high score with the current one
    if (g_score > saved_score) {
        // Open the file for writing (overwrite)
        file = fopen(HIGH_SCORE_PATH, "w");
        if (file == NULL) {
            perror("Error opening the file for writing");
            exit(0);
            return;
        }

        // Write the new high score to the file
        fprintf(file, "%d\n", g_score);
        fclose(file);

        printf("High score updated to %d\n", g_score);
    } else {
        printf("The existing high score (%d) is greater or equal. Not updated.\n", saved_score);
    }
}
