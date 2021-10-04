//This program currently reads a predetermined file and one can edit the one line but not save. I need to add support for arrow key navigation as well as insertion of text instead of overwriting. Line scrolling and page scrolling need to be added

#include "../FalloutTerminalLib.hpp"

void scroll_down() {
}

void scroll_up() {
}

void inputCh(int ch) {
  int y, x;
  getyx(stdscr, y, x);
  addch(ch);
  //moveCursor(y, x+1);
}

extern "C" int start() {
  int y, x; //initialize variables for getyx();
  int ch; //initialize ch variable required by getch() for input handling
  std::string entryContent;
  std::string entryName = "textFile.entry";//"textFile.entry";
  std::ofstream saveFile;
  char myChar;
  std::vector<std::string> entryContentVector;

  clear_menu();
  move(LINES-1, 0);
  clrtoeol();
  wrefresh(stdscr);
  std::string bottomText = "> Editing " + entryName + "...";
  addstr(bottomText.c_str());
  int currentLine = 6;
  std::ifstream openFile(entryName);
  int i = 0;

  while (getline(openFile, entryContent)) {
    //insert entryContent into vector[i]
    entryContentVector.push_back(entryContent);
    move(currentLine, 0);
    wrefresh(stdscr);
    //term_echo(entryContent, 50, COLS, 0, true);
    term_echo(entryContentVector.at(i), 50, COLS, 0, true);
    currentLine++;
    i++;
    if (currentLine == LINES-2) {
      break;
    }
  }

  bool loop = true;
  int inputLength = 0;
  move(6, 0);

  //Start of keys
  //gets int key code of pressed key
  while (loop == true) {
    ch = getch();
    getyx(stdscr, y, x);
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
        if (x > 0) {
          move(y, x-1);
          delch();
            inputLength--;
        }
        break;
      //Backspace key for TTY
      case KEY_BACKSPACE:
        if (x > 0) {
          move(y, x-1);
          delch();
          inputLength--;
        }
        break;
      //Disable left arrow key
      case KEY_LEFT:
        if (x > 0) {
          move(y, x - 1);
        }
        break;
      //Disable right arrow key
      case KEY_RIGHT:
        if (x < COLS) {
          move(y, x + 1);
        }
        break;
      //Disable up arrow key
      case KEY_UP:
        if (y > 6) {
          move(y - 1, x);
        }
        break;
      //Disable down arrow key
      case KEY_DOWN:
        if (y < (LINES - 2)) {
          move(y + 1, x);
        }
        break;
      //Disable delete key
      case KEY_DC:
        break;
      //Switch to Nav mode
      case KEY_IC:
        break;
      //Handle CTRL+S
      case 19:
        saveFile.open(entryName, std::ofstream::out | std::ofstream::trunc);
        char myChar;
        //Add for loop that saves chars on screen into file
        getyx(stdscr, y, x);
        move(6, 0);
        for (int i = 0; i < LINES - 7; i++) {
          
          for (int j = 0; j < COLS; j++) {
            if (i > 5 && i < (LINES - 1)) {
              myChar = (mvinch(i, j) & A_CHARTEXT);
              saveFile << myChar;
              //saveFile << entryContentVector.at(i);
            }
          }
          //saveFile << '\n';

        }
        move(y, x);
        break;
      //Handle all other inputs
      default:
        inputCh(ch);
        break;
    }
    wrefresh(stdscr);
  }
  //end of keys

  endwin();
  return 0;
}