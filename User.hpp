#ifndef USER_H
#define USER_H

#include <linux/input.h>

typedef enum KeyName
{
	KEY_DOWN_P	= 1,
	KEY_DOWN_C	= 2,
	KEY_DOWN_R	= 3,
	KEY_LEFT_P	= 4,
	KEY_LEFT_C	= 5,
	KEY_LEFT_R	= 6,
	KEY_RIGHT_P	= 7,
	KEY_RIGHT_C	= 8,
	KEY_RIGHT_R	= 9
} KeyName;

class User
{
public:
	static int getInput(FILE *);
private:
};

#endif /* USER_H */
