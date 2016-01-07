#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

#define INFO(X); // { std::cout << "\033[0m\033[" << ::LoggerYPos++ << ";" << ::LoggerXPos * 2 << "HI: \033[1;32m" << X << "\033[0m"; if(::LoggerYPos == ::LoggerYMax) Logger::clear(); }
#define WARN(X); // { std::cout << "\033[0m\033[" << ::LoggerYPos++ << ";" << ::LoggerXPos * 2 << "HW: \033[1;33m" << X << "\033[0m"; if(::LoggerYPos == ::LoggerYMax) Logger::clear(); }
#define ERROR(X); // { std::cout << "\033[0m\033[" << ::LoggerYPos++ << ";" << ::LoggerXPos * 2 << "HE: \033[1;31m" << X << "\033[0m"; if(::LoggerYPos == ::LoggerYMax) Logger::clear(); }

extern unsigned LoggerYPos;
extern unsigned LoggerXPos;
extern unsigned LoggerYMax;

class Logger
{
public:
	Logger(void);

	static void clear(void);

	static void setXPos(unsigned);
};

#endif /* LOGGER_HPP */
