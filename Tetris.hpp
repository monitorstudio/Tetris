#ifndef TETRIS_HPP_
#define TETRIS_HPP_

#include <iostream>
#include <deque>
#include <cstdio>
#include <cstddef>

#if defined(_WIN32)
        #include <windows.h>
#endif

#if defined(__linux__)
        typedef const char * color_t;
        #define	WHITE   "\033[1;37;47m"
        #define SILVER  "\033[0;37;47m"
        #define	CYAN    "\033[0;36;46m"
        #define	YELLOW  "\033[1;33;43m"
        #define	GREEN   "\033[0;32;42m"
        #define	RED     "\033[0;31;41m"
        #define	ORANGE  "\033[0;33;43m"
        #define	BLUE    "\033[0;34;44m"
        #define	PURPLE  "\033[0;35;45m"
        #define	GRAY    "\033[1;30;40m"
        #define DEFAULT "\033[0m"
#elif defined(_WIN32)
        typedef int color_t;
        #define	WHITE   255
        #define SILVER  392
        #define	CYAN    187
        #define	YELLOW  238
        #define	GREEN   426
        #define	RED     460
        #define	ORANGE  358
        #define	BLUE    409
        #define	PURPLE  477
        #define	GRAY    392
        #define DEFAULT 256
#endif

inline void hide_cursor(void)
{
#if defined(__linux__)
        std::cout << "\033[?25l";
        std::fflush(stdout);
#elif defined(_WIN32)
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
#endif
}

inline void show_cursor(void)
{
#if defined(__linux__)
        std::cout << "\033[?25h";
        std::fflush(stdout);
#elif defined(_WIN32)
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = TRUE;
        SetConsoleCursorInfo(consoleHandle, &info);
#endif
}

inline void goto_yx(size_t y, size_t x)
{
#if defined(__linux__)
        std::cout << "\033[" << y << ";" << x << "H";
        std::fflush(stdout);
#elif defined(_WIN32)
        COORD scrn;
        HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        scrn.Y = y;
        scrn.X = x;
        SetConsoleCursorPosition(hOutput, scrn);
#endif
}

inline void set_console_color(color_t color)
{
#if defined(__linux__)
        std::cout << color;
#elif defined(_WIN32)
        HANDLE  hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
#endif
}

inline void clear(void)
{
#if defined(__linux__)
        std::cout << "\033[2J";
        std::fflush(stdout);
#elif defined(_WIN32)
        COORD coordScreen = {0, 0};
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
                return;

        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        if(!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
                return;

        if(!GetConsoleScreenBufferInfo(hConsole, &csbi))
                return;

        if(!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
                return;

        SetConsoleCursorPosition(hConsole, coordScreen);
#endif
}

// FIXME clear_line NOT work
// inline void clear_line(void)
// {
// #if defined(__linux__)
//         std::cout << DEFAULT << "\033[K";
//         std::fflush(stdout);
// #elif defined(_WIN32)
// #endif
// }

// FIXME clear_line NOT work
// inline void clear_line(std::size_t y)
// {
// #if defined(__linux__)
//         goto_yx(y, 0);
//         clear_line();
// #elif defined(_WIN32)
// #endif
// }

inline void print_cell(color_t color)
{
        set_console_color(color);
#if defined(__linux__)
        std::cout << "██";
#elif defined(_WIN32)
        std::cout << "  ";
#endif
        std::fflush(stdout);
}

inline void print_cell(color_t color, std::size_t length)
{
        set_console_color(color);
        for(std::size_t i = 0; i < length; i++)
        {
#if defined(__linux__)
                std::cout << "█";
#elif defined(_WIN32)
                std::cout << " ";
#endif
        }
        std::fflush(stdout);
}

#endif  // TETRIS_HPP_

