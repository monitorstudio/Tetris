#include <iostream>

#include "Tetris.hpp"

#define LARGE true
#define SMALL false

namespace {

	const char *blkColor(block_t type)
	{
		switch(type)
		{
		case BLK_E:	return WHITE;
		case BLK_I:	return CYAN;
		case BLK_O:	return YELLOW;
		case BLK_S:	return GREEN;
		case BLK_Z:	return RED;
		case BLK_L:	return ORANGE;
		case BLK_J:	return BLUE;
		case BLK_T:	return PURPLE;
		default:	return WHITE;
		}
	}

	void box(bool size, int y, int x)
	{
		if(size == LARGE)
		{
			for(int i = y; i < y + 4; i++)
				std::cout << "\033[" << i << ";" << x << "H\033[0;37m████████";
		}
		else if(size == SMALL)
		{
			for(int i = y; i < y + 4; i++)
				std::cout << "\033[" << i << ";" << x << "H\033[0;37m██████";
		}

		std::cout << "\033[0m";
	}

	void box(bool size, block_t type, int y, int x)
	{
		Block block = Block(type);

		box(size, y, x);

		if(size == LARGE)
		{
			block.blocks().forEach([&] (Array<int> &pair) mutable -> void
			{
				switch(type)
				{
				case BLK_I:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1]     + 3 + x << "H" << ::blkColor(type) << "███";
					break;
				case BLK_O:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1] * 2 + 2 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_Z:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1] * 2 + 3 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_S:
				case BLK_L:
				case BLK_J:
				case BLK_T:
					std::cout << "\033[" << pair[0] + 2 + y << ";" << pair[1] * 2 + 3 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_E:
					break;
				}
			});
		}
		else if(size == SMALL)
		{
			block.blocks().forEach([&] (Array<int> &pair) mutable -> void
			{
				switch(type)
				{
				case BLK_I:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1] + 3 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_O:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1] + 1 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_Z:
					std::cout << "\033[" << pair[0] + 1 + y << ";" << pair[1] + 2 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_S:
				case BLK_L:
				case BLK_J:
				case BLK_T:
					std::cout << "\033[" << pair[0] + 2 + y << ";" << pair[1] + 2 + x << "H" << ::blkColor(type) << "██";
					break;
				case BLK_E:
					break;
				}
			});
		}

		std::cout << "\033[0m";
	}

}

BlockHolder::BlockHolder(void)
{
	reset();
}

void BlockHolder::hold(block_t &type)
{
	{	block_t swap = type;
		type = _hold;
		_hold = swap;
	}

	box(LARGE, _hold, 2, 2);
}

void BlockHolder::clear(void)
{
	box(LARGE, 2, 2);
}

void BlockHolder::reset(void)
{
	_hold = BLK_E;

	clear();
}

BlockGenerator::BlockGenerator(void)
{
	srand(static_cast<unsigned>(time(NULL)));

	_pos = 7;
}

block_t BlockGenerator::getBlock(void)
{
	while(_buffer.size() <= 5)
		_buffer.push_back(_nextBlock());

	block_t block = _buffer.front();
	_buffer.pop_front();

	box(LARGE, _buffer[0], 2, 34);
	for(int i = 1; i < 4; i++)
		box(SMALL, _buffer[i], 3 + i * 4, 34);

	return block;
}

void BlockGenerator::reset(void)
{
	_buffer.clear();
	_pos = 7;
}

block_t BlockGenerator::_nextBlock(void)
{
	static block_t array[7] = { BLK_I, BLK_O, BLK_S, BLK_Z, BLK_L, BLK_J, BLK_T };

	if(_pos == 7)
	{
		for (size_t i = 0; i < 7; i++)
			{
				int rnd = rand() % 7;
				block_t t = array[i];
				array[i] = array[rnd];
				array[rnd] = t;
			}

		_pos = 0;
	}

	return array[_pos++];
}

