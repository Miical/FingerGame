CXX?=g++
CXXFLAGS?=-O2 -std=c++11 -Wall -g
BINARIES=fingergame

fingergame: fingergame.cc
	$(CXX) $(CXXFLAGS) fingergame.cc -o fingergame

clean:
	rm -f $(BINARIES)
