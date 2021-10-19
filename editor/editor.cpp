//This program currently reads a predetermined file. I need to add support for insertion of text instead of overwriting. Line scrolling needs to be added

#include "editor.hpp"

extern "C" int start() {
                                                                                                    // Start of variable delcaration
  int y, x;                                                                                         // Variables for getyx();
  int ch;                                                                                           // Variable for getch() input handling
  std::ifstream paramFile("paramFile");
  std::string params;
  std::string entryName;
  std::string entryContent;                                                                         // Temporarily stores one line of the file
  std::ifstream openFile;                                                                           // Used to read the file
  std::ofstream saveFile;                                                                           // Used to save the file
  std::vector<std::string> entryContentVector;                                                      // Stores the file to be edited saved into the file later
  int scrollVerticalOffset = 0;                                                                     // Current amount of lines scrolled down
  int currentLine = 6;                                                                              // Line to print to when reading
  int currentIndex = 0;                                                                             // Index of vector to print from
  std::string bottomText;
                                                                                                    // End of variable delcaration

  getline(paramFile, params);
  entryName = params;
  paramFile.close();
  bottomText = "> Editing " + entryName + "...";                                                    // Text for bottom of the screen
  openFile.open(entryName);

  clear_menu();                                                                                     // Prepares menu to print file contents
  move(LINES-1, 0);                                                                                 // Move to bottom of screen
  clrtoeol();
  wrefresh(stdscr);
  addstr(bottomText.c_str());

  while (getline(openFile, entryContent)) {                                                         // Next line of file  
    entryContentVector.push_back(entryContent);                                                     // Insert entryContent into vector[i]
    if (currentLine < LINES - 2) {
      move(currentLine, 0);
      wrefresh(stdscr);
      term_echo(entryContentVector[currentIndex], 50, COLS, 0, true);
      currentLine++;
      currentIndex++;
    }
  }
  openFile.close();                                                                                 // Closes file after reading

  if (entryContentVector.size() == 0) {                                                             // Makes sure the vector isn't empty
    entryContentVector.push_back("");                                                               // Inserts empty first element
  }

  move(6, 0);

  while (true) {                                                                                    // Start of input handling
    ch = getch();                                                                                   // Gets int key code of pressed key
    getyx(stdscr, y, x);
    switch(ch) {
      case 10:                                                                                      // Return key
        return 0;
        break;
      case 27:                                                                                      // Escape key
        return 0;
        break;
      case 274:                                                                                     // F10 key
        return 0;
        break;
      case 127:                                                                                     // Backspace key for Konsole
        if (x > 0) {
          move(y, x-1);
          delch();
        }
        update_current_line(entryContentVector[(y - 6) + scrollVerticalOffset]);                    // Update the vector
        break;
      case KEY_BACKSPACE:                                                                           // Backspace key for TTY
        if (x > 0) {
          move(y, x-1);
          delch();
        }
        update_current_line(entryContentVector[(y - 6) + scrollVerticalOffset]);                    // Update the vector
        break;
      case KEY_LEFT:                                                                                // Move cursor left
        if (x > 0) {
          move(y, x - 1);
        }
        break;
      case KEY_RIGHT:                                                                               // Move cursor right
        if (x < COLS) {
          move(y, x + 1);
        }
        break;
      case KEY_UP:                                                                                  // Disable up arrow key
        if (y > 6) {
          move(y - 1, x);
        } else if (scrollVerticalOffset >= 0) {
          scroll_up(scrollVerticalOffset, entryContentVector);
        }
        break;
      case KEY_DOWN:                                                                                // Move cursor down
        if (y < (LINES - 3)) {                                                                      // If cursor is not at bottom of screen
          move(y + 1, x);                                                                           // move cursor down     
          if (y - 6 >= entryContentVector.size() - 1) {
            entryContentVector.push_back("");
          }
        } else if (scrollVerticalOffset <= entryContentVector.size() - (LINES - 8)) {               // if not at end of file
          scroll_down(scrollVerticalOffset, entryContentVector);                                    // scroll down
        } else {                                                                                    // if at end of file
          entryContentVector.push_back("");                                                         // insert empty line into vector for the vector to update properly
          scroll_down(scrollVerticalOffset, entryContentVector);                                    // scroll down
        }
        break;
      case KEY_DC:                                                                                  // Disable delete key
        break;
      case KEY_IC:                                                                                  // Disable insert key
        break;
      case 19:                                                                                      // Handle CTRL+S
        saveFile.open(entryName, std::ofstream::out | std::ofstream::trunc);                        // Open file to save to

        getyx(stdscr, y, x);
        move(6, 0);                                                                                 //Move cursor to beginning of the window content

        while (isLineBlank(entryContentVector, entryContentVector.size() - 1)) {                    // Check that the last line of the vector is empty
          removeLastLine(entryContentVector);                                                       // Clear the last line
        }

        for (int i = 0; i < entryContentVector.size(); i++) {
          rtrim(entryContentVector[i]);                                                             // Remove tailing whitespaces
          saveFile << entryContentVector[i] << std::endl;                                           // Save element into file
        }
        saveFile.close();                                                                           // Close the file to save changes
        move(y, x);                                                                                 // Return to previous cursor position
        break;
      
      default:                                                                                      // Handle all other inputs
        addch(ch);                                                                                  // Insert the key pressed into the window
        refresh();
        update_current_line(entryContentVector[(y - 6) + scrollVerticalOffset]);                    // Update the vector
        break;
    }
    refresh();
  }
                                                                                                    // End of input handling
  return 0;                                                                                         // Exit editor
}