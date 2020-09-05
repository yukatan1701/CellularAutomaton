CC=g++
CFLAGS=-Wall -lGL -lGLU -lglut

%: %.cpp
	g++ $@.cpp automaton.cpp -o $@ $(CFLAGS) 
