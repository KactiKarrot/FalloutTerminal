#include "FalloutTerminalLib.hpp" //Header with all functions for the terminal

//Note to self: use std::system(file.o); to run external extension software like floppy games

//Global definitions
std::vector<std::string> fileList; //Global initiation of fileList[] vector
std::vector<std::filesystem::path> filePathList; //Global initiation of fullFileList[] vector
int selectedEntryNum = 0; //Global initiation of selectedEntryNum int that stores which entry is currently selected
int entryCount = 0; //Stores how many entries are on current page
int currentLine = 0;
int totalCols;
int directoryLevel = 0;

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
void list_files(int selectedEntry = -1, int page = -1) {
  std::string currentPath = std::filesystem::current_path();
  currentPath = currentPath + "/";
  currentLine = 6;
  entryCount = 0;
  filePathList.clear();
  fileList.clear();
  for (const auto & entry : std::filesystem::directory_iterator(currentPath)) {
    std::filesystem::path entryPath = entry.path();
    std::string fullEntryName = entry.path();
    std::string entryName;
    entryName = eraseSubstring(fullEntryName, currentPath);
    if (hasEnding(entryName, ".entry") == true || hasEnding(entryName, ".folder") == true) {
      entryName = eraseSubstring(entryName, ".entry");
      entryName = eraseSubstring(entryName, ".folder");
      move(currentLine, 0);
      wrefresh(stdscr);
      term_echo("> ", 50, COLS, 0);
      term_echo(entryName, 50, COLS, 2);
      currentLine++;
      fileList.push_back(entryName);
      filePathList.push_back(entryPath);
      entryCount++;
    }
  }
  if (selectedEntry != -1) {
    selectedEntryNum = selectedEntry;
  }
}

//Opens n entry from the list
void open_entry(int n) {
  int fileType; //specifies filetype of input file, 0 = text, 1 = folder, 2 = executable
  std::string entryName = filePathList[n];
  char ch;
  if (hasEnding(entryName, ".entry") == true) {
    fileType = 0;
  } else if (hasEnding(entryName, ".folder") == true) {
    fileType = 1;
  } else if (hasEnding(entryName, ".program") == true) {
    fileType = 2;
  }
  std::string entryContent;
  switch(fileType) {
    case 0: {
      //open text file
      clear_menu();
      move(LINES-1, 2);
      clrtoeol();
      wrefresh(stdscr);
      addstr("Accessing...");
      currentLine = 6;
      std::ifstream openFile(entryName);
      while (getline(openFile, entryContent)) { //This currently works, it prints the entry and waits to print a second page if required. It will make mistakes if a line that is too long is in the input file. This would done by comparing entryContent.length() to COLS, however this may not be needed as files created in my editor will not be able to keep going past the edge of the screen
        move(currentLine, 0);
        wrefresh(stdscr);
        term_echo(entryContent, 50, COLS, 0);
        currentLine++;
        if (currentLine == LINES-2) {
          ch = getch();
          while (ch != 10 && ch != KEY_BACKSPACE && ch != 127) {
            ch = getch();
          }
          clear_menu();
          currentLine = 6;
        }
      }
      //While enter has not been pressed
      ch = getch();
      while (ch != 10 && ch != KEY_BACKSPACE && ch != 127) {
        ch = getch();
      }
      move(LINES-1, 1);
      wrefresh(stdscr);
      clrtoeol();
      clear_menu();
      list_files();
      break;
    }
    case 1:
      //open folder
      std::filesystem::current_path(filePathList[n]);
      clear_menu();
      list_files(0);
      break;
    case 2:
      //open executable file
      break;
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
  totalCols = COLS;
  //std::string rootDir = std::filesystem::current_path();

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
  term_echo(title, 50, COLS, 0);
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
  term_echo("> ", 50, COLS, 0);

  //Init user control
  int mode = 1;
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
            //std::system("./ext.o")
            open_entry(selectedEntryNum);
            //endwin();
            //return 0;
            break;
          //Backspace key for Konsole
          case 127:
            std::filesystem::current_path(std::filesystem::current_path().parent_path());
            clear_menu();
            list_files();
            break;
          //Backspace key for TTY
          case KEY_BACKSPACE:
            std::filesystem::current_path(std::filesystem::current_path().parent_path());
            clear_menu();
            list_files();
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
