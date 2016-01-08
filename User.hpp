#ifndef USER_H
#define USER_H

typedef enum KeyName
{
	K_UP_P		=	458834,
	K_UP_C		=	103,
	K_UP_R		=	1,
	K_DOWN_P	=	458833,
	K_DOWN_C	=	108,
	K_DOWN_R	=	2,
	K_LEFT_P	=	458832,
	K_LEFT_C	=	105,
	K_LEFT_R	=	3,
	K_RIGHT_P	=	458831,
	K_RIGHT_C	=	106,
	K_RIGHT_R	=	4,
	K_SPACE_P	=	458796,
	K_SPACE_C	=	57,
	K_SPACE_R	=	5,
	K_LSHIFT_P	=	458977,
	K_LSHIFT_C	=	42,
	K_LSHIFT_R	=	6,
	K_RSHIFT_P	=	458981,
	K_RSHIFT_C	=	54,
	K_RSHIFT_R	=	7,
	K_LCTRL_P	=	458976,
	K_LCTRL_C	=	29,
	K_LCTRL_R	=	8,
	K_RCTRL_P	=	488980,
	K_RCTRL_C	=	97,
	K_RCTRL_R	=	9,
	K_P_P		=	458771,
	K_P_C		=	25,
	K_P_R		=	10,
	K_Q_P		=	458772,
	K_Q_C		=	16,
	K_Q_R		=	11,
	K_S_P		=	458774,
	K_S_C		=	31,
	K_S_R		=	12
} KeyName;

class User
{
public:
	static KeyName getInput(FILE *);
private:
};

#endif /* USER_H */
