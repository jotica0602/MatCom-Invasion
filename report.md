# Project Implementation Report: "MatCom Invasion"

## Introduction

The *MatCom Invasion* project is an arcade-style game developed in C, inspired by the classic "Alien Invaders." The aim of this project is to implement and demonstrate fundamental operating system and programming concepts, such as event-driven programming, concurrency with threads, scheduling strategies, and page replacement algorithms. This report details how the project code addresses these key aspects, using a matrix to represent the game field and the event and concurrency logic necessary for a dynamic and smooth gaming experience.

## Game Structure

The game operates on a matrix of size defined by `ROWS` and `COLS`, where:
- **`ROWS`** represents the number of rows (16 in total).
- **`COLS`** represents the number of columns (36 in total).

The matrix is used to represent the game field, where:
- The player's ship (`A`) is positioned at the bottom of the matrix.
- Enemies (`M`) move horizontally and descend in the matrix.
- Player projectiles (`^`) and enemy projectiles (`*`) move vertically.

## Event-Driven Programming

**Event-driven programming** in the project is managed through several key functions that allow real-time interaction:

### `player.h`

- **Function `readinput()`**:
  This function runs in a separate thread to handle user input. It detects keyboard events (such as movement and shooting) and updates the game state based on the player's actions. Running in a separate thread ensures that user input is processed efficiently without blocking other game operations.

### `visuals.h`

- **Function `draw_screen()`**:
  The `draw_screen()` function is responsible for updating the game's display on the console. It clears the screen and redraws the matrix with the current positions of the ship, enemies, projectiles, and other elements. Real-time updating ensures that player actions are immediately reflected on the screen.

## Concurrency with Threads

**Concurrency with threads** is implemented to handle multiple aspects of the game simultaneously:

### `game.h`, `player.h`, `visuals.h`

- **Thread for `readinput()`**:
  A separate thread executes `readinput()`, allowing the game to respond to player actions in parallel with game logic and screen updates.

- **Thread for `explosions_cleaner_routine()`**:
  This routine runs in an independent thread and is responsible for cleaning up explosions and visual effects, ensuring that game logic and screen rendering continue uninterrupted.

## Scheduling Strategies

**Scheduling strategies** are reflected in the logic of enemy movement and their appearance in the game field:

### `enemy.h`, `game.h`

- **Enemy Movement**:
  Enemies move horizontally and descend in the matrix when reaching the screen edges. When an enemy reaches the right edge, all enemies move down and shift to the left. Similarly, upon reaching the left edge, they move down and to the right. This strategy ensures a constant challenge for the player and a dynamic planning of enemy movement.

## Page Replacement Algorithm

The **page replacement algorithm** is reflected in how enemies are positioned and move in the matrix:

### `enemy.h`, `game.h`

- **Enemy Trajectory**:
  Enemies do not always land in the same location. The implemented logic allows enemies to appear in different positions based on available space and player actions. This behavior simulates a page replacement algorithm, ensuring that enemies are positioned dynamically and orderly on the game field.

## Deployment and Execution on Arch Linux

**Deployment and execution on Arch Linux** is managed through the following file:

### `build.sh`

- **Build Script**:
  The `build.sh` script compiles all project files, copies the executable and necessary files to the system binaries, and creates a `highscore.txt` file. Proper configuration of the script allows the game to run by typing `MatComInvasion` in the Arch Linux console, meeting the project presentation requirements.

## Conclusion

The code for the *MatCom Invasion* project comprehensively addresses the requirements outlined in the project orientation. The implementation of event-driven programming, concurrency with threads, scheduling strategies, and the page replacement algorithm is clearly reflected in the design and functioning of the game. The use of a matrix to represent the game field and the associated logic ensures a smooth and challenging gameplay experience, demonstrating a solid understanding of the required fundamental concepts.