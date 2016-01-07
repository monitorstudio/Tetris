#ifndef BLOCKGENERATOR_HPP_
#define BLOCKGENERATOR_HPP_

#include <deque>

#include "Block.hpp"

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

#endif	// BLOCKGENERATOR_HPP_
