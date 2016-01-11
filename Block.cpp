#include <cstddef>
#include "Block.hpp"
#include "Logger.hpp"

namespace {

	char blockName(block_t type)
	{
		switch(type)
		{
		case BLK_I:	return 'I';
		case BLK_O:	return 'O';
		case BLK_S:	return 'S';
		case BLK_Z:	return 'Z';
		case BLK_L:	return 'L';
		case BLK_J:	return 'J';
		case BLK_T:	return 'T';
		case BLK_E:	return 'E';
		}
	}

	unsigned blockId(block_t type)
	{
		switch(type)
		{
		case BLK_I:	return 0;
		case BLK_O:	return 1;
		case BLK_S:	return 2;
		case BLK_Z:	return 3;
		case BLK_L:	return 4;
		case BLK_J:	return 5;
		case BLK_T:	return 6;
		case BLK_E:	return 7;
		}
	}

}

Block::Block(void)
{
	// Do Nothing
}

Block::Block(block_t type)
	: _type(type), _rotation(0)
{
	switch(type)
	{
	case BLK_I:
		_y = 2, _x = 0;
		_blocks[0][0] =  0, _blocks[0][1] = -2;
		_blocks[1][0] =  0, _blocks[1][1] = -1;
		_blocks[2][0] =  0, _blocks[2][1] =  0;
		_blocks[3][0] =  0, _blocks[3][1] =  1;
		break;
	case BLK_O:
		_y = 1, _x = -1;
		_blocks[0][0] =  0, _blocks[0][1] =  0;
		_blocks[1][0] =  0, _blocks[1][1] =  1;
		_blocks[2][0] =  1, _blocks[2][1] =  0;
		_blocks[3][0] =  1, _blocks[3][1] =  1;
		break;
	case BLK_S:
		_y = 2, _x = -1;
		_blocks[0][0] = -1, _blocks[0][1] =  0;
		_blocks[1][0] = -1, _blocks[1][1] =  1;
		_blocks[2][0] =  0, _blocks[2][1] =  0;
		_blocks[3][0] =  0, _blocks[3][1] = -1;
		break;
	case BLK_Z:
		_y = 1, _x = -1;
		_blocks[0][0] =  0, _blocks[0][1] = -1;
		_blocks[1][0] =  0, _blocks[1][1] =  0;
		_blocks[2][0] =  1, _blocks[2][1] =  0;
		_blocks[3][0] =  1, _blocks[3][1] =  1;
		break;
	case BLK_L:
		_y = 2, _x = -1;
		_blocks[0][0] =  0, _blocks[0][1] = -1;
		_blocks[1][0] =  0, _blocks[1][1] =  0;
		_blocks[2][0] =  0, _blocks[2][1] =  1;
		_blocks[3][0] = -1, _blocks[3][1] =  1;
		break;
	case BLK_J:
		_y = 2, _x = -1;
		_blocks[0][0] = -1, _blocks[0][1] = -1;
		_blocks[1][0] =  0, _blocks[1][1] = -1;
		_blocks[2][0] =  0, _blocks[2][1] =  0;
		_blocks[3][0] =  0, _blocks[3][1] =  1;
		break;
	case BLK_T:
		_y = 2, _x = -1;
		_blocks[0][0] =  0, _blocks[0][1] = -1;
		_blocks[1][0] =  0, _blocks[1][1] =  0;
		_blocks[2][0] = -1, _blocks[2][1] =  0;
		_blocks[3][0] =  0, _blocks[3][1] =  1;
		break;
	case BLK_E:
		break;
	}

	_findBrim();
}

block_t Block::type(void)
{
	return _type;
}

char Block::name(void)
{
	return ::blockName(_type);
}

int Block::x(void)
{
	return _x;
}

int Block::y(void)
{
	return _y;
}

Array<Array<int> > &Block::blocks(void)
{
	return _blocks;
}

Array<Array<int> > &Block::top(void)
{
	return _top;
}

Array<Array<int> > &Block::buttom(void)
{
	return _buttom;
}

Array<Array<int> > &Block::left(void)
{
	return _left;
}

Array<Array<int> > &Block::right(void)
{
	return _right;
}

void Block::rotate(void)
{
	bool dir = true;	// Counter-Clockwise Rotate Is Not Support Yet

	if(_type != BLK_O)	// No Need To Rotate Block O
	{
		_blocks.forEach([&] (Array<int> &array) mutable -> void
		{
			int i = array[0];
			array[0] = array[1];
			array[1] = -i;
		});

		_moveCenter();
		_findBrim();
	}

	// Update Rotation Status
	switch(_rotation)
	{
	case 0:	_rotation = (dir == true) ? 1 : 3;	break;
	case 1:	_rotation = (dir == true) ? 2 : 0;	break;
	case 2:	_rotation = (dir == true) ? 3 : 1;	break;
	case 3:	_rotation = (dir == true) ? 0 : 2;	break;
	default:					break;
	}
}

void Block::move(int my, int mx)
{
	I("Block::move");

	_y += my, _x += mx;

	I("_block._y = " << _y);
	I("_block._x = " << _x);
}

void Block::_findBrim(void)
{
	I("Block::_findBrim");

	_top.clear();
	_buttom.clear();
	_left.clear();
	_right.clear();

	_blocks.forEach([&] (Array<int> &array) mutable -> void
	{
		Array<int> test = array;

		test[0] -= 1;
		if(_blocks.find(test) == false)
		{
			test = array;
			_top[_top.size()] = test;
		}

		test = array;
		test[0] += 1;
		if(_blocks.find(test) == false)
		{
			test = array;
			_buttom[_buttom.size()] = test;
		}

		test = array;
		test[1] -= 1;
		if(_blocks.find(test) == false)
		{
			test = array;
			_left[_left.size()] = test;
		}

		test = array;
		test[1] += 1;
		if(_blocks.find(test) == false)
		{
			test = array;
			_right[_right.size()] = test;
		}
	});
}
			

void Block::_moveCenter(void)	// Correct Center On Each Rotation Status
{
	int my = 0, mx = 0;

	switch(blockId(_type) + _rotation * 4)
	{
	// BLK_I
	case 0 +  0:	my = -1, mx =  0;	break;
	case 0 +  4:	my =  0, mx = -1;	break;
	case 0 +  8:	my =  1, mx =  0;	break;
	case 0 + 12:	my =  0, mx =  1;	break;
	// BLK_S
	case 2 +  0:	my =  0, mx =  1; 	break;
	case 2 +  4:	my = -1, mx =  0;	break;
	case 2 +  8:	my =  0, mx = -1;	break;
	case 2 + 12:	my =  1, mx =  0;	break;
	// BLK_Z
	case 3 +  0:	my =  0, mx = -1;	break;
	case 3 +  4:	my =  1, mx =  0;	break;
	case 3 +  8:	my =  0, mx =  1;	break;
	case 3 + 12:	my = -1, mx =  0;	break;
	default:	my =  0, mx =  0;	break;
	}

	switch (_type)
	{
	case BLK_I:
	case BLK_S:
	case BLK_Z:
		_y += my, _x += mx;
		_blocks.forEach([&](Array<int> &array) mutable -> void { array[0] -= my, array[1] -= mx; });
		break;
	default:
		break;
	}
}

