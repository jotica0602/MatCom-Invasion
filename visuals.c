#include "visuals.h"
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

void draw_screen(int *pRows, int *pCols, char ** grid){
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            // g_max = grid[i][j] == 'M' && grid[i] > g_max? 
            printf("%2c", grid[i][j]);
        }
        printf("\n");
    }
    printf(" Score:%-d-------------------------------------------------------------------------------------------Lives:%d\t\t\t\n" , g_score, g_lives);
}