#include <cstddef>
#include <ctime>
#include <deque>
#include <unistd.h>

#include "Board.hpp"

#define LARGE true
#define SMALL false

namespace {

        color_t blkColor(block_t type)
        {
                switch(type)
                {
                case BLK_E:     return DEFAULT;
                case BLK_I:     return CYAN;
                case BLK_O:     return YELLOW;
                case BLK_S:     return GREEN;
                case BLK_Z:     return RED;
                case BLK_L:     return ORANGE;
                case BLK_J:     return BLUE;
                case BLK_T:     return PURPLE;
                default:        return WHITE;
                }
        }

        void box(bool size, int y, int x)
        {
                if(size == LARGE)
                {
                        for(int i = y; i < y + 4; i++)
                        {
                                goto_yx(i, x);
                                print_cell(BGWHITE, 8);
                        }
                }
                else if(size == SMALL)
                {
                        for(int i = y; i < y + 4; i++)
                        {
                                ::goto_yx(i, x);
                                ::print_cell(BGWHITE, 6);
                        }
                }


                ::print_cell(DEFAULT, 0);
        }

        void box(bool size, block_t type, int y, int x)
        {
                Block block = Block(type);

                box(size, y, x);

                if(size == LARGE)
                {
                        block.blocks().for_each([&] (Array<int> &coord) mutable -> void
                        {
                                switch(type)
                                {
                                case BLK_I:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] + 3 + x);
                                        ::print_cell(::blkColor(type), 3);
                                        break;
                                case BLK_O:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] * 2 + 2 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_Z:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] * 2 + 3 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_S:
                                case BLK_L:
                                case BLK_J:
                                case BLK_T:
                                        ::goto_yx(coord[0] + 2 + y, coord[1] * 2 + 3 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_E:
                                        break;
                                }
                        });
                }
                else if(size == SMALL)
                {
                        block.blocks().for_each([&] (Array<int> &coord) mutable -> void
                        {
                                switch(type)
                                {
                                case BLK_I:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] + 3 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_O:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] + 1 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_Z:
                                        ::goto_yx(coord[0] + 1 + y, coord[1] + 2 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_S:
                                case BLK_L:
                                case BLK_J:
                                case BLK_T:
                                        ::goto_yx(coord[0] + 2 + y, coord[1] + 2 + x);
                                        ::print_cell(::blkColor(type));
                                        break;
                                case BLK_E:
                                        break;
                                }
                        });
                }

                ::print_cell(DEFAULT, 0);
        }

}

BlockHolder::BlockHolder(void)
{
        reset();
}

void BlockHolder::hold(block_t &type)
{
        {        block_t swap = type;
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
}

BlockGenerator::BlockGenerator(int y, int x)
{
        srand(static_cast<unsigned>(time(NULL)));

        _y = y, _x = x;
        _pos = 7;
}

block_t BlockGenerator::getBlock(void)
{
        while(_buffer.size() <= 5)
                _buffer.push_back(_nextBlock());

        block_t block = _buffer.front();
        _buffer.pop_front();

        box(LARGE, _buffer[0], _y + 2, _x * 2 + 14);
        for(int i = 1; i < 4; i++)
                box(SMALL, _buffer[i], _y + 3 + i * 4, _x * 2 + 14);

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

namespace {
        NodeState swState(NodeState state)
        {
                switch(state)
                {
                case STATE_OFF:         return SWITCH_OFF;
                case STATE_I:           return SWITCH_I;
                case STATE_O:           return SWITCH_O;
                case STATE_S:           return SWITCH_S;
                case STATE_Z:           return SWITCH_Z;
                case STATE_L:           return SWITCH_L;
                case STATE_J:           return SWITCH_J;
                case STATE_T:           return SWITCH_T;
                case STATE_SHADOW:      return SWITCH_SHADOW;
                default:                return state;
                }
        }

        NodeState stState(NodeState state)
        {
                switch(state)
                {
                case SWITCH_OFF:        return STATE_OFF;
                case SWITCH_I:          return STATE_I;
                case SWITCH_O:          return STATE_O;
                case SWITCH_S:          return STATE_S;
                case SWITCH_Z:          return STATE_Z;
                case SWITCH_L:          return STATE_L;
                case SWITCH_J:          return STATE_J;
                case SWITCH_T:          return STATE_T;
                case SWITCH_SHADOW:     return STATE_SHADOW;
                default:                return state;
                }
        }

        NodeState blockState(block_t type)
        {
                switch(type)
                {
                case BLK_I:     return SWITCH_I;
                case BLK_O:     return SWITCH_O;
                case BLK_S:     return SWITCH_S;
                case BLK_Z:     return SWITCH_Z;
                case BLK_L:     return SWITCH_L;
                case BLK_J:     return SWITCH_J;
                case BLK_T:     return SWITCH_T;
                case BLK_E:     return SWITCH_OFF;
                default:        return STATE_OFF;
                }
        }

        color_t stateColor(NodeState state)
        {
                switch(state)
                {
                case SWITCH_OFF:        case STATE_OFF:         return WHITE;
                case SWITCH_I:          case STATE_I:           return CYAN;
                case SWITCH_O:          case STATE_O:           return YELLOW;
                case SWITCH_S:          case STATE_S:           return GREEN;
                case SWITCH_Z:          case STATE_Z:           return RED;
                case SWITCH_L:          case STATE_L:           return ORANGE;
                case SWITCH_J:          case STATE_J:           return BLUE;
                case SWITCH_T:          case STATE_T:           return PURPLE;
                case SWITCH_SHADOW:     case STATE_SHADOW:      return GRAY;
                default:                                        return WHITE;
                }
        }

        size_t heightOf(Block &block, Array<Array<NodeState> > &table)
        {
                size_t height = table.size(), i;

                block.buttom().for_each([&] (Array<int> &coord) mutable -> void
                {
                        if(static_cast<std::size_t>(coord[0] + block.y()) == table.size() - 1)
                                height = 0;

                        for(i = coord[0] + block.y() + 1; i < table.size(); i++)
                        {
                                NodeState state = table[i][block.x() + coord[1]];

                                if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
                                {
                                        if(i - block.y() - coord[0] - 1 < height)
                                                height = i - block.y() - coord[0] - 1;

                                        break;
                                }

                                if(i == table.size() - 1 && i - block.y() - coord[0] < height)
                                        height = i - block.y() - coord[0];
                        }
                });

                return height;
        }

        size_t clearLines(Board &board, Array<Array<NodeState> > &table)
        {
                Array<int> toClear;

                table.for_each([&] (Array<NodeState> &array, size_t column) mutable -> void
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

                toClear.for_each([&] (int &column) mutable -> void
                {
                        table[column].for_each([&] (NodeState &state) mutable -> void
                        {
                                state = SWITCH_OFF;
                        });
                });

                if(toClear.size() != 0)
                {
                        board.show();

                        toClear.for_each([&] (int &column) mutable -> void
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

                                table[0].for_each([&] (NodeState &state) mutable -> void
                                {
                                        if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
                                        {
                                                state = SWITCH_OFF;
                                        }
                                });
                        });

                        usleep(150000);
                }

                return toClear.size();
        }

}

Board::Board(size_t column, size_t row)
{
        ::hide_cursor();
        ::clear();
        ::goto_yx(1, 1);

        for(size_t y = 0; y < column + 2; y++)
        for(size_t x = 0; x < row; x++)
                _table[y][x] = SWITCH_OFF;

        _blk_holder = BlockHolder();
        _blk_gen = BlockGenerator(0, _table[0].size());
        _gen_block();
        show();
}

Board::~Board(void)
{
        ::print_cell(DEFAULT, 0);
        ::clear();
        ::show_cursor();
        ::goto_yx(1, 1);
}

void Board::show(void)
{
        _update_buffer();

        _buffer.for_each([&] (Array<int> &array) mutable -> void
        {
                ::goto_yx(array[0] - 2, (array[1] + 6) * 2);
                ::print_cell(::stateColor(_table[array[0]][array[1]]));
        });

        _buffer.clear();

        std::fflush(stdout);
}

void Board::clear(void)
{
        _table.for_each([&] (Array<NodeState> &array) mutable -> void
        {
                array.for_each([&] (NodeState &state) mutable -> void
                {
                        if(state != STATE_OFF && state != SWITCH_OFF)
                                state = SWITCH_OFF;
                });
        });

        show();
}

void Board::rotate_block(bool dir)
{
        auto collision = [&] (void) mutable -> bool
        {
                for(size_t i = 0; i < 4; i++)
                {
                        if(_block.y() + _block.blocks()[i][0] < 0 || static_cast<std::size_t>(_block.y() + _block.blocks()[i][0]) >= _table.size() || _block.x() + _block.blocks()[i][1] < 0 || static_cast<std::size_t>(_block.x() + _block.blocks()[i][1]) >= _table[0].size())
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

        _erase_block();
        _erase_shadow();

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

        _print_shadow();
        _print_block();

        show();
}

void Board::drop_block(void)
{
        _erase_block();
        _erase_shadow();

        _block.move(::heightOf(_block, _table), 0);

        if(_pile_over_height() == true)
        {
                clear();
                _blk_gen.reset();
                _blk_holder.reset();
        }
        else
        {
                _print_block();
                ::clearLines(*this, _table);
        }

        show();

        _gen_block();

        _print_shadow();
        _print_block();

        show();
}

void Board::hold_block(void)
{
        _erase_block();
        _erase_shadow();

        block_t block = _block.type();

        _blk_holder.hold(block);

        if(block == BLK_E)
        {
                _gen_block();
        }
        else
        {
                _block = Block(block);
                _block.move(0, _table[0].size() / 2);
        }

        _print_shadow();
        _print_block();

        show();
}

void Board::move_block_down(void)
{
        static int drop = 0;

        auto atButtom = [&] (void) -> bool
        {
                int x = _block.x(), y = _block.y();
                Array<Array<int> > &_buttom = _block.buttom();
                NodeState state;

                for (size_t i = 0, size = _buttom.size(); i < size; i++)
                {
                        if (static_cast<std::size_t>(y + _buttom[i][0] + 1) < _table.size())
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
                        drop_block();
                        drop = 0;
                }
                else
                        drop++;
        }
        else
        {
                drop = 0;
                _erase_block();
                _erase_shadow();
                _block.move(1, 0);
                _print_shadow();
                _print_block();
                show();
        }
}

int Board::move_block_left(void)
{
        int x = _block.x(), y = _block.y();
        Array<Array<int> > &_left = _block.left();
        NodeState state;

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

        _erase_block();
        _erase_shadow();

        _block.move(0, -1);

        _print_shadow();
        _print_block();

        show();

        return 0;
}

int Board::move_block_right(void)
{
        int x = _block.x(), y = _block.y();
        Array<Array<int> > &_right = _block.right();
        NodeState state;

        for(size_t i = 0, size = _right.size(); i < size; i++)
        {
                if(static_cast<std::size_t>(x + _right[i][1] + 1) < _table[0].size())
                {
                        state = _table[y + _right[i][0]][x + _right[i][1] + 1];

                        if(state != STATE_OFF && state != SWITCH_OFF && state != STATE_SHADOW && state != SWITCH_SHADOW)
                                return -1;
                }
                else
                        return -1;
        }

        _erase_block();
        _erase_shadow();

        _block.move(0, 1);

        _print_shadow();
        _print_block();

        show();

        return 0;
}

void Board::_gen_block(void)
{
        _block = Block(_blk_gen.getBlock());
        _block.move(0, _table[0].size() / 2);
}

void Board::_update_buffer(void)
{
        Array<int> test;

        _table.for_each([&] (Array<NodeState> &array, size_t y) mutable -> void
        {
                array.for_each([&] (NodeState &state, size_t x) mutable -> void
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

void Board::_print_shadow(void)
{
        size_t height = ::heightOf(_block, _table);
        int y = _block.y(), x = _block.x();

        _block.blocks().for_each([&](Array<int> &coord) mutable -> void
        {
                _table[y + coord[0] + height][x + coord[1]] = SWITCH_SHADOW;
        });
}

void Board::_erase_shadow(void)
{
        size_t height = ::heightOf(_block, _table);
        int y = _block.y(), x = _block.x();

        _block.blocks().for_each([&](Array<int> &coord) mutable -> void
        {
                _table[y + coord[0] + height][x + coord[1]] = SWITCH_OFF;
        });
}

void Board::_print_block(void)
{
        int x = _block.x(), y = _block.y();
        NodeState state = ::blockState(_block.type());

        _block.blocks().for_each([&](Array<int> &coord) mutable -> void
        {
                _table[y + coord[0]][x + coord[1]] = state;
        });
}

void Board::_erase_block(void)
{
        int x = _block.x(), y = _block.y();

        _block.blocks().for_each([&](Array<int> &coord) mutable -> void
        {
                _table[y + coord[0]][x + coord[1]] = SWITCH_OFF;
        });
}

bool Board::_pile_over_height(void)
{
        for(size_t i = 0, size = _block.top().size(); i < size; i++)
        {
                if(_block.top()[i][0] + _block.y() <= 3)
                        return true;
        }

        return false;
}

