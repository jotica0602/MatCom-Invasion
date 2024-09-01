#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <math.h>
#include "player.h"
#include "enemy.h"
#include "bullet.h"

#define LEFT 0                                  // Left enemy movement direction
#define RIGHT 1                                 // Right enemy movement direction
#define NUM_ENEMIES 55                          // Number of enemies
#define MAX_ENEMY_BULLETS NUM_ENEMIES           // MAX_ENEMY_BULLETS
#define ROWS 16                                 // Grid rows
#define COLS 36                                 // Grid columns
#define UP_ARROW 65
#define DOWN_ARROW 66
#define ENTER_KEY 10

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
extern Bullet enemy_bullets[MAX_ENEMY_BULLETS]; // Enemy bullets array

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
extern int living_enemy_count;                  // Living enemy count


void initialize_player();                       // Initialize player properties
void initialize_player_bullet();                // Initialize player bullet is_active property
void initialize_enemy_bullets();                // Initialize enemy bullets is_active property
void set_up_grid();                             // Place enemies and players
void update_score(char enemy_type);             // Updates score according to the enemy type
void reset_globals();                           // Reset global variables
void level_up();                                // Increases the actual level by 1
#endif