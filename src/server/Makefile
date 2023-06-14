# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -lpthread

# ****************************************************
# Targets needed to bring the executable up to date
# Server File name : main.cpp headers : ../utils/global.h

DEPS = ../utils/global.h
OBJ = server.o

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

server: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^



clean:
	rm -f *.o server

# End of Makefile

