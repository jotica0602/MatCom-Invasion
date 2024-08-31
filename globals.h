#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <math.h>
#include "player.h"
#include "enemy.h"
#include "bullet.h"

#define LEFT 0
#define RIGHT 1
#define NUM_ENEMIES 55
#define ROWS 16
#define COLS 36

typedef pthread_mutex_t mutex_thread;
typedef pthread_t thread;

extern int terminate;                           // Threads termination flag
extern mutex_thread grid_lock;                  // Mutex thread 
extern thread player_input_thread;              // Player input thread
extern thread enemy_movement_thread;            // Enemy movement thread
extern thread enemy_bullet_movement_thread;     // Enemy bullet movement thread
extern thread enemy_explosion_cleaner_thread;   // Enemy explosion cleaner thread
extern thread player_bullet_movement_thread;    // Player bullet movement thread

extern char grid[ROWS][COLS];                   // Game grid
extern Player player;                           // Player
extern Bullet player_bullet;                    // Player bullet
extern Enemy enemies[NUM_ENEMIES];              // Enemy array
extern Bullet enemy_bullets[NUM_ENEMIES];       // Enemy bullets array

extern int direction;                           // Enemy direction
extern int g_max;                               // Enemy lowest row
extern int g_is_over;                           // Game over flag
extern int g_score;                             // Actual score
extern int g_lives;                             // Player hp
extern int g_win_flag;                          // Win flag
extern int g_current_level;                     // Current level
extern int active_enemy_bullets;                // Active enemy bullets count
extern int enemy_bullet_index;                  // Actual enemy bullet
extern int boss_count;                          // Boss count: every 5 enemies killed it will appear


void initialize_player();                       // Initialize player properties
void set_up_grid();                               // Place enemies and players
void reset_globals();
void level_up();
#endif