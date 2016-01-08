#include <cstdio>
#include <unistd.h>
#include <linux/input.h>
#include <sys/select.h>
#include "User.hpp"

KeyName User::getInput(FILE *kb_device)
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
		KeyName k;
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
			read(fd[0], &k, sizeof(KeyName));

			return k;
		}
	}
	else if(flag == 0)
	{
		flag = 1;
		close(fd[0]);

		bool upPressed = false, downPressed = false, leftPressed = false, rightPressed = false, ctrlPressed = false, dropPressed = false, isHold = false;
		KeyName key;

		struct input_event ev;
		while(1)
		{
			fread(&ev, sizeof(struct input_event), 1, kb_device);

			if(ev.type == 1 && ev.value == 2)
			{
				key = (KeyName)ev.code;
			}
			else if(ev.type == 4 && ev.code == 4)
			{
				key = (KeyName)ev.value;
			}
			else if(ev.type == 1 && ev.value == 0)
			{
				key = (KeyName)ev.code;
			}
			else
			{
				continue;
			}

			switch(key)
			{
			case K_DOWN_P:
				if(downPressed == true)
				{
					downPressed = false;
					key = K_DOWN_R;
					write(fd[1], &key, sizeof(KeyName));
				}
				else
				{
					downPressed = true;
					write(fd[1], &key, sizeof(KeyName));
				}
				break;
			case K_LEFT_P:
				if(leftPressed == true)
				{
					leftPressed = false;
					key = K_LEFT_R;
					write(fd[1], &key, sizeof(KeyName));
				}
				else
				{
					leftPressed = true;
					write(fd[1], &key, sizeof(KeyName));
				}
				break;
			case K_RIGHT_P:
				if(rightPressed == true)
				{
					rightPressed = false;
					key = K_RIGHT_R;
					write(fd[1], &key, sizeof(KeyName));
				}
				else
				{
					rightPressed = true;
					write(fd[1], &key, sizeof(KeyName));
				}
				break;
			case K_UP_P:
				if((upPressed = !upPressed) == true)
					write(fd[1], &key, sizeof(KeyName));
				break;
			case K_SPACE_P:
				if((dropPressed = !dropPressed) == true)
					write(fd[1], &key, sizeof(KeyName));
				break;
			case K_Q_P:
				write(fd[1], &key, sizeof(KeyName));
				break;
			case K_P_P:
				write(fd[1], &key, sizeof(KeyName));
				break;
			case K_LSHIFT_P:
			case K_RSHIFT_P:
				if((isHold = !isHold) == true)
					write(fd[1], &key, sizeof(KeyName));
				break;
			case K_LCTRL_P:
			case K_RCTRL_P:
				if((ctrlPressed = !ctrlPressed) == true)
					write(fd[1], &key, sizeof(KeyName));
				break;
			default:
				break;
			}
		}

		close(fd[1]);
	}

	return (KeyName)0;
}
