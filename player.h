#ifndef PLAYER_H
#define PLAYER_H

typedef struct{
    int x;
    int y;
    int can_shoot;
    int is_alive;
    int moved_left; 
    int moved_right;
} Player;

void *read_input(void *params);
void process_player_inputs();

#endif