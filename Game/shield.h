#ifndef SHIELD_H
#define SHIELD_H

typedef struct {
    int x;
    int y;
    int hp;
} Shield;

void handle_shield_collisions();

#endif // SHIELD_H
