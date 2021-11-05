#!/bin/bash
if [ -f "/usr/lib/libncurses.so" ]; then
  g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o FalloutTerminal.cpp /usr/lib/libncurses.so -ldl && ./main.o
else
  g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o FalloutTerminal.cpp /usr/lib/x86_64-linux-gnu/libncurses.so -ldl && ./main.o
fi
# g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o FalloutTerminal.cpp /usr/lib/x86_64-linux-gnu/libncurses.so -ldl && ./main.o
# g++ -std=c++17 -Wall -Weffc++ -Wextra -Wsign-conversion -o main.o FalloutTerminal.cpp /usr/lib/libncurses.so -ldl && ./main.o
# Rpi location: /usr/lib/arm-linux-gnueabihf/libncurses.so
# also for rpi add -lstdc++fs after main.cpp
