CXX = g++
CXXFLAGS = -g -Wall -std=c++0x

y86dis: y86dis.o Tools.o
	$(CXX) $(CXXFLAGS) y86dis.o Tools.o -o y86dis
y86dis.o: Tools.h y86dis.cpp
Tools.o: Tools.h Tools.cpp
clean:
	rm -f y86dis y86dis.o Tools.o
