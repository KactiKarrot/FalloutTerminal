#include "FalloutTerminal.hpp"

namespace fs = std::filesystem;

int main() {
                                                                                                    // Start of inits for ncurses
  initscr();                                                                                        // Initiate ncurses window
  raw();                                                                                            // Required for Ctrl + S use
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
                                                                                                    // End of inits
  int y, x;                                                                                         // Declare variables for getyx();
  int ch;                                                                                           // Declare ch int used for getch to determine presed key
  std::string cmdInput = "";                                                                        // Defines empty string for commmand line input

  print_static();                                                                                   // Write general static info to top of screen

  int mode = 1;
  int inputLength = 0;
  while(mode >=0) {
    getyx(stdscr, y, x);
    switch(mode) {                                                                                  // Detects which is the current mode, cmd or nav
      case 0:
        ch = getch();                                                                               // Gets int key code of pressed key
        switch(ch) {
          case 10:                                                                                  // Return key
            cmdInput = "";
            for (int i = 0; i < COLS - 2; i++) {
              cmdInput = cmdInput + (char)(mvinch(y, i + 2) & A_CHARTEXT);
            }
            rtrim(cmdInput);
            move(y, x);
            run_command(cmdInput);
            break;
          case 27:                                                                                  // Escape key
            endwin();
            return 0;
            break;
          case 274:                                                                                 // F10 key
            endwin();
            return 0;
            break;
          case 127:                                                                                 // Backspace key for Konsole
            getyx(stdscr, y, x);
            if (x > 2) {
              move(y, x-1);
              delch();
              inputLength--;
            }
            break;
          case KEY_BACKSPACE:                                                                       // Backspace key for TTY
            getyx(stdscr, y, x);
            if (x > 2) {
              move(y, x-1);
              delch();
              inputLength--;
            }
            break;
          case KEY_LEFT:                                                                            // Move cursor left
            move(y, x - 1);
            break;
          case KEY_RIGHT:                                                                           // Move cursor right
            move(y, x + 1);
            break;
          case KEY_UP:                                                                              // Disable up arrow key
            break;
          case KEY_DOWN:                                                                            // Disable down arrow key
            break;
          case KEY_DC:                                                                              // Disable delete key
            break;
          case KEY_IC:                                                                              // Switch to Nav mode
            mode = 1;
            break;
          default:                                                                                  // Handle all other inputs
            echochar(ch);
            inputLength++;
            break;
        }
        wrefresh(stdscr);
        break;
      case 1:
        select_entry(selectedEntryNum);
        ch = getch();                                                                               // Gets int key code of pressed key
        switch(ch) {
          case KEY_UP:                                                                              // Move selection up
            nav_up();
            break;
          case KEY_DOWN:                                                                            // Move selection down
            nav_down();
            break;
          case KEY_IC:                                                                              // Insert key
            mode = 0;
            move(LINES-1, 2);
            wrefresh(stdscr);
            break;
          case 10:                                                                                  // Return key
            open_entry(selectedEntryNum);
            break;
          case 127:                                                                                 // Backspace key for Konsole
            if (fs::current_path() != rootPath) {
              fs::current_path(fs::current_path().parent_path());
              clear_menu();
              list_files();
            }
            break;
          case KEY_BACKSPACE:                                                                       // Backspace key for TTY
            if (fs::current_path() != rootPath) {
              fs::current_path(fs::current_path().parent_path());
              clear_menu();
              list_files();
            }
            break;
          case KEY_DC:                                                                              // Delete file or folder
            if (hasEnding(filePathList[selectedEntryNum], ".folder")) {
              run_command("rmdir " + filePathList[selectedEntryNum].string());
            } else {
              run_command("rm " + filePathList[selectedEntryNum].string());
            }
            break;
          case 27:                                                                                  // Escape key
            endwin();
            return 0;
            break;
          case 274:                                                                                 // F10 key
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
