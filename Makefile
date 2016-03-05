CXX=clang++
CXXFLAGS=-std=c++14 -Wall -pedantic -Ofast -g3 -static
STRIP=strip

all:
	make release -j8

release: debug
	cp Tetris.dbg Tetris
	$(STRIP) Tetris

debug: main Board Tetris KeyInput Block Logger
	$(CXX) $(CXXFLAGS) -o Tetris.dbg main.o Board.o Tetris.o KeyInput.o Block.o Logger.o

main: Board.hpp Array.hpp Block.hpp Logger.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Board: Board.hpp Tetris.hpp Block.hpp Array.hpp Logger.hpp Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

Tetris: Tetris.hpp Tetris.cpp
	$(CXX) $(CXXFLAGS) -c Tetris.cpp

KeyInput: KeyInput.hpp KeyInput.cpp
	$(CXX) $(CXXFLAGS) -c KeyInput.cpp

Block: Block.hpp Array.hpp Logger.hpp Block.cpp
	$(CXX) $(CXXFLAGS) -c Block.cpp

Logger: Logger.hpp Logger.cpp
	$(CXX) $(CXXFLAGS) -c Logger.cpp

.PHONY clean:
	rm Tetris.dbg Tetris *.o
