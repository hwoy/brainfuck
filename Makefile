BF = bf.exe
TBF = tbf.exe

CXX = g++

DEBUGFLAGS = -g -Wall
RELEASEFLAGS = -O2

CXXFLAGS = $(DEBUGFLAGS)

.PHONY: all clean

all: $(BF) $(TBF)

$(BF): bf.cpp bf.hpp
		$(CXX) -o $(BF) $(CXXFLAGS) bf.cpp

$(TBF): tbf.cpp bf.hpp
		$(CXX) -o $(TBF) $(CXXFLAGS) tbf.cpp
		
clean:
	del *.exe *.o *.txt *.bf