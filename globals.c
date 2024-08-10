#include "globals.h"
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t grid_lock;
int g_max;
int g_is_over = false;
int g_score = 0;
int g_lives = 3;
int g_win_flag = false;
int g_current_level = 1;

void reset_globals(){
    g_max = -__INT32_MAX__;
    g_is_over = false;
    g_score = 0;
    g_lives = 3;
    g_win_flag = false;
}

void level_up(){
    g_current_level++;
}