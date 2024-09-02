#include "sounds.h"
#include "stdlib.h"

void play_sound(int option){
    switch (option){
    case PLAYER_SHOT:
        system("ffplay -nodisp -autoexit -volume 5 /usr/local/bin/Sounds/shoot.wav > /dev/null 2>&1 &");
        break;
    case ENEMY_MOVEMENT:
        system("ffplay -nodisp -autoexit -volume 15 /usr/local/bin/Sounds/fastinvader4.wav > /dev/null 2>&1 &");
        break;
    case UFO_MOVEMENT:
        system("ffplay -nodisp -autoexit -volume 5 /usr/local/bin/Sounds/ufo_highpitch.wav > /dev/null 2>&1 &");
        break;
    case ENEMY_KILLED:
        system("ffplay -nodisp -autoexit -volume 5 /usr/local/bin/Sounds/invaderkilled.wav > /dev/null 2>&1 &");
        break;
    case PLAYER_KILLED:
        system("ffplay -nodisp -autoexit -volume 5 /usr/local/bin/Sounds/playerkilled.wav > /dev/null 2>&1 &");
        break;
    case FAIL:
        system("ffplay -nodisp -autoexit -volume 35 /usr/local/bin/Sounds/que-es-eso-bob-esponja.mp3 > /dev/null 2>&1 &");
        break;
    }
}