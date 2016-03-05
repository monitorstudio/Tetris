#include <windows.h>
#include <iostream>
#include <cstdio>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>

#include "Board.hpp"
#include "KeyInput.hpp"

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
        Input input = Input();

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
                        case VK_UP:
                                board.rotate_block(CLOCKWISE);
                                break;
                        case VK_LSHIFT:
                        case VK_RSHIFT:
                                board.hold_block();
                                break;
                        case VK_LCONTROL:
                        case VK_RCONTROL:
                                board.rotate_block(COUNTERCLOCKWISE);
                                break;
                        case VK_SPACE:
                                board.drop_block();
                                break;
                        case VK_DOWN:
                                board.move_block_down();
                                break;
                        case VK_LEFT:
                                board.move_block_left();
                                break;
                        case VK_RIGHT:
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
                                case VK_DOWN:
                                        board.move_block_down();
                                        break;
                                case VK_LEFT:
                                        board.move_block_left();
                                        break;
                                case VK_RIGHT:
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
				        set_console_color(YELLOW);
                        std::cout << splash[i];
                        break;
                default:
				        set_console_color(BLUE);
                        std::cout << splash[i];
                        break;
                }
        }
        std::fflush(stdout);
        sleep(3);
		::clear();
}

void echo_off(void)
{
    // TODO Impl this function
}

void echo_on(void)
{
    // TODO Impl this function
}
