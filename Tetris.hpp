#ifndef TETRIS_HPP_
#define TETRIS_HPP_

#include <deque>

#include "Block.hpp"

#define	WHITE	255
#define	CYAN	187
#define	YELLOW	238
#define	GREEN	426
#define	RED		460
#define	ORANGE	358
#define	BLUE	409
#define	PURPLE	477
#define	GRAY	392
#define BLACK	256

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
	BlockGenerator(int, int);
	block_t getBlock(void);
	void reset(void);
private:
	int _y, _x;
	int _pos;
	block_t _nextBlock(void);
	std::deque<block_t> _buffer;
};

#endif	// TETRIS_HPP_

