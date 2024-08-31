#include "terminal.h"

int get_cols()
{
    struct winsize w;
    // Getting terminal dimensions
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        perror("ioctl");
        // return;
    }
    int rows = w.ws_row;
    // int cols = w.ws_col;
    return rows;
}

int get_rows()
{
    struct winsize w;
    // Getting terminal dimensions
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        perror("ioctl");
        // return;
    }
    // int rows = w.ws_row;
    int cols = w.ws_col;
    return cols;
}

void set_conio_mode(int mode)
{
    static struct termios oldt, newt;
    if (mode == 0){
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else{
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}


void restore_terminal(){
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON;
    term.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &term);
}
