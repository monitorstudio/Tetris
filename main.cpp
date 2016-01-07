#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <linux/input.h>
#include <sys/timeb.h>
#include "Board.hpp"
#include "User.hpp"

int main(void)
{
	Board board = Board(20, 10);

	struct termios t_old, t_new;
	struct input_event ev;

	tcgetattr(STDIN_FILENO, &t_old);
	memcpy(&t_new, &t_old, sizeof(struct termios));
	t_new.c_lflag &= static_cast<unsigned>(~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL));
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	FILE *kb_device = fopen("/dev/input/by-id/usb-0566_USB_Keyboard-event-if00", "r");

	bool left = false, right = false, buttom = false;
	char dummy[255];

	struct timeb start, end;
	ftime(&start);
	ftime(&end);

	for(; ; )
	{
		switch(User::getInput(kb_device))
		{
		case K_SPACE_P:
			board.dropBlock();
			break;
		case K_LSHIFT_P:
		case K_RSHIFT_P:
			board.holdBlock();
			break;
		case K_UP_P:
			board.rotateBlock();
			break;
		case K_LCTRL_P:
		case K_RCTRL_P:
			break;
		case K_DOWN_P:
			buttom = true;
			break;
		case K_DOWN_R:
			buttom = false;
			break;
		case K_LEFT_P:
			left = true;
			break;
		case K_LEFT_R:
			left = false;
			break;
		case K_RIGHT_P:
			right = true;
			break;
		case K_RIGHT_R:
			right = false;
			break;
		case K_Q_P:
			std::fflush(stdin);
			std::scanf("%s", dummy);
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
			usleep(75000);
		else
			usleep(4);

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
