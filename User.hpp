#ifndef USER_H
#define USER_H

typedef enum KeyName
{
	K_UP_P		=	458834,
	K_UP_R		=	103,
	K_DOWN_P	=	458833,
	K_DOWN_R	=	108,
	K_LEFT_P	=	458832,
	K_LEFT_R	=	105,
	K_RIGHT_P	=	458831,
	K_RIGHT_R	=	106,
	K_SPACE_P	=	458796,
	K_SPACE_R	=	57,
	K_LSHIFT_P	=	458977,
	K_LSHIFT_R	=	42,
	K_RSHIFT_P	=	458981,
	K_RSHIFT_R	=	54,
	K_LCTRL_P	=	458976,
	K_LCTRL_R	=	29,
	K_RCTRL_P	=	488980,
	K_RCTRL_R	=	97,
	K_P_P		=	458771,
	K_P_R		=	25,
	K_Q_P		=	458772,
	K_Q_R		=	16,
	K_S_P		=	458774,
	K_S_R		=	31
} KeyName;

class User
{
public:
	static KeyName getInput(FILE *);
private:
};

#endif /* USER_H */
