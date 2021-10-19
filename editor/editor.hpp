#include "../FalloutTerminalLib.hpp"                                                                // Includes functions for working with parent program properly

void removeLastLine(std::vector<std::string> &entryContentVector) {                                 // Removes the last line of the vector
  entryContentVector.pop_back();
}

bool isLineBlank(std::vector<std::string> &entryContentVector, int n) {                             // Checks if the last element of the vector is blank
  if (entryContentVector[n] == "") {
    return true;
  } else {
    return false;
  }
}

void scroll_down(int &scrollOffset, std::vector<std::string> &entryContentVector) {                 // Scrolls down one line
  int y, x;                                                                                         // Used for getyx();
  getyx(stdscr, y, x);

  for (int i = 0; i < LINES - 8; i++) {
    move(i + 6, 0);
    wrefresh(stdscr);
    clrtoeol();
    term_echo(entryContentVector[i + scrollOffset], 0);
  }

  move(LINES - 3, x);
  wrefresh(stdscr);
  scrollOffset++;
}

void scroll_up(int &scrollOffset, std::vector<std::string> &entryContentVector) {                   // Scrolls up one line
  int y, x;                                                                                         // Used for getyx();
  getyx(stdscr, y, x);

  for (int i = 0; i < LINES - 8; i++) {
    move(i + 6, 0);
    wrefresh(stdscr);
    clrtoeol();
    term_echo(entryContentVector[i + scrollOffset], 0);
  }

  move(6, x);
  wrefresh(stdscr);
  scrollOffset--;
}

void update_current_line(std::string &str) {                                                        // Updates the string given by reference
  int y, x;
  getyx(stdscr, y, x);
  str = "";                                                                                         // Resets string
  for (int i = 0; i < COLS; i++) {                                                                  // Reads line one char at a time
    str = str + (char)(mvinch(y, i) & A_CHARTEXT);                                                  // Inserts one char
    move(y, i);
  }
  move(y, x);
}