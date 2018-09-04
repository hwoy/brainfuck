BF = bf
TBF = tbf

CXX = g++

STD =c++11

COL=96

LOOPLIMIT=64

BFCELL=512

TBFCELL=2

BFFLAGS = -DLOOPLIMIT=$(LOOPLIMIT) -DBFCELL=$(BFCELL)
TBFFLAGS = -DCOL=$(COL) -DTBFCELL=$(TBFCELL)

DEBUGFLAGS = -g -Wall -std=$(STD) -pedantic
RELEASEFLAGS = -O2 -Wall -std=$(STD) -pedantic

CXXFLAGS = $(DEBUGFLAGS)

.PHONY: all clean release

all: $(BF) $(TBF)

$(BF): bf.cpp bf.hpp
		$(CXX) -o $(BF) $(CXXFLAGS) $(BFFLAGS) bf.cpp

$(TBF): tbf.cpp bf.hpp
		$(CXX) -o $(TBF) $(CXXFLAGS) $(TBFFLAGS) tbf.cpp


release: $(BF)-r $(TBF)-r

$(BF)-r: bf.cpp bf.hpp
		$(CXX) -o $(BF)-r $(RELEASEFLAGS) $(BFFLAGS) bf.cpp

$(TBF)-r: tbf.cpp bf.hpp
		$(CXX) -o $(TBF)-r $(RELEASEFLAGS) $(TBFFLAGS) tbf.cpp		
		
clean:
	rm -rf *.exe *.o  *.bf $(BF) $(TBF) $(BF)-r $(TBF)-r
