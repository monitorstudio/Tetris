CXX=g++
CXXFLAGS=-std=c++14 -Ofast -static
STRIP=strip

all:
	make release -j8

release: debug
	$(STRIP) Tetris.exe

debug: main Board Tetris KeyInput Block
	$(CXX) $(CXXFLAGS) -o Tetris.exe main.o Board.o Tetris.o KeyInput.o Block.o

main: Board.hpp Array.hpp Block.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Board: Board.hpp Tetris.hpp Block.hpp Array.hpp Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

Tetris: Tetris.hpp Tetris.cpp
	$(CXX) $(CXXFLAGS) -c Tetris.cpp

KeyInput: KeyInput.hpp KeyInput.cpp
	$(CXX) $(CXXFLAGS) -c KeyInput.cpp

Block: Block.hpp Array.hpp Block.cpp
	$(CXX) $(CXXFLAGS) -c Block.cpp

.PHONY clean:
	rm Tetris.dbg Tetris *.o
