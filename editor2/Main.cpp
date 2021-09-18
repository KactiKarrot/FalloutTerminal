#include <Main.hpp>

void curses_init() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
}

int main(std::string fileName = "") {
  curses_init();
  refresh();
  endwin();
  return 0;
}