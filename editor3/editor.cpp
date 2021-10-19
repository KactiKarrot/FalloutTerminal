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
  std::ifstream openFile;                                                                // Used to read the file
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
  term_echo(params, 0);

  move(6, 0);

  while (true) {                                                                                    // Start of input handling
    ch = getch();                                                                                   // Gets int key code of pressed key
    getyx(stdscr, y, x);
    switch(ch) {
      default:                                                                                      // Handle all other inputs
        addch(ch);                                                                                  // Insert the key pressed into the window
        update_current_line(entryContentVector[(y - 7) + scrollVerticalOffset]);                    // Update the vector
        break;
    }
    wrefresh(stdscr);
  }
                                                                                                    // End of input handling
  return 0;                                                                                         // Exit editor
}