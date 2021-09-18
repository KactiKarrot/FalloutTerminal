//This program currently reads a predetermined file and one can edit the one line but not save. I need to add support for arrow key navigation as well as insertion of text instead of overwriting. Line scrolling and page scrolling need to be added

#include "../FalloutTerminalLib.hpp"

/*void moveCursor(int y = -1, int x = -1, std::string direction = "none") {
  if (y == -1 && direction == "none") {
    return;
  }
  int prevY, prevX;
  getyx(stdscr, prevY, prevX);
  if (direction == "right") {
    y = prevY;
    x = prevX + 1;
  } else if (direction == "left") {
    y = prevY;
    x = prevX - 1;
  } else {

  }
  chgat(1, A_NORMAL, 0, NULL);
  mvchgat(y, x, 1, A_STANDOUT, 0, NULL);
}*/

void inputCh(int ch) {
  int y, x;
  getyx(stdscr, y, x);
  addch(ch);
  //moveCursor(y, x+1);
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
  int y, x; //initialize variables for getyx();
  int ch; //initialize ch variable required by getch() for input handling
  std::string entryContent;
  std::string entryName = "textFile.entry";

  clear_menu();
  move(LINES-1, 0);
  clrtoeol();
  wrefresh(stdscr);
  std::string bottomText = "> Editing " + entryName + "...";
  addstr(bottomText.c_str());
  int currentLine = 6;
  std::ifstream openFile(entryName);

  while (getline(openFile, entryContent)) { //This currently works, it prints the entry and waits to print a second page if required. It will make mistakes if a line that is too long is in the input file. This would done by comparing entryContent.length() to COLS, however this may not be needed as files created in my editor will not be able to keep going past the edge of the screen
    move(currentLine, 0);
    wrefresh(stdscr);
    term_echo(entryContent, 50, COLS, 0);
    currentLine++;
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
        if (x > 0) {
          move(y, x-1);
          delch();
            inputLength--;
        }
        break;
      //Backspace key for TTY
      case KEY_BACKSPACE:
        getyx(stdscr, y, x);
        if (x > 0) {
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
        break;
      //Handle all other inputs
      default:
        inputCh(ch);
        //break;
    }
    wrefresh(stdscr);
  }
  //end of keys

  endwin();
  return 0;
}





/*
      clear_menu();
      move(LINES-1, 2);
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
*/