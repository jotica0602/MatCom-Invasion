#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define false 0
#define true 1
#define left 0
#define right 1

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

// ==> PLAYER SECTION <== //

typedef struct{
    int y;
    int x;
    int can_shoot;
    int proyectile_is_alive;
    int is_alive;
    int moved_left; 
    int moved_right;
} Player;

Player *new_player(int *rows, int *cols){
    Player *player = (Player *)malloc(sizeof(Player));
    player->y = *cols / 2;
    player->x = *rows - 1;
    player->can_shoot = false;
    player->proyectile_is_alive = false;
    player->is_alive = true;
    player->moved_left = false;
    player->moved_right = false;
    return player;
}

// ==> PLAYER SECTION <== //



// ==> ENEMY SECTION <== //
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
// ==> ENEMY SECTION <== //

void *read_input(void *arg){
    Player *player = (Player *)arg;
    char key;
    while (1){
        key = getchar();
        if (key != EOF){
            switch (key){
            // left
            case 'a':
                player->moved_left = true;
                // printf("%d",player->y);
                // printf("left");
                break;
            // right
            case 'd':
                player->moved_right = true;
                // printf("%d",player->y);
                // printf("right");
                break;
            // shot
            case 'j':
                player->can_shoot = true;
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
    usleep(10000);
    return NULL;
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


void draw_screen(int *pRows, int *pCols, char grid[*pRows][*pCols]){
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            printf("%3c", grid[i][j]);
        }
        printf("\n");
    }
}

void fill_grid(int *pRows, int *pCols, char grid[*pRows][*pCols], int * enemies, Player * player){
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            if (j == 0 || j == *pCols - 1){
                grid[i][j] = '|';
            }
            else if (j < *pCols / 4 || j > (3 * (*pCols) / 4)){
                grid[i][j] = ' ';
            }
            else if (i > *pRows / 2 - 5){
                grid[i][j] = ' ';
            }
            else{
                grid[i][j] = ' ';
                // *enemies++;
            }
        }
    }
    grid[player->x][player->y] = 'A';
}

void handle_player_movement(Player * player, int *pRows, int *pCols, char grid[*pRows][*pCols]){
    int temp = player->y;
    grid[player->x][temp] = ' ';
    if(player->moved_left){
        player->moved_left = false;
        player->y--;
        if(player->y<1){
            player->y = 1;
            return;
        }
    }
    else if(player->moved_right){
        player->moved_right = false;
        player->y++;
        if(player->y > *pCols - 2){
            player->y = *pCols - 2;
            return;
        }
    }
    grid[player->x][player->y] = 'A';
}

typedef struct{
    Enemy *enemy;
    int *pRows;
    int *pCols;
    char **grid;
    int *direction;

}EnemyMovementParams;

void handle_enemy_movement(Enemy *enemy, int *pRows, int *pCols, char grid[*pRows][*pCols], int *direction) {
    
    grid[enemy->x][enemy->y] = ' ';

    // Check for boundary conditions and update direction
    if (enemy->y == 1){
        *direction = right;  
        if(enemy->x < *pRows - 3) enemy->x++;
        enemy->y++;
    } else if (enemy->y == *pCols - 2){
        *direction = left;
        if(enemy->x < *pRows - 3) enemy->x++;
        enemy->y--;
    }

    // Update position based on the current direction
    if (*direction == right) {
        enemy->y++;
    } else if (*direction == left) {
        enemy->y--;
    }
    
    // Place the enemy at the new position
    grid[enemy->x][enemy->y] = 'M';
}


int main(){
    pthread_t input_handler;
    int rows, cols;
    int enemies = 1;
    int direction = 1;

    get_terminal_size(&rows, &cols);
    char grid[rows][cols];
    Player *player = new_player(&rows, &cols);
    
    fill_grid(&rows, &cols, grid, &enemies, player);
    
    // starts here
    system("clear");
    // welcome();

    // Proyectile initialization
    int alx = player->x;
    int aly = player->y + 1;
    int oldx, oldy;

    set_conio_mode(0);

    if (pthread_create(&input_handler, NULL, read_input, (void *)player) != 0){
        fprintf(stderr, "Error creating the keyreader thread.");
        restore_terminal();
        free(player);
        return 1;
    }
    else{
        printf("Keyreader thread successfully created.");
    }


    // ==> REMOVE THIS WHEN YOU FINISH <==
    enemies = 1;
    grid[rows/2][cols/2] = 'M';
    Enemy * enemy =  new_enemy(rows/2, cols/2);
    printf("\n(%d,%d)\n",enemy->x,enemy->y);
    // exit(0);
    // ==> REMOVE THIS WHEN YOU FINISH <==

    while (enemies){
        system("clear");
        
        // draw grid
        draw_screen(&rows,&cols,grid);

        // handling player movement
        handle_player_movement(player, &rows, &cols, grid);

        // enemy movement
        handle_enemy_movement(enemy, &rows, &cols, grid, &direction);

        
        if (player->can_shoot && !player->proyectile_is_alive)
        {
            alx = player->x - 1;
            aly = player->y;
            // printf("%d %d",alx,aly);
            grid[alx][aly] = '^';
            // printf("true");
            player->can_shoot = false;
            player->proyectile_is_alive = true;
        }

        if (player->proyectile_is_alive && grid[alx - 1][aly] == 'M')
        {
            grid[alx][aly] = ' ';
            grid[alx - 1][aly] = ' ';
            player->proyectile_is_alive = false;
            alx = player->x;
            aly = player->y;
            enemies--;
        }

        if (alx == 0)
        {
            grid[alx][aly] = ' ';
            grid[alx - 1][aly] = ' ';
            player->proyectile_is_alive = false;
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
        usleep(10000);
    }

    pthread_join(input_handler, NULL);
    // restore_terminal();
    set_conio_mode(1);
    free(player);
    printf("end");
    return 0;
}