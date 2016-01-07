#include "BlockGenerator.hpp"

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
