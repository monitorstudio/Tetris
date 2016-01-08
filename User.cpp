#include <cstdio>
#include <unistd.h>
#include <linux/input.h>
#include <sys/select.h>
#include "User.hpp"

int User::getInput(FILE *kb_device)
{
	static int flag = 0;
	static pid_t pid;
	static int fd[2];

	if(flag == 0)
	{
		pipe(fd);
		pid = fork();
	}

	if(pid > 0)
	{
		flag = 1;
		close(fd[1]);
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		int key;
		fd_set read_set;
		FD_ZERO(&read_set);
		FD_SET(fd[0], &read_set);

		if(select(fd[0] + 1, &read_set, NULL, NULL, &tv) == 0)
		{
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			FD_ZERO(&read_set);
			FD_SET(fd[0], &read_set);
		}
		else
		{
			read(fd[0], &key, sizeof(int));

			return key;
		}
	}
	else if(flag == 0)
	{
		flag = 1;
		close(fd[0]);
		int key;
		struct input_event ev;

		while(1)
		{
			fread(&ev, sizeof(struct input_event), 1, kb_device);

			if(ev.type == 1)
				key = ev.code;
			else
				continue;

			switch(key)
			{
			case KEY_DOWN:
				if(ev.value == 2 && (key = KEY_DOWN_C))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 1 && (key = KEY_DOWN_P))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 0 && (key = KEY_DOWN_R))
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_LEFT:
				if(ev.value == 2 && (key = KEY_LEFT_C))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 1 && (key = KEY_LEFT_P))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 0 && (key = KEY_LEFT_R))
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_RIGHT:
				if(ev.value == 2 && (key = KEY_RIGHT_C))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 1 && (key = KEY_RIGHT_P))
					write(fd[1], &key, sizeof(int));
				else if(ev.value == 0 && (key = KEY_RIGHT_R))
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_UP:
				if(ev.value == 1)
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_SPACE:
				if(ev.value == 1)
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_Q:
				write(fd[1], &key, sizeof(int));
				break;
			case KEY_P:
				if(ev.value == 1)
				write(fd[1], &key, sizeof(int));
				break;
			case KEY_LEFTSHIFT:
			case KEY_RIGHTSHIFT:
				if(ev.value == 1)
					write(fd[1], &key, sizeof(int));
				break;
			case KEY_LEFTCTRL:
			case KEY_RIGHTCTRL:
				if(ev.value == 1)
					write(fd[1], &key, sizeof(int));
				break;
			default:
				break;
			}
		}

		close(fd[1]);
	}

	return 0;
}
