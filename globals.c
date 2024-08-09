#include "globals.h"
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t grid_lock;
int g_max;
int g_is_over = false;
int g_score = 0;
int g_lives = 5;
int g_win_flag = false;
