#include <stdlib.h>
#include "grid.h"

char **new_grid(int *pRows, int *pCols){
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
    int lbx = ceil((34 * *pRows)/100);        // lower bound for x (6%)
    int ubx = lbx + 5;                      // upper bound for x
    int lby = ceil((32 * *pCols)/100);       // lower bound for y (32%)
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
            else if(i == 0 && j >= 1 && j < *pCols - 1){
                grid[i][j] = '-';
            }
            else{
                grid[i][j] = ' ';
                // printf("%2c",grid[i][j]);
            }
        }
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
    grid[player->x][player->y] = 'A';
}
