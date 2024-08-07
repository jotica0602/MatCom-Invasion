#include "invaders.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
pthread_mutex_t lock;
int max;
int is_over = FALSE;
int score = 0;
int lives = 5;
enum Collision{
    LEFT_COLLISION = 0,
    RIGHT_COLLISION = 1,
    NO_COLLISION = 2,
};

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
    printf("\t \t \t \tWELCOME SOLDIER \n \n");
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
    int y;
    int x;
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
    player->can_shoot = FALSE;
    player->proyectile_is_alive = FALSE;
    player->is_alive = TRUE;
    player->moved_LEFT = FALSE;
    player->moved_RIGHT = FALSE;
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
                player->moved_LEFT = TRUE;
                // printf("%d",player->y);
                // printf("LEFT");
                break;
            // RIGHT
            case 'd':
                player->moved_RIGHT = TRUE;
                // printf("%d",player->y);
                // printf("RIGHT");
                break;
            // shot
            case 'j':
                player->can_shoot = TRUE;
                break;
            // quit
            case 'q': 
                restore_terminal();
                exit(0);
                break;
            default:
                break;
            }
        }
    }
    // usleep(10000);
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
    enemy->is_alive = TRUE;
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

void handle_enemy_movement(Enemy *enemy, int *pRows, int *pCols, char ** grid, int *direction, enum Collision * collision) {
    max = enemy->is_alive && max < enemy->x ? enemy->x : max;
    if(!enemy->is_alive) {
        grid[enemy->x][enemy->y] = ' ';
        return;
    }
    grid[enemy->x][enemy->y] = ' ';

    // Check for boundary conditions and update direction
    if (enemy->y == 1 || enemy->y == *pCols -2){
        *collision = enemy->y == 1 ? LEFT_COLLISION : RIGHT_COLLISION;
        return;
    }

    // Update position based on the current direction
    if (*direction == RIGHT){
        enemy->y++;
    } else if (*direction == LEFT){
        enemy->y--;
    }
    
    // Place the enemy at the new position
    grid[enemy->x][enemy->y] = 'M';   
}

void handle_enemy_collision(Enemy * enemies[],int num_enemies, int *pRows, int *pCols, char **grid, int * direction, enum Collision * collision){
    
    for(int i = 0; i < NUM_ENEMIES; i++){
        grid[enemies[i]->x][enemies[i]->y] = ' ';
        enemies[i]->x++;
        enemies[i]->y = *collision == LEFT_COLLISION ? enemies[i]->y + 1 : enemies[i]->y - 1;
        grid[enemies[i]->x][enemies[i]->y] = 'M';
        max = enemies[i]->is_alive && max < enemies[i]->x ? enemies[i]->x : max;
    }
    *direction = *collision == LEFT_COLLISION ? RIGHT : LEFT;
    *collision = NO_COLLISION;
}


void handle_enemies_movement(Enemy * enemies[], int num_enemies, int *pRows, int *pCols, char **grid, int * direction) {
    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);
}


void * enemies_movement_thread(void *params){
    AllEnemiesMovementParams *aemp = (AllEnemiesMovementParams*)params;
    while(!(*(aemp->terminate))){
        handle_enemies_movement(aemp->enemies, aemp->num_enemies, aemp->pRows, aemp->pCols, aemp->grid, aemp->direction);
        // usleep(1000000);
        // usleep(1500000);
        usleep(1500);
    }
    return NULL;
}
// ==> END ENEMY SECTION <== //

void draw_screen(int *pRows, int *pCols, char ** grid){
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            printf("%3c", grid[i][j]);
        }
        printf("\n");
    }
    printf("  Score:%-d-----------------------------------------------------------------------------------------------------------------------------------------------Lives:%d\t\t\t\n" , score, lives);
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
    int ub = ceil((6 * *pRows)/100);        // lower bound for x
    int lowb = ub + 5;                      // upper bound for x
    int lb = ceil((40 * *pCols)/100);       // lower bound for y
    int rb = lb + 11;                       // upper bound for y
    
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            if (j == 0 || j == *pCols - 1){
                // printf("filled");
                grid[i][j] = '|';
            }
            else if (i>= ub && i < lowb && j >= lb && j < rb){
                if(index < NUM_ENEMIES){
                    grid[i][j] = 'M';
                    enemies[index] = new_enemy(i,j);
                    index++;
                }
                else{
                    grid[i][j] = ' ';
                }
            }
            else{
                grid[i][j] = ' ';
                // *enemies++;
            }
        }
    }
    grid[player->x][player->y] = 'A';
}

void handle_player_movement(Player * player, int *pRows, int *pCols, char ** grid){
    int temp = player->y;
    grid[player->x][temp] = ' ';
    if(player->moved_LEFT){
        player->moved_LEFT = FALSE;
        player->y--;
        if(player->y<1){
            player->y = 1;
            return;
        }
    }
    else if(player->moved_RIGHT){
        player->moved_RIGHT = FALSE;
        player->y++;
        if(player->y > *pCols - 2){
            player->y = *pCols - 2;
            return;
        }
    }
    grid[player->x][player->y] = 'A';
}

int check_game_state(int * pRows){
    if(max == *pRows - 3){
        return TRUE;
    }
    return FALSE;
}

int main(){
    set_conio_mode(0);
    int rows, cols;
    rows = 16; 
    cols = 53;
    int direction = RIGHT;
    // get_terminal_size(&rows, &cols);
    printf("%d x %d", rows, cols);

    Enemy * enemies[NUM_ENEMIES];
    Player *player = new_player(&rows, &cols);
    char **grid = new_grid(&rows,&cols);
    fill_grid(&rows, &cols, grid, enemies, player);
    // exit(0);

    // // starts here
    // system("clear");
    // welcome();

    // // Proyectile initialization
    int alx = player->x;
    int aly = player->y + 1;
    int oldx, oldy;


    // Creating Key Reader Thread
    pthread_t input_handler;
    if (pthread_create(&input_handler, NULL, read_input, (void *)player) != 0){
        fprintf(stderr, "Error creating the keyreader thread.");
        restore_terminal();
        free(player);
        return 1;
    }
    else{
        printf("Keyreader thread successfully created.");
    }

    // Initializing mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return 1;
    }
    else{
        printf("Mutex thread succesfully created");
    }

    // Enemy movement Thread
    int terminate = 0; // initializing flag
    pthread_t enemy_movement_thread;
    AllEnemiesMovementParams *aemp = new_Enemy_Movement_Params(enemies, &rows, &cols, grid, &direction, &terminate);
    if (pthread_create(&enemy_movement_thread, NULL, enemies_movement_thread, (void *)aemp)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    else{
        printf("Enemy movement thread successfully created.");
    }

    // Main Loop
    while (!is_over){
        system("clear");

        // draw grid
        pthread_mutex_lock(&lock);
        draw_screen(&rows,&cols,grid);

        // handling player movement
        handle_player_movement(player, &rows, &cols, grid);
        pthread_mutex_unlock(&lock);

        if (player->can_shoot && !player->proyectile_is_alive)
        {
            alx = player->x - 1;
            aly = player->y;
            // printf("%d %d",alx,aly);
            grid[alx][aly] = '^';
            // printf("TRUE");
            player->can_shoot = FALSE;
            player->proyectile_is_alive = TRUE;
        }

        if (player->proyectile_is_alive && grid[alx - 1][aly] == 'M')
        {
            grid[alx][aly] = ' ';
            grid[alx - 1][aly] = ' ';
            player->proyectile_is_alive = FALSE;
            // enemy->is_alive=FALSE;
            alx = player->x;
            aly = player->y;
            score++;
            // enemies--;
        }

        if (alx == 1)
        {
            grid[alx][aly] = ' ';
            grid[alx - 1][aly] = ' ';
            player->proyectile_is_alive = FALSE;
            alx = player->x;
            aly = player->y;
        }

        if (player->proyectile_is_alive)
        {
            oldx = alx;
            grid[oldx][aly] = ' ';
            alx--;
            grid[alx][aly] = '^';
        }
        is_over = check_game_state(&rows);
        if(is_over) {
            break;
        }
        usleep(7000);
    }

    terminate = 1;
    // pthread_join(input_handler, NULL);
    pthread_join(enemy_movement_thread, NULL);
    pthread_cancel(input_handler);              // the input handler thread can be safely cancelled
    pthread_mutex_destroy(&lock);
    set_conio_mode(1);
    free(player);
    free_enemies(enemies);
    free_grid(grid, &rows, &cols);
    printf("end");
    return 0;
}
