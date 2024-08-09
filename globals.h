
#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#define LEFT 0
#define RIGHT 1
#define NUM_ENEMIES 1

typedef pthread_mutex_t mutex_thread;
typedef pthread_t thread;

// Grid lock mutex
extern mutex_thread grid_lock;
extern int g_max;
extern int g_is_over;
extern int g_score;
extern int g_lives;
extern int g_win_flag;
extern int g_current_level;

void reset_globals();
void level_up();
#endif