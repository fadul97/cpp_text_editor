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

    /* Local mode 
     * ECHO: turn off key printing when typed
     * ICANON: turn off canonical mode
     * ISIG: turn off transmission from Ctrl-C and Ctrl-Z
     * IEXTEN: turn off transmission from Ctrl-V
     */
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    /* Input mode
     * IXON: turn off software flow control(Ctrl-S and Ctrl-Q transmission)
     * ICRNL: turn off carriage returns
     * BRKINT: turn off break condition and avoid SIGINT signal
     * INPCK: disable parity checking
     * ISTRIP: disable 8th bit of each input byte to be stripped(set to 0)
     */
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);

    /* Output mode
     * OPOST: turn off output processing features
     */
    raw.c_oflag &= ~(OPOST);

    /* Control mode
     * CS8: set the character size to 8 bits per byte
    */
    raw.c_cflag |= (CS8);

    /* Control characters
     * VMIN: set min number of bytes of input needed before read() can return
     * VTIME: set max amount of time to wait before read() returns
     */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    /* Set terminal attributes
    TCSAFLUSH waits for all pending output to be written to the terminal
     and discards input that wasn't read */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(int argc, char** argv)
{
    enable_raw_mode();
    
    while(true)
    {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        
        // If c is a control character(nonprintable characters)
        if(iscntrl(c))
            std::cout << (int)c << "\r\n";
        else
            std::cout << (int)c << " ('" << c << "')\r\n";

        if(c == 'q')
            break;
    }
    return 0;
}