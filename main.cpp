//for 31/08/21: learn how to make c++ headers and libraries so that I can transfer most of my functions to a library so Nick and I can create extension programs


//Required includes on linux, look for alternatives for windows
//no longer required, have found cross-platform alternative (std::filesystem in c++17)
//#include <sys/stat.h>
//#include <sys/types.h>

//Required includes
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <filesystem>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;

std::vector<std::string> fileList; //Global initiation of fileList[] vector
std::vector<fs::path> filePathList; //Global initiation of fullFileList[] vector
int selectedEntryNum = 0; //Global initiation of selectedEntryNum int that stores which entry is currently selected
int entryCount = 0; //Stores how many entries are on current page

//Test if a string ends with a specified substring
bool hasEnding(std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return(0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

//Erase specified substring from a string
std::string eraseSubstring(std::string fullString, std::string subString) {
  std::size_t pos = fullString.find(subString);
  if (pos != std::string::npos) {
    fullString.erase(pos, subString.length());
  }
  return fullString;
}

//Prints a string on char at a time with specified delay, using echochar() to do so
void term_echo(std::string str, int delay) {
  for(char c: str) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    //std::cout << c << std::flush; //unneeded now, kept for future reference
    echochar(c);
  }
}

//Place a string in the middle of the sreen using term_echo()
void center_print(int COLS, int currentLine, std::string str) {
  int mid = COLS/2;
  int offset = str.length()/2;
  move(currentLine, mid-offset);
  wrefresh(stdscr);
  term_echo(str, 50);
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

//Opens n entry from the list
void open_entry(int n) {
  int fileType; //specifies filetype of input file, 0 = text, 1 = folder, 2 = executable
  std::string entryName = filePathList[n];
  if (hasEnding(entryName, ".entry") == true) {
    fileType = 0;
  } else if (hasEnding(entryName, ".folder") == true) {
    fileType = 1;
  } else if (hasEnding(entryName, ".program") == true) {
    fileType = 2;
  }
  switch(fileType) {
    case 0:
      //open_text
      //clear_menu();
      //fstream::ifstream openFile(entryName);
      break;
    case 1:
      //open_folder
      break;
    case 2:
      //open_executable
      break;
  }
}

//Selects entry n from the list
void select_entry(int n) {
  move(6+n, 2);
  wrefresh(stdscr);
  clrtoeol();
  attron(A_STANDOUT);
  addstr(fileList[n].c_str());
  attroff(A_STANDOUT);
  wrefresh(stdscr);
}

//Deselects entry n from the list
void deselect_entry(int n) {
  move(6+n, 2);
  wrefresh(stdscr);
  clrtoeol();
  addstr(fileList[n].c_str());
  wrefresh(stdscr);
}

//Moves down one selection
void nav_up() {
  if (selectedEntryNum > 0) {
    deselect_entry(selectedEntryNum);
    selectedEntryNum--;
    select_entry(selectedEntryNum);
  }
}

//Moves down one selection
void nav_down() {
  if (selectedEntryNum < entryCount-1) {
    deselect_entry(selectedEntryNum);
    selectedEntryNum++;
    select_entry(selectedEntryNum);
  }
}

//Lists all files of the current directory of specified extensions into the menu
void list_files() {
  std::string currentPath = fs::current_path();
  currentPath = currentPath + "/";
  int currentLine = 6;
  entryCount = 0;
  for (const auto & entry : fs::directory_iterator(currentPath)) {
    fs::path entryPath = entry.path();
    std::string fullEntryName = entry.path();
    std::string entryName;
    entryName = eraseSubstring(fullEntryName, currentPath);
    if (hasEnding(entryName, ".entry") == true || hasEnding(entryName, ".folder") == true) {
      entryName = eraseSubstring(entryName, ".entry");
      entryName = eraseSubstring(entryName, ".folder");
      move(currentLine, 0);
      wrefresh(stdscr);
      term_echo("> ", 50);
      term_echo(entryName, 50);
      currentLine++;
      fileList.push_back(entryName);
      filePathList.push_back(entryPath);
      entryCount++;
    }
  }
}

int main() {
  initscr(); //initiate ncurses window
  //required inits for ncurses
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  //initialize variables for getyx();
  int y;
  int x;
  //initialize ch variable required by getch() for input handling
  int ch;
  //std::string rootDir = fs::current_path();

  //tmp inits
  //This uses the mkdir command built into POSIX compliant operating systems, this means it does not work on windows, instead a solution in std::filesystem should be used
  //mkdir("./This is a folder", 0777);
  std::string title = "KactiKarrot Log Terminal"; //Temporary title for this terminal, subject to change

  //Write general static info to top of screen
  center_print(COLS, 0, "ROBCO INDUSTRIES UNIFIED OPERATING SYSTEM");
  center_print(COLS, 1, "COPYRIGHT 2075-2077 ROBCO INDUSTRIES");
  center_print(COLS, 2, "-Server 56-");

  //Needed vars: file count,
  //file count cannot be at point that requires scrolling, may be necesarry to implement later
  //add a header with metadata like date made, file type, and such

  //draw menu
  move(4, 0);
  wrefresh(stdscr);
  term_echo(title, 50);
  move(5, 0);
  wrefresh(stdscr);
  for (unsigned long i = 0; i != title.length(); i++) {
    term_echo("_", 50);
  }
  list_files();
  select_entry(selectedEntryNum);

  //draw command line
  move(LINES-1, 0);
  wrefresh(stdscr);
  term_echo("> ", 50);

  //Init user control
  int mode = 0;
  int inputLength = 0;
  while(mode >=0) {
    //detects which is the current mode, cmd or nav
    switch(mode) {
      case 0:
        //gets int key code of pressed key
        ch = getch();
        switch(ch) {
          //Return key
          case 10:
            endwin();
            return 0;
            break;
          //Escape key
          case 27:
            endwin();
            return 0;
            break;
          //F10 key
          case 274:
            endwin();
            return 0;
            break;
          //Backspace key for Konsole
          case 127:
            getyx(stdscr, y, x);
            if (x > 2) {
              move(y, x-1);
              delch();
              inputLength--;
            }
            break;
          //Backspace key for TTY
          case KEY_BACKSPACE:
            getyx(stdscr, y, x);
            if (x > 2) {
              move(y, x-1);
              delch();
              inputLength--;
            }
            break;
          //Disable left arrow key
          case KEY_LEFT:
            break;
          //Disable right arrow key
          case KEY_RIGHT:
            break;
          //Disable up arrow key
          case KEY_UP:
            break;
          //Disable down arrow key
          case KEY_DOWN:
            break;
          //Disable delete key
          case KEY_DC:
            break;
          //Switch to Nav mode
          case KEY_IC:
            mode = 1;
            break;
          //Handle all other inputs
          default:
            echochar(ch);
            break;
        }
        wrefresh(stdscr);
        break;
      case 1:
        select_entry(selectedEntryNum);
        //gets int key code of pressed key
        ch = getch();
        switch(ch) {
          case KEY_UP:
            nav_up();
            break;
          case KEY_DOWN:
            nav_down();
            break;
          //Insert key
          case KEY_IC:
            mode = 0;
            move(LINES-1, 2);
            wrefresh(stdscr);
            break;
          //Return key
          case 10:
            open_entry(selectedEntryNum);
            endwin();
            return 0;
            break;
          //Escape key
          case 27:
            endwin();
            return 0;
            break;
          //F10 key
          case 274:
            endwin();
            return 0;
            break;
        }
        break;
    }
  }
  endwin();
  return 0;
}
