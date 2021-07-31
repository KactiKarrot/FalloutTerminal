#!/bin/bash
g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o main.cpp /usr/lib/libncurses.so && ./main.o
