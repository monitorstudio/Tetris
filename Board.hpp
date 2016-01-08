#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <cstddef>
#include "Block.hpp"
#include "Tetris.hpp"
#include "Array.hpp"

#define CLOCKWISE true
#define COUNTERCLOCKWISE false

typedef enum
{
	STATE_OFF,
	STATE_I,
	STATE_O,
	STATE_S,
	STATE_Z,
	STATE_L,
	STATE_J,
	STATE_T,
	STATE_SHADOW,
	SWITCH_OFF,
	SWITCH_I,
	SWITCH_O,
	SWITCH_S,
	SWITCH_Z,
	SWITCH_L,
	SWITCH_J,
	SWITCH_T,
	SWITCH_SHADOW
} NodeState;

class Board
{
public:
	Board(size_t, size_t);
	~Board(void);
	void show(void);
	void clear(void);
	void autoPlay(void);
	void rotateBlock(bool);
	void dropBlock(void);
	void holdBlock(void);
	void moveBlockDown(void);
	int moveBlockLeft(void);
	int moveBlockRight(void);
private:
	void _genBlock(void);
	void _updateBuffer(void);
	void _printShadow(void);
	void _eraseShadow(void);
	void _printBlock(void);
	void _eraseBlock(void);
	bool _pileOverHeight(void);
	Block _block;
	BlockHolder _blkHolder;
	BlockGenerator _blkGen;
	Array<Array<NodeState> > _table;
	Array<Array<int> > _buffer;
};

#endif	// BOARD_HPP_
