#ifndef TERMINAL_H
#define TERMINAL_H
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

void get_terminal_size(int *rows, int *cols);

int get_rows();

int get_cols();

// Terminal Canonic mode enabler/disabler
void set_conio_mode(int mode);

void restore_terminal();

#endif