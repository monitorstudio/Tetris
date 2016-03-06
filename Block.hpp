#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "Array.hpp"

typedef enum
{       BLK_I, BLK_O, BLK_S,
        BLK_Z, BLK_L, BLK_J,
        BLK_T, BLK_E
} block_t;

class Block
{
public:
        Block(void);
        Block(block_t);

        block_t type(void);
        char name(void);
        int x(void);
        int y(void);
        Array<Array<int> > &blocks(void);
        Array<Array<int> > &top(void);
        Array<Array<int> > &buttom(void);
        Array<Array<int> > &left(void);
        Array<Array<int> > &right(void);

        void rotate(void);	// Conter
        void move(int, int);	// Move y, x
private:
        void _find_brim(void);
        void _move_center(void);
        block_t _type;
        int _x, _y;
        int _rotation;
        Array<Array<int> > _blocks;
        Array<Array<int> > _top;
        Array<Array<int> > _buttom;
        Array<Array<int> > _left;
        Array<Array<int> > _right;
};

#endif  // BLOCK_HPP_

