#include <iostream>
#include <cstdio>
#include <functional>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Board.hpp"

#include "KeyInput.hpp"

static struct termios t_old, t_new;
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
        Input input = Input("/dev/input/by-path/platform-80860F41:00-event-kbd");

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

                usleep(10000);
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
                        std::cout << "\033[0m" << splash[i];
                        break;
                default:
                        std::cout << "\033[1;34m" << splash[i];
                        break;
                }
        }
        std::fflush(stdout);
        sleep(3);
}

void echo_off(void)
{
        tcgetattr(STDIN_FILENO, &t_old);
	memcpy(&t_new, &t_old, sizeof(struct termios));
	t_new.c_lflag &= static_cast<unsigned>(~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL));
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
}

void echo_on(void)
{
        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
}
