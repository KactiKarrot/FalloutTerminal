#bin/bash
g++ -c -fPIC editor.cpp -o editor.o
g++ -shared editor.o -o editor.program
