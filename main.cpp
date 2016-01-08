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
#include "User.hpp"

int main(void)
{
	Board board = Board(20, 10);

	struct termios t_old, t_new;

	tcgetattr(STDIN_FILENO, &t_old);
	memcpy(&t_new, &t_old, sizeof(struct termios));
	t_new.c_lflag &= static_cast<unsigned>(~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL));
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	FILE *kb_device = fopen("/dev/input/by-id/usb-0566_USB_Keyboard-event-if00", "r");

	bool left = false, right = false, buttom = false;

	struct timeb start, end;
	ftime(&start);
	ftime(&end);

	for(; ; )
	{
		switch(User::getInput(kb_device))
		{
		case KEY_SPACE:
			board.dropBlock();
			break;
		case KEY_LEFTSHIFT:
		case KEY_RIGHTSHIFT:
			board.holdBlock();
			break;
		case KEY_UP:
			board.rotateBlock(CLOCKWISE);
			break;
		case KEY_LEFTCTRL:
		case KEY_RIGHTCTRL:
			board.rotateBlock(COUNTERCLOCKWISE);
			break;
		case KEY_DOWN_P:
			buttom = true;
			break;
		case KEY_DOWN_R:
			buttom = false;
			break;
		case KEY_LEFT_P:
			left = true;
			break;
		case KEY_LEFT_R:
			left = false;
			break;
		case KEY_RIGHT_P:
			right = true;
			break;
		case KEY_RIGHT_R:
			right = false;
			break;
		case KEY_Q:
			std::fflush(stdin);
			tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
			return 0;
		default:
			break;
		}

		if(buttom == true)
		{
			board.moveBlockDown();
		}
		if(left == true)
		{
			board.moveBlockLeft();
		}
		if(right == true)
		{
			board.moveBlockRight();
		}

		if(buttom && !left && !right)
			usleep(30000);
		else if(left || right)
			usleep(60000);
		else
			usleep(1);

		if(!buttom)
		{
			ftime(&end);

			if(end.time - start.time >= 1)
			{
				board.moveBlockDown();
				ftime(&start);
				ftime(&end);
			}
		}
	}
}
