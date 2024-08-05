#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define true 1
#define false 0

// void print_grid(char *grid, int rows, int cols);

void getTerminalSize(int *rows, int *cols) {
    struct winsize w;
    // Obtener las dimensiones del terminal
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return;
    }
    *rows = w.ws_row;
    *cols = w.ws_col/4;
}

void set_conio_mode(int mode) {
    static struct termios oldt, newt;
    if (mode == 0) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
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

void restore_terminal() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON;
    term.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &term);
}

void clear_screen() {
    printf("\033[H\033[J");
}

typedef struct{
    int y;
    int x;
    int can_shoot;
    int proyectile_is_alive;
}Player;

typedef struct{
    int y;
    int x;
}Laser;

void* read_input(void* arg) {
    Player* player = (Player*) arg;
    char key;
    while (1) {
        key = getchar();
        if (key != EOF) {
            switch (key) {
                case 'a':
                    player->y--;
                    // printf("%d",player->y);
                    // printf("left");
                    break;
                case 'd':
                    player->y++;
                    // printf("%d",player->y);
                    // printf("right");
                    break;
                case 'j':
                    player->can_shoot = true;
                    break;
                case 'q': // Salir
                    restore_terminal();
                    exit(0);
                    break;
                default:
                    break;
            }
        }
    }
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



int main()
{
    int rows, cols;
    getTerminalSize(&rows,&cols);
    char grid[rows][cols];
    pthread_t input_handler;


    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
        {
            if(j == 0 || j == cols - 1){
                grid[i][j] = '|';

            }
            else if(j < cols/4 || j > (3 * cols/4)){
                grid[i][j] = ' ';
            }
            else if(i > rows / 2 - 3){
                grid[i][j] = ' ';
            }
            else{
                grid[i][j] = 'M';
            }
        }
        printf("\n");
    }

    int px = rows-1;
    int py = cols/2;
    int pp = false;
    grid[px][py] = 'A';

    printf("starts here:\n");
    system("clear");
    welcome();
    
    // Player memory allocation
    Player* player = (Player*) malloc(sizeof(Player));
    if(!player){
        fprintf(stderr, "Error allocating memory.");
        return 1;
    }

    // Player initialization
    player->y = cols/2;
    player->x = rows-1;
    player->can_shoot = false;
    player->proyectile_is_alive = false;
    int proyectile_loc;

    // Proyectile initialization
    int alx = player->x;
    int aly = player->y+1;
    int oldx, oldy;

    printf("%d", player->y);
    // setup_terminal();
    set_conio_mode(0);

    if(pthread_create(&input_handler, NULL, read_input, (void*) player) != 0){
        fprintf(stderr, "Error creating the keyreader thread.");
        restore_terminal();
        free(player);
        return 1;
    }
    else{
        printf("Keyreader thread successfully created.");
    }


    while(true){
        system("clear");

        // draw grid 
        for(int i = 0; i < rows;i++){
            for (int j = 0; j < cols; j++)
            {
                // printf("i: %d\nj: %d\n",i,j);
                printf("%3c", grid[i][j]);
            }
            printf("\n");
        }

        // player movement
        if(player->y < 1){
            player->y = 1;
            py = player->y;
        }
        else if(player->y > cols-2){
            player->y = cols-2;
            py = player->y;
        }
        else{
            grid[px][py] = ' ';
            py = player->y;
            grid[px][py] = 'A';
        }


        if(player->can_shoot && !player->proyectile_is_alive){
            alx = player->x-1;
            aly = player->y;
            // printf("%d %d",alx,aly);
            grid[alx][aly] = '^';
            // printf("true");
            player->can_shoot = false;
            player->proyectile_is_alive = true;

        }

        if(player->proyectile_is_alive && grid[alx-1][aly] == 'M' || alx == 0){
            grid[alx][aly] = ' ';
            grid[alx-1][aly] = ' ';
            player->proyectile_is_alive = false;
            alx = player->x;
            aly = player->y;
        }
        
        if(player->proyectile_is_alive){
            oldx = alx;
            grid[oldx][aly] = ' ';
            alx--;
            grid[alx][aly] = '^';
        }
        usleep(10000);
    }

    pthread_join(input_handler, NULL);
    restore_terminal();
    set_conio_mode(1);
    free(player);
    printf("end");
    return 0;
}
