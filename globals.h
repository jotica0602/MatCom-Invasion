
#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#define LEFT 0
#define RIGHT 1
#define NUM_ENEMIES 55

// Grid lock mutex
extern pthread_mutex_t grid_lock;
extern int g_max;
extern int g_is_over;
extern int g_score;
extern int g_lives;
extern int g_win_flag;


#endif