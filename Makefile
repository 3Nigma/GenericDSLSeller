CPP = g++

all:
	$(CPP) -Wall -g -O0 -std=c++0x main.cpp -o main -llua5.1 -lboost_regex
	./main