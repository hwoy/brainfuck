BF = bf
TBF = tbf

CXX = g++

STD =c++11

DEBUGFLAGS = -g -Wall -std=$(STD) -pedantic
RELEASEFLAGS = -O2 -Wall -std=$(STD) -pedantic

CXXFLAGS = $(DEBUGFLAGS)

.PHONY: all clean release

all: $(BF) $(TBF)

$(BF): bf.cpp bf.hpp
		$(CXX) -o $(BF) $(CXXFLAGS) bf.cpp

$(TBF): tbf.cpp bf.hpp
		$(CXX) -o $(TBF) $(CXXFLAGS) tbf.cpp


release: $(BF)-r $(TBF)-r

$(BF)-r: bf.cpp bf.hpp
		$(CXX) -o $(BF)-r $(RELEASEFLAGS) bf.cpp

$(TBF)-r: tbf.cpp bf.hpp
		$(CXX) -o $(TBF)-r $(RELEASEFLAGS) tbf.cpp		
		
clean:
	rm -rf *.exe *.o  *.bf