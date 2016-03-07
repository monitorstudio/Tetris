CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -Ofast -g3 -static
STRIP=strip
KB_DEVICE=
/dev/input/by-id/usb-0566_USB_Keyboard-event-if00
#/dev/input/by-path/platform-80860F41:00-event-kbd

ifneq ($(OS), Windows_NT)
ifeq ($(shell uname -s), Linux)
ifndef KB_DEVICE
$(error Please specify keyboard device path)
else ifeq ($(wildcard $(KB_DEVICE)),)
$(error Specified keyboard NOT found)
endif
endif
endif

all:
	echo 'Please specify your opertaing system type -- "make linux" OR "make windows"'

windows: main_win32 Board Tetris KeyInput Block
	$(CXX) $(CXXFLAGS) -o Tetris.exe main.o Board.o Tetris.o KeyInput.o Block.o

linux: main_linux Board Tetris KeyInput Block
	$(CXX) $(CXXFLAGS) -o Tetris main.o Board.o Tetris.o KeyInput.o Block.o
	$(STRIP) Tetris

main_win32: Board.hpp Array.hpp Block.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

main_linux: Board.hpp Array.hpp Block.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -DKB_DEVICE=\"$(KB_DEVICE)\"

Board: Board.hpp Tetris.hpp Block.hpp Array.hpp Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

Tetris: Tetris.hpp Tetris.cpp
	$(CXX) $(CXXFLAGS) -c Tetris.cpp

KeyInput: KeyInput.hpp KeyInput.cpp
	$(CXX) $(CXXFLAGS) -c KeyInput.cpp

Block: Block.hpp Array.hpp Block.cpp
	$(CXX) $(CXXFLAGS) -c Block.cpp

.PHONY clean:
	rm Tetris Tetris.exe *.o

