CC=g++
CFLAGS=-Wall -lGL -lGLU -lglut

%: %.cpp
	g++ $@.cpp -o $@ $(CFLAGS) 
