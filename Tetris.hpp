#ifndef TETRIS_H
#define TETRIS_H

#include <deque>

#include "Block.hpp"

#define	WHITE	"\033[1;37;47m"
#define	CYAN	"\033[0;36;46m"
#define	YELLOW	"\033[1;33;43m"
#define	GREEN	"\033[0;32;42m"
#define	RED	"\033[0;31;41m"
#define	ORANGE	"\033[0;33;43m"
#define	BLUE	"\033[0;34;44m"
#define	PURPLE	"\033[1;35;45m"
#define	GRAY	"\033[1;30;40m"

class BlockHolder
{
public:
	BlockHolder(void);
	void hold(block_t &);
	void clear(void);
	void reset(void);
private:
	block_t _hold;
};

class BlockGenerator
{
public:
	BlockGenerator(void);
	block_t getBlock(void);
	void reset(void);
private:
	int _pos;
	block_t _nextBlock(void);
	std::deque<block_t> _buffer;
};

#endif /* TETRIS_H */
