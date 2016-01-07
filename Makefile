CXX=clang++
CXXFLAGS=-std=c++14 -Ofast -g3 -static
STRIP=strip

all:
	make release -j8

release: debug
	cp Tetris.dbg Tetris
	$(STRIP) Tetris

debug: main Board BlockGenerator Block User Logger
	$(CXX) $(CXXFLAGS) -o Tetris.dbg main.o Board.o BlockGenerator.o Block.o User.o Logger.o

main: Board.hpp Array.hpp Block.hpp Logger.hpp User.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Board: Board.hpp BlockGenerator.hpp Block.hpp Array.hpp Logger.hpp Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

BlockGenerator: BlockGenerator.hpp Block.hpp BlockGenerator.cpp
	$(CXX) $(CXXFLAGS) -c BlockGenerator.cpp

Block: Block.hpp Array.hpp Logger.hpp Block.cpp
	$(CXX) $(CXXFLAGS) -c Block.cpp

User: User.hpp User.cpp
	$(CXX) $(CXXFLAGS) -c User.cpp

Logger: Logger.hpp Logger.cpp
	$(CXX) $(CXXFLAGS) -c Logger.cpp

.PHONY clean:
	rm Tetris.dbg Tetris *.o
