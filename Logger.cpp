#include "Logger.hpp"

unsigned LoggerXPos = 30;
unsigned LoggerYPos = 1;
unsigned LoggerYMax = 40;

void Logger::setXPos(unsigned pos)
{
	::LoggerXPos = pos;
}

void Logger::clear(void)
{
	for (unsigned i = 1; i < ::LoggerYMax; i++)
	{
		std::cout << "\033[" << i << ";" << ::LoggerXPos * 2 << "H                                            ";
	}

	std::fflush(stdout);

	::LoggerYPos = 1;
}
