#!/bin/bash
g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o main.cpp /usr/lib/x86_64-linux-gnu/libncurses.so -ldl
g++ -c -fPIC lib.cpp -o lib.o
g++ -shared lib.o -o myLib.so
cp myLib.so /usr/lib/myLib.so