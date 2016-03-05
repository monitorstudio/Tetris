#ifndef TETRIS_HPP_
#define TETRIS_HPP_

#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <deque>
#include <cstdio>
#include <cstddef>

#define	WHITE   255
#define BGWHITE 392
#define	CYAN    187
#define	YELLOW  238
#define	GREEN   426
#define	RED     460
#define	ORANGE  358
#define	BLUE    409
#define	PURPLE  477
#define	GRAY    392
#define DEFAULT 256

inline void hide_cursor(void)
{
        // TODO Impl this function
}

inline void show_cursor(void)
{
        // TODO Impl this function
}

inline void goto_yx(std::size_t y, std::size_t x)
{
        COORD scrn;
        HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        scrn.Y = y;
        scrn.X = x;
        SetConsoleCursorPosition(hOutput, scrn);
}

inline void clear(void)
{
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; 
    DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
    {
       return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    if(!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
    {
       return;
    }

    if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
       return;
    }

    if( !FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
    {
       return;
    }

    SetConsoleCursorPosition(hConsole, coordScreen);
}

inline void set_console_color(int color)
{
        HANDLE  hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
}

// FIXME clear_line NOT work
inline void clear_line(void)
{
        std::cout << DEFAULT << "\033[K";
        std::fflush(stdout);
}

// FIXME clear_line NOT work
inline void clear_line(std::size_t y)
{
        goto_yx(y, 0);
        clear_line();
}

inline void print_cell(int color)
{
        set_console_color(color);
        std::cout << "  ";
        std::fflush(stdout);
}

inline void print_cell(int color, std::size_t length)
{
        set_console_color(color);
        for(std::size_t i = 0; i < length; i++)
        {
                std::cout << " ";
        }
        std::fflush(stdout);
}

#endif  // TETRIS_HPP_

