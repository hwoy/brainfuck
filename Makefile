BF = bf.exe

CXX = g++

CXXFLAGS = -g

.PHONY: all clean

all: $(BF)

$(BF): bf.cpp bf.hpp
		$(CXX) -o $(BF) $(CXXFLAGS) bf.cpp
		
clean:
	del *.exe *.o