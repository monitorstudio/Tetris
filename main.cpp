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

int isKeyPressed(int, int);

int main(void)
{
	Board board = Board(30, 10);

	struct termios t_old, t_new;

	tcgetattr(STDIN_FILENO, &t_old);
	memcpy(&t_new, &t_old, sizeof(struct termios));
	t_new.c_lflag &= static_cast<unsigned>(~( ECHO | ICANON | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL));
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	int kbd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
	unsigned delay = 341;

	bool up = false, lshift = false, rshift = false, lctrl = false, rctrl = false, space = false;

	struct timeb start, end;
	ftime(&start);
	ftime(&end);

	auto noContinusKeyAction = [&] (int key, bool &state, std::function<void(void)> action) mutable -> bool
	{
		bool pressed = isKeyPressed(kbd, key);

		if(pressed == true)
		{
			if(state == false)
			{
				action();
				state = true;
			}
		}
		else
			state = false;

		return pressed;
	};

	auto keyAction = [&] (int key, std::function<void(void)> action) mutable -> void
	{
		switch(key)
		{
		case KEY_UP:
			noContinusKeyAction(key, up, action);
			break;
		case KEY_LEFTSHIFT:
			noContinusKeyAction(key, lshift, action);
			break;
		case KEY_RIGHTSHIFT:
			noContinusKeyAction(key, rshift, action);
			break;
		case KEY_LEFTCTRL:
			noContinusKeyAction(key, lctrl, action);
			break;
		case KEY_RIGHTCTRL:
			noContinusKeyAction(key, rctrl, action);
			break;
		case KEY_SPACE:
			noContinusKeyAction(key, space, action);
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
			if(isKeyPressed(kbd, key) == true)
			{	delay = (delay < 80000) ? 80000 : delay;
				action();
			}	break;
		case KEY_DOWN:
			if(isKeyPressed(kbd, key) == true)
			{	delay = (delay < 30000) ? 30000 : delay;
				action();
			}	break;
		default:	break;
		}
	};

	for(; ; )
	{
		if(isKeyPressed(kbd, KEY_Q) == true)
		{
			fflush(stdin);
			tcsetattr(STDIN_FILENO, TCSANOW, &t_old);

			break;
		}

		delay = 341;
		keyAction(KEY_RIGHT,	[&] (void) mutable -> void { board.moveBlockRight(); });
		keyAction(KEY_DOWN, 	[&] (void) mutable -> void { board.moveBlockDown(); });
		keyAction(KEY_LEFT, 	[&] (void) mutable -> void { board.moveBlockLeft(); });
		keyAction(KEY_UP,	[&] (void) mutable -> void { board.rotateBlock(CLOCKWISE); });
		keyAction(KEY_LEFTCTRL,	[&] (void) mutable -> void { board.rotateBlock(COUNTERCLOCKWISE); });
		keyAction(KEY_RIGHTCTRL,[&] (void) mutable -> void { board.rotateBlock(COUNTERCLOCKWISE); });
		keyAction(KEY_LEFTSHIFT,[&] (void) mutable -> void { board.holdBlock(); });
		keyAction(KEY_RIGHTSHIFT,[&] (void) mutable -> void { board.holdBlock(); });
		keyAction(KEY_SPACE,	[&] (void) mutable -> void { board.dropBlock(); });

		ftime(&end);
		if(end.time - start.time >= 1)
		{
			board.moveBlockDown();
			ftime(&start);
			ftime(&end);
		}

		usleep(delay);
	}

	return 0;
}

int isKeyPressed(int fd, int key)
{
	char key_b[(KEY_MAX + 7) / 8];
	memset(key_b, 0, sizeof(key_b));
	ioctl(fd, EVIOCGKEY(sizeof(key_b)), key_b);

	return !!(key_b[key / 8] & (1 << (key % 8)));
}


