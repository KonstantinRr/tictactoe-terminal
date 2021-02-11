
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>

#define PLATFORM_WINDOWS defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM_UNIX __unix__
#define PLATFORM_LINUX __linux__

#if PLATFORM_WINDOWS

#include <conio.h> // defines _getch

int getChar()
{
    return _getch();
}

void clearScreen(void)
{
    system("cls");
}

#elif PLATFORM_UNIX || PLATFORM_LINUX

#include <termios.h>
#include <stdio.h>

static struct termios old, current;

static void initTermios(int echo) 
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    if (echo)
    {
        current.c_lflag |= ECHO;
    }
    else
    {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
}

static void resetTermios(void) 
{
    tcsetattr(0, TCSANOW, &old);
}

static char getch_(int echo) 
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

int getChar(void) 
{
    return (int)getch_(0);
}

void clearScreen(void)
{
    system("clear");
}

#endif