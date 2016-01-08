#include <iostream>

#include <cstddef>
#include <ctime>

#include <deque>

#include "unistd.h"
#include "Board.hpp"
#include "Logger.hpp"

namespace {
	NodeState swState(NodeState state)
	{
		switch(state)
		{
		case STATE_OFF:		return SWITCH_OFF;
		case STATE_I:		return SWITCH_I;
		case STATE_O:		return SWITCH_O;
		case STATE_S:		return SWITCH_S;
		case STATE_Z:		return SWITCH_Z;
		case STATE_L:		return SWITCH_L;
		case STATE_J:		return SWITCH_J;
		case STATE_T:		return SWITCH_T;
		case STATE_SHADOW:	return SWITCH_SHADOW;
		default:		return state;
		}
	}

	NodeState stState(NodeState state)
	{
		switch(state)
		{
		case SWITCH_OFF:	return STATE_OFF;
		case SWITCH_I:		return STATE_I;
		case SWITCH_O:		return STATE_O;
		case SWITCH_S:		return STATE_S;
		case SWITCH_Z:		return STATE_Z;
		case SWITCH_L:		return STATE_L;
		case SWITCH_J:		return STATE_J;
		case SWITCH_T:		return STATE_T;
		case SWITCH_SHADOW:	return STATE_SHADOW;
		default:		return state;
		}
	}

	NodeState blockState(block_t type)
	{
		switch(type)
		{
		case BLK_I:	return SWITCH_I;
		case BLK_O:	return SWITCH_O;
		case BLK_S:	return SWITCH_S;
		case BLK_Z:	return SWITCH_Z;
		case BLK_L:	return SWITCH_L;
		case BLK_J:	return SWITCH_J;
		case BLK_T:	return SWITCH_T;
		case BLK_E:	return SWITCH_OFF;
		default:	return STATE_OFF;
		}
	}

	block_t stateBlock(NodeState state)
	{
		switch(state)
		{
		case STATE_OFF:		case SWITCH_OFF:
		case STATE_SHADOW:	case SWITCH_SHADOW:	return BLK_E;
		case STATE_I:		case SWITCH_I:		return BLK_I;
		case STATE_O:		case SWITCH_O:		return BLK_O;
		case STATE_S:		case SWITCH_S:		return BLK_S;
		case STATE_Z:		case SWITCH_Z:		return BLK_Z;
		case STATE_L:		case SWITCH_L:		return BLK_L;
		case STATE_J:		case SWITCH_J:		return BLK_J;
		case STATE_T:		case SWITCH_T:		return BLK_T;
		default:					return BLK_E;
		}
	}

	const char *stateColor(NodeState state)
	{
		switch(state)
		{
		case SWITCH_OFF:	case STATE_OFF:		return WHITE;
		case SWITCH_I:		case STATE_I:		return CYAN;
		case SWITCH_O:		case STATE_O:		return YELLOW;
		case SWITCH_S:		case STATE_S:		return GREEN;
		case SWITCH_Z:		case STATE_Z:		return RED;
		case SWITCH_L:		case STATE_L:		return ORANGE;
		case SWITCH_J:		case STATE_J:		return BLUE;
		case SWITCH_T:		case STATE_T:		return PURPLE;
		case SWITCH_SHADOW:	case STATE_SHADOW:	return GRAY;
		default:					return WHITE;
		}
	}

	size_t heightOf(Block &block, Array<Array<NodeState> > &table)
	{
		size_t height = table.size(), i;

		block.buttom().forEach([&] (Array<int> &pair) mutable -> void
		{
			if(pair[0] + block.y() == table.size() - 1)
				height = 0;

			for(i = pair[0] + block.y() + 1; i < table.size(); i++)
			{
				NodeState state = table[i][block.x() + pair[1]];

				if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
				{
					if(i - block.y() - pair[0] - 1 < height)
						height = i - block.y() - pair[0] - 1;

					break;
				}

				if(i == table.size() - 1 && i - block.y() - pair[0] < height)
					height = i - block.y() - pair[0];
			}
		});

		return height;
	}

	size_t clearLines(Board &board, Array<Array<NodeState> > &table)
	{
		INFO("::clearLines");

		Array<int> toClear;

		table.forEach([&] (Array<NodeState> &array, size_t column) mutable -> void
		{
			NodeState state;

			for(size_t row = 0, size = array.size(); row < size; row++)
			{
				state = array[row];

				if(state == STATE_OFF || state == SWITCH_OFF || state == STATE_SHADOW || state == SWITCH_SHADOW)
					break;

				if(row == size - 1)
					toClear[toClear.size()] = column;
			}
		});

		toClear.forEach([&] (int &column) mutable -> void
		{
			table[column].forEach([&] (NodeState &state) mutable -> void
			{
				state = SWITCH_OFF;
			});
		});

		if(toClear.size() != 0)
		{
			board.show();
			usleep(100000);

			toClear.forEach([&] (int &column) mutable -> void
			{
				for(int start = column; start >= 1; start--)
				{
					for(size_t i = 0; i <= table[0].size() - 1; i++)
					{
						NodeState state = table[start - 1][i];

						if(state != table[start][i])
						{
							table[start][i] = ::swState(state);
						}
					}
				}

				table[0].forEach([&] (NodeState &state) mutable -> void
				{
					if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
					{
						state = SWITCH_OFF;
					}
				});
			});
		}

		return toClear.size();
	}

}

Board::Board(size_t column, size_t row)
{
	INFO("Board::Board");

	std::cout << "\e[?25l\e[1;1H\e[2J";

	for(size_t y = 0; y < column + 2; y++)
	for(size_t x = 0; x < row; x++)
		_table[y][x] = SWITCH_OFF;

	_blkHolder = BlockHolder();
	_blkGen = BlockGenerator();
	_genBlock();
}

Board::~Board(void)
{
	INFO("Board::~Board");

	std::cout << "\033[0m\e[2J\e[?25h\033[1;1H";
}

void Board::show(void)
{
	INFO("Board::show");

	_updateBuffer();

	_buffer.forEach([&] (Array<int> &array) mutable -> void
	{
		std::cout << "\033[" << array[0] - 2 << ";" << (array[1] + 6) * 2 << "H" << ::stateColor(_table[array[0]][array[1]]) << "██";
	});

	_buffer.clear();

	std::fflush(stdout);
}

void Board::clear(void)
{
	INFO("Board::clear");

	_table.forEach([&] (Array<NodeState> &array) mutable -> void
	{
		array.forEach([&] (NodeState &state) mutable -> void
		{
			if(state != STATE_OFF && state != SWITCH_OFF)
				state = SWITCH_OFF;
		});
	});

	show();
}

void Board::rotateBlock(bool dir)
{
	INFO("Board::rotateBlock");

	auto collision = [&] (void) mutable -> bool
	{
		for(size_t i = 0; i < 4; i++)
		{
			if(_block.y() + _block.blocks()[i][0] < 0 || _block.y() + _block.blocks()[i][0] >= _table.size() || _block.x() + _block.blocks()[i][1] < 0 || _block.x() + _block.blocks()[i][1] >= _table[0].size())
			{
				return true;
			}
			else
			{
				NodeState state = _table[_block.y() + _block.blocks()[i][0]][_block.x() + _block.blocks()[i][1]];

				if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
					return true;
			}
		}

		return false;
	};

	auto adjust = [&] (void) -> void
	{
		for(int y = 2; y >= -2; y--)
		{
			_block.move(y, 0);

			for(int x = -2; x <= 2; x++)
			{
				_block.move(0, x);

				if(collision() == true)
					_block.move(0, -x);
				else
					return;
			}

			_block.move(-y, 0);
		}

		if(collision() == true)
		{
			if(dir == CLOCKWISE)
			{
				_block.rotate();
				_block.rotate();
				_block.rotate();
			}
			else
			{
				_block.rotate();
			}
		}
	};

	_eraseBlock();
	_eraseShadow();

	if(dir == CLOCKWISE)
	{
		_block.rotate();
	}
	else
	{
		_block.rotate();
		_block.rotate();
		_block.rotate();
	}

	if(collision() == true)
		adjust();

	_printShadow();
	_printBlock();

	show();
}

void Board::dropBlock(void)
{
	INFO("Board::dropBlock");

	_eraseBlock();
	_eraseShadow();

	_block.move(::heightOf(_block, _table), 0);

	if(_pileOverHeight() == true)
	{
		clear();
		_blkGen.reset();
		_blkHolder.reset();
	}
	else
	{
		_printBlock();
		::clearLines(*this, _table);
	}

	show();

	_genBlock();

	_printShadow();
	_printBlock();

	show();
}

void Board::holdBlock(void)
{
	INFO("Board::holdBlock");

	_eraseBlock();
	_eraseShadow();

	block_t block = _block.type();

	_blkHolder.hold(block);

	if(block == BLK_E)
	{
		_genBlock();
	}
	else
	{
		_block = Block(block);
		_block.move(0, _table[0].size() / 2);
	}

	_printShadow();
	_printBlock();

	show();
}

void Board::moveBlockDown(void)
{
	INFO("Board::moveBlockDown");

	static int drop = 0;

	auto atButtom = [&] (void) -> bool
	{
		int x = _block.x(), y = _block.y();
		Array<Array<int> > &_buttom = _block.buttom();
		NodeState state;

		for (size_t i = 0, size = _buttom.size(); i < size; i++)
		{
			if (y + _buttom[i][0] + 1 < _table.size())
			{
				state = _table[y + _buttom[i][0] + 1][x + _buttom[i][1]];

				if (state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
					return true;
			}
			else
				return true;
		}

		return false;
	};

	if(atButtom() == true)
	{
		if(drop == 4)
		{
			dropBlock();
			drop = 0;
		}
		else
			drop++;
	}
	else
	{
		drop = 0;
		_eraseBlock();
		_eraseShadow();
		_block.move(1, 0);
		_printShadow();
		_printBlock();
		show();
	}
}

int Board::moveBlockLeft(void)
{
	INFO("Board::moveBlockDown");

	int x = _block.x(), y = _block.y();
	Array<Array<int> > &_left = _block.left();
	NodeState state;

	// Check if _block is at buttom
	for (size_t i = 0, size = _left.size(); i < size; i++)
	{
		if (x + _left[i][1] - 1 >= 0)
		{
			state = _table[y + _left[i][0]][x + _left[i][1] - 1];

			if (state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
				return -1;
		}
		else
			return -1;
	}

	_eraseBlock();
	_eraseShadow();

	_block.move(0, -1);

	_printShadow();
	_printBlock();

	show();

	return 0;
}

int Board::moveBlockRight(void)
{
	INFO("Board::moveBlockDown");

	int x = _block.x(), y = _block.y();
	Array<Array<int> > &_right = _block.right();
	NodeState state;

	// Check if _block is at buttom
	for (size_t i = 0, size = _right.size(); i < size; i++)
	{
		if (x + _right[i][1] + 1 < _table[0].size())
		{
			state = _table[y + _right[i][0]][x + _right[i][1] + 1];

			if (state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
				return -1;
		}
		else
			return -1;
	}

	_eraseBlock();
	_eraseShadow();

	_block.move(0, 1);

	_printShadow();
	_printBlock();

	show();

	return 0;
}

void Board::_genBlock(void)
{
	INFO("Board::_genBlock");

	_block = Block(_blkGen.getBlock());
	_block.move(0, _table[0].size() / 2);
}

void Board::_updateBuffer(void)
{
	INFO("Board::_updateBuffer");

	Array<int> test;

	_table.forEach([&] (Array<NodeState> &array, size_t y) mutable -> void
	{
		array.forEach([&] (NodeState &state, size_t x) mutable -> void
		{
			if(y >=2)
			{
				switch (state)
				{
				case SWITCH_OFF:
				case SWITCH_I:
				case SWITCH_O:
				case SWITCH_S:
				case SWITCH_Z:
				case SWITCH_L:
				case SWITCH_J:
				case SWITCH_T:
				case SWITCH_SHADOW:
					test[0] = y;
					test[1] = x;
					if (_buffer.find(test) == false)
						_buffer[_buffer.size()] = test;
					state = ::stState(state);
					break;

				default:
					break;
			}
			}
		});
	});
}

void Board::_printShadow(void)
{
	INFO("Board::_printShadow");

	size_t height = ::heightOf(_block, _table);
	int y = _block.y(), x = _block.x();

	_block.blocks().forEach([&](Array<int> &pair) mutable -> void
	{
		_table[y + pair[0] + height][x + pair[1]] = SWITCH_SHADOW;
	});
}

void Board::_eraseShadow(void)
{
	INFO("Board::_ersaeShadow");

	size_t height = ::heightOf(_block, _table);
	int y = _block.y(), x = _block.x();

	_block.blocks().forEach([&](Array<int> &pair) mutable -> void
	{
		_table[y + pair[0] + height][x + pair[1]] = SWITCH_OFF;
	});
}

void Board::_printBlock(void)
{
	INFO("Board::_printBlock");

	int x = _block.x(), y = _block.y();
	NodeState state = ::blockState(_block.type());

	_block.blocks().forEach([&](Array<int> &pair) mutable -> void
	{
		_table[y + pair[0]][x + pair[1]] = state;
	});
}

void Board::_eraseBlock(void)
{
	INFO("Board::_eraseBlock");

	int x = _block.x(), y = _block.y();

	_block.blocks().forEach([&](Array<int> &pair) mutable -> void
	{
		_table[y + pair[0]][x + pair[1]] = SWITCH_OFF;
	});
}

bool Board::_pileOverHeight(void)
{
	for(size_t i = 0, size = _block.top().size(); i < size; i++)
	{
		if(_block.top()[i][0] + _block.y() <= 3)
			return true;
	}

	return false;
}
