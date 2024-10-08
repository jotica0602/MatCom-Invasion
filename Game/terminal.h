#ifndef TERMINAL_H
#define TERMINAL_H
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

void get_terminal_size(int *rows, int *cols);
void set_conio_mode(int mode);                  // Terminal Canonic mode enabler/disabler
#endif