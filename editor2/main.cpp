#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>

int main() {
  initscr();
  raw();
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  addch('#');
  if ((mvinch(0, 0) & A_CHARTEXT) == '#') {
     std::cout << "True" << std::endl;
  }
  std::cout << (mvinch(0, 0) & A_CHARTEXT) << std::endl;
  char ch = (mvinch(0, 0) & A_CHARTEXT);
  std::cout << ch << std::endl;

  int x, y;
  std::vector<std::string> fileContent;

  char ch2;
  while (ch2 != 'a') {
    ch2 = getch();
    std::cout << ch2;
    int tmpint = 19;
    if (ch2 == 19) {
      std::cout << "return true;" << std::endl;
    }
  }
  std::string tmp = keyname(ch2);
  /*
  std::ifstream openFile;
  openFile.open("test.txt");
  std::string line;
  int i = 0;
  while(std::getline(openFile, line)) {
    getyx(stdscr, y, x);
    if (y < LINES) {//This would be y < LINES - 7 in the real program
      addstr(line.c_str());
      getyx(stdscr, y, x);
      move(y + 1, 0);
    }
    fileContent[i] = line;
    i++;
  }
  /*

  /*
  std::ofstream openFile;
  openFile.open("test.txt", std::ofstream::out | std::ofstream::trunc);
  char myChar;
  //Add for loop that saves chars on screen into file
  for (int i = 0; i < LINES; i++) {
    for (int j = 0; j < COLS; j++) {
      myChar = (mvinch(i, j) & A_CHARTEXT);
      if (myChar != ' ') {
        openFile << myChar;
      }
    }
  }
  //Need to change LINES in this segment ot LINES-7
  */
  ch2 = getch();
  endwin();
  std::cout << (int)ch2 << std::endl;
}