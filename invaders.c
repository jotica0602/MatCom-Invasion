#include "globals.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <time.h>

pthread_mutex_t grid_lock;
int g_max;
int g_is_over = false;
int g_score = 0;
int g_lives = 5;
int g_win_flag = false;



// ==> BEGIN TERMINAL SECTION <==
void get_terminal_size(int *rows, int *cols)
{
    struct winsize w;
    // Getting terminal dimensions
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        perror("ioctl");
        return;
    }
    *rows = w.ws_row;
    *cols = w.ws_col / 4;
}


void set_conio_mode(int mode)
{
    static struct termios oldt, newt;
    if (mode == 0){
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else{
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void setup_terminal(){
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Desactivar el modo canónico y el eco
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Configurar el terminal en modo no bloqueante
}

void restore_terminal(){
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON;
    term.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &term);
}

void clear_screen(){
    printf("\033[H\033[J");
}

void welcome(){
    printf("\t \t \t \tWELCOME SOLDIER! \n \n");
    sleep(2);
    printf("\t THE COMMAND PROMPT INVADERS ARE BACK AND EVERYONE NEEDS A HERO \n \n ");
    sleep(2);
    printf("\t \t DEFEND YOUR OPERATING SYSTEM NO MATTER WHAT\n \n");
    sleep(2);
    printf("\t \t \t \tGOOD LUCK!!!");
    sleep(2);
    printf("\n\n\t\t\t  PRESS ANY KEY TO START");
    getchar();
}

// ==> END TERMINAL SECTION <==


// ==> BEGIN PLAYER SECTION <== //
typedef struct{
    int x;
    int y;
    int can_shoot;
    int proyectile_is_alive;
    int is_alive;
    int moved_LEFT; 
    int moved_RIGHT;
} Player;

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

typedef struct{
    int x;
    int y;
}Bullet;

Bullet * new_bullet(int *pPlayerx, int *pPlayery){
    Bullet *player_bullet = (Bullet*)malloc(sizeof(Bullet));
    player_bullet->x = *pPlayerx - 1;
    player_bullet->y = *pPlayery;
    return player_bullet;
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
// ==> END PLAYER SECTION <== //


// ==> BEGIN ENEMY SECTION <== //
typedef struct{
    int y;
    int x;
    int is_alive;
} Enemy;

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

typedef struct {
    Enemy **enemies;
    int num_enemies;
    int *pRows;
    int *pCols;
    char **grid;
    int *direction;
    int *terminate; // termination flag
} AllEnemiesMovementParams;

AllEnemiesMovementParams * new_Enemy_Movement_Params(Enemy * enemies[], int *pRows, int *pCols, char **grid, int *direction, int * terminate){
    AllEnemiesMovementParams *aemp = (AllEnemiesMovementParams*)malloc(sizeof(AllEnemiesMovementParams));
    aemp->direction = direction;
    aemp->enemies = enemies;
    aemp->grid = grid;
    aemp->num_enemies = NUM_ENEMIES;
    aemp->pCols = pCols;
    aemp->pRows = pRows;
    aemp->terminate = terminate;
    return aemp;
}

enum Collision{
    LEFT_COLLISION = 0,
    RIGHT_COLLISION = 1,
    NO_COLLISION = 2,
};

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


void handle_enemies_movement(Enemy * enemies[], int num_enemies, int *pRows, int *pCols, char **grid, int * direction) {
    // pthread_mutex_lock(&lock);
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
    // pthread_mutex_unlock(&lock);
}


void *enemies_movement_thread(void *params){
    AllEnemiesMovementParams *aemp = (AllEnemiesMovementParams*)params;
    while(!(*(aemp->terminate))){
        pthread_mutex_lock(&grid_lock);
        handle_enemies_movement(aemp->enemies, aemp->num_enemies, aemp->pRows, aemp->pCols, aemp->grid, aemp->direction);
        pthread_mutex_unlock(&grid_lock);
        // usleep(100000);
        // usleep(1050000);
        usleep(900000);
    }
    return NULL;
}

void move_enemy_bullets(char **grid, Bullet *bullets[], int *pRows, int *bullets_count){
    for(int i = 0; i < 11; i++){
        if(bullets[i] != NULL){
            if(bullets[i]->x == *pRows - 1){
                grid[bullets[i]->x][bullets[i]->y] = ' ';
                bullets[i] = NULL;
                continue;
            }else{
                grid[bullets[i]->x][bullets[i]->y] = ' ';
                bullets[i]->x++;
                grid[bullets[i]->x][bullets[i]->y] = '*';
            }
        }
    }
    // usleep(4000);
}

typedef struct 
{
    char **grid;
    Bullet ** bullets;
    int *pRows;
    int *bullets_count;
}EnemyBulletThreadParams;

EnemyBulletThreadParams *new_enemy_bullet_thread_params(char **grid, Bullet *bullets[], int *pRows, int *bullets_count){
    EnemyBulletThreadParams *ebtp = (EnemyBulletThreadParams*)malloc(sizeof(EnemyBulletThreadParams));
    ebtp->grid = grid;
    ebtp->bullets = bullets;
    ebtp->pRows = pRows;
    ebtp->bullets_count = bullets_count;
    return ebtp;
}

void *enemy_bullet_thread_function(void *params){
    EnemyBulletThreadParams *ebtp = (EnemyBulletThreadParams*)params;
    while(true){
        pthread_mutex_lock(&grid_lock);
        move_enemy_bullets(ebtp->grid, ebtp->bullets, ebtp->pRows, ebtp->bullets_count);
        pthread_mutex_unlock(&grid_lock);
        usleep(90000);
    }

    return NULL;
}

// ==> END ENEMY SECTION <== //

void draw_screen(int *pRows, int *pCols, char ** grid){
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            printf("%2c", grid[i][j]);
        }
        printf("\n");
    }
    printf(" Score:%-d-------------------------------------------------------------------------------------------Lives:%d\t\t\t\n" , g_score, g_lives);
}

char ** new_grid(int * pRows, int * pCols){
    char ** grid = malloc(*pRows * sizeof(char *));
    for(int j = 0; j < *pRows; j++){
        grid[j] = malloc(*pCols * sizeof(char));
    }
    return grid;
}

void free_grid(char ** grid, int *pRows, int *pCols){
    for(int i = 0; i < *pRows; i++){
        free(grid[i]);
    }
    free(grid);
}

void fill_grid(int *pRows, int *pCols, char ** grid, Enemy ** enemies, Player * player){
    int index = 0;
    int lbx = ceil((10 * *pRows)/100);        // lower bound for x
    int ubx = lbx + 5;                      // upper bound for x
    int lby = ceil((32 * *pCols)/100);       // lower bound for y
    int uby = lby + 22;                       // upper bound for y
    int used = false;
    // printf("\n");
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            if (j == 0 || j == *pCols - 1){
                // printf("filled");
                grid[i][j] = '|';
                // printf("%2c", grid[i][j]);
            }
            else{
                grid[i][j] = ' ';
                // printf("%2c",grid[i][j]);
            }
        }
        // printf("\n");
    }

    for(int i = lbx ; i < ubx; i++){
        for(int j = lby; j < uby; j++){
            if(!used && index < NUM_ENEMIES)
            {
                grid[i][j] = 'M';
                enemies[index] = new_enemy(i,j);
                index++;
                used = true;
            }
            else{used = false;}
        }
    }
    
    // draw_screen(pRows, pCols, grid);
    grid[player->x][player->y] = 'A';
}

void handle_player_movement(Player * player, int *pRows, int *pCols, char ** grid){
    int temp = player->y;
    grid[player->x][temp] = ' ';
    if(player->moved_LEFT){
        player->moved_LEFT = false;
        player->y--;
        if(player->y<1){
            player->y = 1;
            return;
        }
    }
    else if(player->moved_RIGHT){
        player->moved_RIGHT = false;
        player->y++;
        if(player->y > *pCols - 2){
            player->y = *pCols - 2;
            return;
        }
    }
    grid[player->x][player->y] = 'A';
}

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

int check_player_bullet_collision(char ** grid, int * pRows, int * pCols, Enemy * enemies[]){
    for(int i = 0; i < NUM_ENEMIES; i++){
        if(grid[enemies[i]->x + 1][enemies[i]->y] == '^' && enemies[i]->is_alive){
            enemies[i]->is_alive = false;
            grid[enemies[i]->x][enemies[i]->y] = ' ';
            g_score++;
            return true;
        }
    }
    return false;
}

void generate_enemy_bullet(Enemy * enemies[], char **grid, int *pRows, int *pCols, Bullet *bullets[], int *bullet_index, int *bullets_count){
    if(*bullets_count == 11) return;
    int random;
    for(int i = NUM_ENEMIES - 1; i >= 0; i--){
        random = rand();
        if(enemies[i]->is_alive && (rand()%200 == 0) && g_max - enemies[i]->x <= 1 && grid[enemies[i]->x+1][enemies[i]->y] == ' '){
            grid[enemies[i]->x+1][enemies[i]->y] = '*';
            bullets[*bullet_index] = new_bullet(&enemies[i]->x,&enemies[i]->y);
            *bullet_index = (*bullet_index + 1)%11;
            *bullets_count++;
            break;
        }
    }
}

int main(){
    srand(time(NULL));
    set_conio_mode(0);
    int rows, cols;
    rows = 16; 
    cols = 53;
    int direction = rand() % 2;
    // get_terminal_size(&rows, &cols);qq
    // printf("%d x %d", rows, cols);

    Enemy * enemies[NUM_ENEMIES];
    Player *player = new_player(&rows, &cols);
    char **grid = new_grid(&rows,&cols);
    fill_grid(&rows, &cols, grid, enemies, player);
    // exit(0);

    // starts here
    // system("clear");
    // welcome();

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

    // Creating Enemy movement Thread
    int terminate = 0; // initializing flag
    AllEnemiesMovementParams *aemp = new_Enemy_Movement_Params(enemies, &rows, &cols, grid, &direction, &terminate);
    pthread_t enemy_movement_thread;
    if (pthread_create(&enemy_movement_thread, NULL, enemies_movement_thread, (void *)aemp)) {
        fprintf(stderr, "Error creating Enemy Movement thread\n");
        return 1;
    }
    else{
        printf("Enemy movement thread successfully created.");
    }


    // Main Loop
    while (!g_is_over){
        system("clear");
        // terminate = 0;
        // draw grid
        pthread_mutex_lock(&grid_lock);
        draw_screen(&rows,&cols,grid);
        pthread_mutex_unlock(&grid_lock);

        // handling player movement
        pthread_mutex_lock(&grid_lock);
        handle_player_movement(player, &rows, &cols, grid);
        pthread_mutex_unlock(&grid_lock);
        
        if(player->can_shoot && player_bullet == NULL){
            pthread_mutex_lock(&grid_lock);
            player_bullet = new_bullet(&player->x,&player->y);
            grid[player_bullet->x][player_bullet->y] = '^';
            player->can_shoot = false;
            pthread_mutex_unlock(&grid_lock);
        }

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

        if(player_bullet != NULL && player_bullet->x != 0){
            grid[player_bullet->x][player_bullet->y] = ' ';
            player_bullet->x--;
            grid[player_bullet->x][player_bullet->y] = '^';
        }

        if(player_bullet != NULL && player_bullet->x == 0){
            grid[player_bullet->x][player_bullet->y] = ' ';
            free(player_bullet);
            player_bullet = NULL;
        }
        if(grid[player->x-1][player->y] == '*' /*|| grid[player->x][player->y] == '*'*/){
            grid[player->x-1][player->y] = ' ';
            g_lives--;
        }
        if(grid[player->x][player->y] == '*'){
            // grid[player->x-1][player->y] = ' ';
            g_lives--;
        }
        
        g_is_over = check_game_state(&rows);
        if(g_is_over) break;
        usleep(22000);
    }

    terminate = 1;
    // pthread_join(input_handler, NULL);
    pthread_join(enemy_movement_thread, NULL);
    // pthread_cancel(enemy_bullet_thread);
    pthread_cancel(player_movement_thread);   // the input handler thread can be safely cancelled
    pthread_mutex_destroy(&grid_lock);
    set_conio_mode(1);
    free(player_bullet);
    free(player);
    free_enemies(enemies);
    free_grid(grid, &rows, &cols);
    printf("end");
    return 0;
}