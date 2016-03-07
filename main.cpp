#include <iostream>
#include <cstdio>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>

#if defined(__linux__)
        #include <termios.h>
        #include <fcntl.h>
        #include <linux/input.h>
        #include <sys/timeb.h>
        #include <sys/types.h>
        #include <sys/stat.h>
#elif defined(_WIN32)
        #include <windows.h>
#endif

#include "Board.hpp"
#include "KeyInput.hpp"

#if defined(_WIN32)
        #define KB_DEVICE
#endif

#if defined(__linux__)
static struct termios t_old, t_new;
#endif

static const char splash[] =
"                                                                         \n\
   _|_|_|            _|      _|            _|_|_|_|_|          _|_|_|    \n\
 _|          _|_|    _|_|    _|    _|_|_|      _|      _|_|_|  _|    _|  \n\
 _|        _|    _|  _|  _|  _|  _|_|          _|    _|    _|  _|_|_|    \n\
 _|        _|    _|  _|    _|_|      _|_|      _|    _|    _|  _|    _|  \n\
   _|_|_|    _|_|    _|      _|  _|_|_|        _|      _|_|_|  _|    _|  \n\
                                                                         \n\
                                                            MonitorStudio";

void show_splash(void);
void echo_off(void);
void echo_on(void);

int main()
{
        struct timeb start, end;

        show_splash();

        Board board = Board(20, 10);
        Input input = Input(KB_DEVICE);

        echo_off();
        ftime(&start);

        std::size_t i, size;
        std::deque<int> pressed_keys;
        std::deque<int> continus_keys;

        for(; ; )
        {
                input._update();
                ftime(&end);

                for(pressed_keys = input.get_pressed_keys(), i = 0, size = pressed_keys.size(); i < size; i++)
                {
                        switch(pressed_keys[i])
                        {
                        case KEY_UP:
                                board.rotate_block(CLOCKWISE);
                                break;
                        case KEY_LEFTSHIFT:
                        case KEY_RIGHTSHIFT:
                                board.hold_block();
                                break;
                        case KEY_LEFTCTRL:
                        case KEY_RIGHTCTRL:
                                board.rotate_block(COUNTERCLOCKWISE);
                                break;
                        case KEY_SPACE:
                                board.drop_block();
                                break;
                        case KEY_Q:
                                echo_on();
                                return 0;
                                break;
                        case KEY_DOWN:
                                board.move_block_down();
                                break;
                        case KEY_LEFT:
                                board.move_block_left();
                                break;
                        case KEY_RIGHT:
                                board.move_block_right();
                                break;
                        default:
                                break;
                        }
                }

                if(end.millitm - start.millitm >= 40)
                {
                        for(continus_keys = input.get_continus_keys(), i = 0, size = continus_keys.size(); i < size; i++)
                        {
                                switch(continus_keys[i])
                                {
                                case KEY_DOWN:
                                        board.move_block_down();
                                        break;
                                case KEY_LEFT:
                                        board.move_block_left();
                                        break;
                                case KEY_RIGHT:
                                        board.move_block_right();
                                        break;
                                default:
                                        break;
                                }
                        }

                        ftime(&start);
                }

                ftime(&end);
                if(end.time - start.time >= 1)
                {
                        board.move_block_down();
                        ftime(&start);
                }

                usleep(15000);
        }
}

void show_splash(void)
{
        ::clear();
        ::goto_yx(1, 1);
        for(std::size_t i = 0; i < sizeof(splash); i++)
        {
                switch(splash[i])
                {
                case ' ':
                case '\n':
                        ::set_console_color(YELLOW);
                        std::cout << splash[i];
                        break;
                default:
                        ::set_console_color(BLUE);
                        std::cout << splash[i];
                        break;
                }
        }
        std::fflush(stdout);
        sleep(3);
}

void echo_off(void)
{
#if defined(__linux__)
        tcgetattr(STDIN_FILENO, &t_old);
	memcpy(&t_new, &t_old, sizeof(struct termios));
	t_new.c_lflag &= static_cast<unsigned>(~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL));
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
#elif defined(_WIN32)
        DWORD mode = 0;
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
#endif
}

void echo_on(void)
{
#if defined(__linux__)
        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
#elif defined(_WIN32)
        DWORD mode = 0;
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & ENABLE_ECHO_INPUT);
#endif
}

