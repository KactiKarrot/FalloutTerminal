#!/bin/bash
g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o main.cpp /usr/lib/libncurses.so && ./main.o
# Rpi location: /usr/lib/arm-linux-gnueabihf/libncurses.so
# also for rpi add -lstdc++fs after main.cpp