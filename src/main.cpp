#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

// Variable that stores terminal attributes
struct termios orig_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
    // Read terminal attributes and put in orig_termios
    tcgetattr(STDIN_FILENO, &orig_termios);

    /* Register disable_raw_mode function to be automatically executed when 
       program exits */
    atexit(disable_raw_mode);

    // Copy struct before making changes
    struct termios raw = orig_termios;

    /* Local flags: 
     * ECHO: turn off key printing when typed
     * ICANON: turn off canonical mode
    */
    raw.c_lflag &= ~(ECHO | ICANON);

    /* Set terminal attributes
    TCSAFLUSH waits for all pending output to be written to the terminal
     and discards input that wasn't read */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(int argc, char** argv)
{
    enable_raw_mode();
    
    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    {
        // If c is a control character(nonprintable characters)
        if(iscntrl(c))
            std::cout << (int)c << "\n";
        else
            std::cout << (int)c << " ('" << c << "')\n";
    }
    return 0;
}