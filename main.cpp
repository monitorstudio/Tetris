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

void echo_off(void);
void echo_on(void);

int main()
{
        struct timeb start, end;
        echo_off();
        Board board = Board(20, 10);
        Input input = Input("/dev/input/by-path/platform-80860F41:00-event-kbd");

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
                                        board.rotateBlock(CLOCKWISE);
                                        break;
                                case KEY_LEFTSHIFT:
                                case KEY_RIGHTSHIFT:
                                        board.holdBlock();
                                        break;
                                case KEY_LEFTCTRL:
                                case KEY_RIGHTCTRL:
                                        board.rotateBlock(COUNTERCLOCKWISE);
                                        break;
                                case KEY_SPACE:
                                        board.dropBlock();
                                        break;
                                case KEY_Q:
                                        echo_on();
                                        return 0;
                                        break;
                                case KEY_DOWN:
                                        board.moveBlockDown();
                                        break;
                                case KEY_LEFT:
                                        board.moveBlockLeft();
                                        break;
                                case KEY_RIGHT:
                                        board.moveBlockRight();
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
                                        board.moveBlockDown();
                                        break;
                                case KEY_LEFT:
                                        board.moveBlockLeft();
                                        break;
                                case KEY_RIGHT:
                                        board.moveBlockRight();
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
                        board.moveBlockDown();
                        ftime(&start);
                }

                usleep(10000);
        }
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
