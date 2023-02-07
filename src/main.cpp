#include <termios.h>
#include <unistd.h>

void enable_raw_mode()
{
    // Variable that stores terminal attributes
    struct termios raw;

    // Read terminal attributes and put in raw
    tcgetattr(STDIN_FILENO, &raw);

    // Local flags: 
    raw.c_lflag &= ~(ECHO);

    /* Set terminal attributes
    TCSAFLUSH waits for all pending output to be written to the terminal
    and discards input that wasn't read */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char** argv)
{
    enable_raw_mode();
    
    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}