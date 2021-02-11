#include "platform.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64)

#include <windows.h>
#include <conio.h> // defines _getch

void initTerminal()
{
    SetConsoleOutputCP(65001U);
}

int getChar(void)
{
    return _getch();
}

void clearScreen(void)
{
    system("cls");
}

#elif __unix__ || __linux__

#include <termios.h>
#include <stdio.h>

static struct termios old, current;

void initTerminal()
{

}

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

void pause(void)
{
    printf("Press any key to continue!\n");
    getChar();
}