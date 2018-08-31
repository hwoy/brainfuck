BF = bf.exe
TBF = tbf.exe

CXX = g++

CXXFLAGS = -g

.PHONY: all clean

all: $(BF) $(TBF)

$(BF): bf.cpp bf.hpp
		$(CXX) -o $(BF) $(CXXFLAGS) bf.cpp

$(TBF): tbf.cpp bf.hpp
		$(CXX) -o $(TBF) $(CXXFLAGS) tbf.cpp
		
clean:
	del *.exe *.o