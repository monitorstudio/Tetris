#ifndef TETRIS_HPP_
#define TETRIS_HPP_

#include <iostream>
#include <deque>
#include <cstdio>
#include <cstddef>

#define	WHITE   "\033[1;37;47m"
#define BGWHITE "\033[0;37;47m"
#define	CYAN    "\033[0;36;46m"
#define	YELLOW  "\033[1;33;43m"
#define	GREEN   "\033[0;32;42m"
#define	RED     "\033[0;31;41m"
#define	ORANGE  "\033[0;33;43m"
#define	BLUE    "\033[0;34;44m"
#define	PURPLE  "\033[0;35;45m"
#define	GRAY    "\033[1;30;40m"
#define DEFAULT "\033[0m"

inline void hide_cursor(void)
{
        std::cout << "\033[?25l";
        std::fflush(stdout);
}

inline void show_cursor(void)
{
        std::cout << "\033[?25h";
        std::fflush(stdout);
}

inline void goto_yx(size_t y, size_t x)
{
        std::cout << "\033[" << y << ";" << x << "H";
        std::fflush(stdout);
}

inline void clear(void)
{
        std::cout << "\033[2J";
        std::fflush(stdout);
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

inline void print_cell(const char *color)
{
        std::cout << color << "██";
        std::fflush(stdout);
}

inline void print_cell(const char *color, std::size_t length)
{
        std::cout << color;
        for(std::size_t i = 0; i < length; i++)
        {
                std::cout << "█";
        }
        std::fflush(stdout);
}

#endif  // TETRIS_HPP_

