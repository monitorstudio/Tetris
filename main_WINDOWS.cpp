#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <windows.h>

#include "Board.hpp"

int main(void)
{
	Board board(20, 10);
	
	BYTE state[256] = {0},
		 old_state[256] = {0};
	
	bool downState = false, leftState = false, rightState = false;
	int upState = 0, spaceState = 0, lshiftState = 0, rshiftState = 0, lctrlState = 0, rctrlState = 0;
	
	unsigned delay = 0;
	
	struct timeb start, end;
	ftime(&start);
	ftime(&end);

	for(; ; )
	{
		old_state[VK_DOWN] = state[VK_DOWN];
		old_state[VK_LEFT] = state[VK_LEFT];
		old_state[VK_RIGHT] = state[VK_RIGHT];
		
		GetKeyState(VK_DOWN);
		GetKeyState(VK_LEFT);
		GetKeyState(VK_RIGHT);
		GetKeyboardState(state);
		
		if(state[VK_DOWN] != old_state[VK_DOWN])
			downState = !downState;
		if(state[VK_LEFT] != old_state[VK_LEFT])
			leftState = !leftState;
		if(state[VK_RIGHT] != old_state[VK_RIGHT])
			rightState = !rightState;
		
		delay = 0;
		
		if(downState == true)
		{
			board.moveBlockDown();
			delay = (delay < 30000) ? 30000 : delay;
		}
		if(leftState == true)
		{
			board.moveBlockLeft();
			delay = (delay < 50000) ? 50000 : delay;
		}
		if(rightState == true)
		{
			board.moveBlockRight();
			delay = (delay < 50000) ? 50000 : delay;
		}
		
		int tmpKeyState;
		
		if(upState != (tmpKeyState = GetKeyState(VK_UP)))
		{
			upState = tmpKeyState;
			if(upState < 0)
				board.rotateBlock(true);
		}
		if(lctrlState != (tmpKeyState = GetKeyState(VK_LCONTROL)))
		{
			lctrlState = tmpKeyState;
			if(lctrlState < 0)
				board.rotateBlock(false);
		}
		if(rctrlState != (tmpKeyState = GetKeyState(VK_RCONTROL)))
		{
			rctrlState = tmpKeyState;
			if(rctrlState < 0)
				board.rotateBlock(false);
		}
		if(spaceState != (tmpKeyState = GetKeyState(VK_SPACE)))
		{
			spaceState = tmpKeyState;
			if(spaceState < 0)
				board.dropBlock();
		}
		if(lshiftState != (tmpKeyState = GetKeyState(VK_LSHIFT)))
		{
			lshiftState = tmpKeyState;
			if(lshiftState < 0)
				board.holdBlock();
		}
		if(rshiftState != (tmpKeyState = GetKeyState(VK_RSHIFT)))
		{
			rshiftState = tmpKeyState;
			if(rshiftState < 0)
				board.holdBlock();
		}
		
		ftime(&end);
		if(end.time - start.time >= 1)
		{
			board.moveBlockDown();
			ftime(&start);
			ftime(&end);
		}
		
		usleep(delay);
	}

	return 0;
}

