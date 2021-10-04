//Required includes
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <filesystem>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <dlfcn.h>
#include <algorithm>
#include <cctype>
#include <locale>

//Prints a string on char at a time with specified delay, using echochar() to do so
void term_echo(std::string str, int delay, int COLS = 0, int currentCol = -1, bool instSpace = false) {
  for(char c : str) {
    if (c == ' ' && instSpace == true) {
      echochar(c);
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(delay));
      //std::cout << c << std::flush; //unneeded now, kept for future reference
      echochar(c);
    }
  }
}

//Place a string in the middle of the sreen using term_echo()
void center_print(int COLS, int currentLine, std::string str, int delay = 50) {
  int mid = COLS/2;
  int offset = str.length()/2;
  move(currentLine, mid-offset);
  wrefresh(stdscr);
  term_echo(str, delay);
}

//Clears the menu for editing
void clear_menu() {
  int currentLine = 6;
  while (currentLine <= LINES-3) {
    move(currentLine, 0);
    wrefresh(stdscr);
    clrtoeol();
    wrefresh(stdscr);
    currentLine++;
  }
}

// trim from end (in place) from: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}