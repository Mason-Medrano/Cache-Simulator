all: main

main: Simulator.o cachesimulator.o Cache.o CacheLine.o MainMemory.o
	c++ -std=c++11 Simulator.o cachesimulator.o Cache.o CacheLine.o MainMemory.o -o main

Simulator.o: Simulator.cpp Simulator.h
	c++ -std=c++11 -c Simulator.cpp Simulator.h

cachesimulator.o: cachesimulator.cpp
	c++ -std=c++11 -c cachesimulator.cpp

Cache.o: Cache.cpp Cache.h
	c++ -std=c++11 -c Cache.cpp Cache.h

CacheLine.o: CacheLine.cpp CacheLine.h
	c++ -std=c++11 -c CacheLine.cpp CacheLine.h

MainMemory.o: MainMemory.cpp MainMemory.h
	c++ -std=c++11 -c MainMemory.cpp MainMemory.h

clean: 
	del -f *.o *.h.gch main
