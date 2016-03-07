#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <cstddef>
#include "Block.hpp"
#include "Tetris.hpp"
#include "Array.hpp"

#define CLOCKWISE        true
#define COUNTERCLOCKWISE false

class BlockHolder
{
public:
        BlockHolder(void);
        void hold(block_t &type);
        void clear(void);
        void reset(void);
private:
        block_t _hold;
};

class BlockGenerator
{
public:
        BlockGenerator(void);
        BlockGenerator(int y, int x);
        block_t getBlock(void);
        void reset(void);
private:
        int _y, _x;
        int _pos;
        block_t _nextBlock(void);
        std::deque<block_t> _buffer;
};

typedef enum
{       STATE_OFF, STATE_I, STATE_O,
        STATE_S, STATE_Z, STATE_L,
        STATE_J, STATE_T, STATE_SHADOW,
        SWITCH_OFF, SWITCH_I, SWITCH_O,
        SWITCH_S, SWITCH_Z, SWITCH_L,
        SWITCH_J, SWITCH_T, SWITCH_SHADOW
} NodeState;

class Board
{
public:
        Board(size_t y, size_t x);
        ~Board(void);

        void show(void);
        void clear(void);
        void auto_play(void);
        void rotate_block(bool dir);
        void drop_block(void);
        void hold_block(void);
        void move_block_down(void);
        int move_block_left(void);
        int move_block_right(void);
        void set_clear_during(unsigned during);
private:
        void _gen_block(void);
        void _update_buffer(void);
        void _print_shadow(void);
        void _erase_shadow(void);
        void _print_block(void);
        void _erase_block(void);
        bool _pile_over_height(void);

        Block _block;
        BlockHolder _blk_holder;
        BlockGenerator _blk_gen;
        Array<Array<NodeState> > _table;
        Array<Array<int> > _buffer;
};

#endif  // BOARD_HPP_

