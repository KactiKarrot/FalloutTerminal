#include "FalloutTerminalLib.hpp"                                                                   // Header with all functions for the terminal

namespace fs = std::filesystem;

                                                                                                    // Global declarations
std::string title = "KactiKarrot Log Terminal";                                                     // Temporary title for this terminal; will be taken from file
std::vector<std::string> fileList;                                                                  // Global declaration of fileList[] vector
std::vector<fs::path> filePathList;                                                                 // Global declaration of filePathList[] vector
int selectedEntryNum = 0;                                                                           // Global initiation of selectedEntryNum int that stores which entry is currently selected
int entryCount = 0;                                                                                 // Stores how many entries are on current page
int currentLine = 0;
int directoryLevel = 0;
int fileScrollOffset = 0;                                                                           // Global initiation of int to store how much the file list has scrolled
std::string entryName;                                                                              // Global declaration of entry name variable
std::string entryToOpen;                                                                            // Global declaration of entry to open for editor
std::ofstream paramFile;                                                                            // Used for passing params to .programs
std::string rootPath = fs::current_path();                                                          // Used to know location of built in programs

bool hasEnding(std::string const &fullString, std::string const &ending) {                          // Test if a string ends with a specified substring
  if (fullString.length() >= ending.length()) {
    return(0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

std::string eraseSubstring(std::string fullString, std::string subString) {                         // Erase specified substring from a string
  std::size_t pos = fullString.find(subString);
  if (pos != std::string::npos) {
    fullString.erase(pos, subString.length());
  }
  return fullString;
}

void run_so(std::string fileName, std::string params = "", bool editor = false) {                   // Open shared object library and run function to start addon program
  std::string tmpPath = fs::current_path();
  if (editor) {
    fs::current_path() = rootPath + "editor";
  }
  paramFile.open("paramFile", std::ofstream::out | std::ofstream::trunc);                           // Open file to save to
  paramFile << params;
  paramFile.close();
  fs::current_path() = tmpPath;
  void* handle = dlopen(fileName.c_str(), RTLD_LAZY);
  typedef void (*test_t)();
  test_t start = (test_t) dlsym(handle, "start");
  start();
}

void select_entry(int n) {                                                                          //  Selects entry n from the list
  move(6+n, 2);
  wrefresh(stdscr);
  clrtoeol();
  attron(A_STANDOUT);
  addstr(fileList[n].c_str());
  attroff(A_STANDOUT);
  wrefresh(stdscr);
}

void deselect_entry(int n) {                                                                        // Deselects entry n from the list
  move(6+n, 2);
  wrefresh(stdscr);
  clrtoeol();
  addstr(fileList[n].c_str());
  wrefresh(stdscr);
}

void nav_up() {                                                                                     //  Moves down one selection
  if (selectedEntryNum > 0) {
    deselect_entry(selectedEntryNum);
    selectedEntryNum--;
    select_entry(selectedEntryNum);
  }
}

void nav_down() {                                                                                   // Moves down one selection
  if (selectedEntryNum < entryCount-1) {
    deselect_entry(selectedEntryNum);
    selectedEntryNum++;
    select_entry(selectedEntryNum);
  }
}

void list_files(int selectedEntry = -1, int page = -1, int printRate = 50) {                        // Lists all files of the current directory of specified extensions into the menu
  std::string currentPath = fs::current_path();
  currentPath = currentPath + "/";
  currentLine = 6;
  entryCount = 0;
  filePathList.clear();
  fileList.clear();
  for (const auto & entry : fs::directory_iterator(currentPath)) {
    fs::path entryPath = entry.path();
    std::string fullEntryName = entry.path();
    entryName = eraseSubstring(fullEntryName, currentPath);
    if (hasEnding(entryName, ".entry") || hasEnding(entryName, ".folder") || hasEnding(entryName, ".program")) {
      entryName = eraseSubstring(entryName, ".entry");
      entryName = eraseSubstring(entryName, ".folder");
      entryName = eraseSubstring(entryName, ".program");
      move(currentLine, 0);
      wrefresh(stdscr);
      term_echo("> ", printRate, COLS, 0);
      term_echo(entryName, printRate, COLS, 2);
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

void print_static(int delay = 50) {                                                                               // Prints the static info and command line with no delay. Also runs list_files();
  center_print(COLS, 0, "ROBCO INDUSTRIES UNIFIED OPERATING SYSTEM", delay);
  center_print(COLS, 1, "COPYRIGHT 2075-2077 ROBCO INDUSTRIES", delay);
  center_print(COLS, 2, "-Server 56-", delay);

  move(4, 0);
  wrefresh(stdscr);
  term_echo(title, delay, COLS, 0);
  move(5, 0);
  wrefresh(stdscr);
  for (unsigned long i = 0; i != title.length(); i++) {
    term_echo("_", delay);
  }
  list_files();
  select_entry(selectedEntryNum);

  move(LINES-1, 0);
  wrefresh(stdscr);
  term_echo("> ", delay, COLS, 0);
}

void open_entry(int n) {                                                                            // Opens n entry from the list
  int fileType;                                                                                     // Specifies filetype of input file, 0 = text, 1 = folder, 2 = executable
  entryName = filePathList[n];
  char ch;
  if (hasEnding(entryName, ".entry")) {
    fileType = 0;
  } else if (hasEnding(entryName, ".folder")) {
    fileType = 1;
  } else if (hasEnding(entryName, ".program")) {
    fileType = 2;
  }
  std::string entryContent;
  switch(fileType) {
    case 0: {                                                                                       // Open text file
      clear_menu();
      move(LINES-1, 2);
      clrtoeol();
      wrefresh(stdscr);
      addstr("Accessing...");
      currentLine = 6;
      std::ifstream openFile(entryName);
      while (getline(openFile, entryContent)) {                                                     // This currently works, it prints the entry and waits to print a second page if required. It will make mistakes if a line that is too long is in the input file. This would done by comparing entryContent.length() to COLS, however this may not be needed as files created in my editor will not be able to keep going past the edge of the screen
        move(currentLine, 0);
        wrefresh(stdscr);
        term_echo(entryContent, 50, COLS, 0, true);
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
      ch = getch();
      while (ch != 10 && ch != KEY_BACKSPACE && ch != 127) {                                        // While enter has not been pressed
        ch = getch();
      }
      move(LINES-1, 1);
      wrefresh(stdscr);
      clrtoeol();
      clear_menu();
      list_files();
      break;
    }
    case 1:                                                                                         // Open folder
      fs::current_path(filePathList[n]);
      clear_menu();
      list_files(0);
      break;
    case 2:                                                                                         // Open executable file                                                                           // Close the file to save changes
      run_so(entryName);
      clear();
      print_static(0);
      break;
  }
}

void run_command(std::string str) {                                                                 // Runs a commmand inputted in the command line
  if (str.substr(0, 6) == "mkdir ") {                                                               // Make a folder
    fs::create_directory(str.substr(6) + ".folder");
  }

  if (str.substr(0, 6) == "rmdir ") {                                                               // Delete a folder
    fs::remove_all(str.substr(7) + ".folder");
    fs::remove(str.substr(7) + ".folder");
  }

  if (str.substr(0, 3) == "rm ") {                                                                  // Delete a file
    fs::remove(str.substr(3) + ".entry");
  }

  if (str.substr(0, 5) == "edit ") {                                                                // Edit a file
    run_so(rootPath + "/editor/editor.so", str.substr(5) + ".entry", true);
  }

  clear_menu();
  list_files(-1, -1, 0);
  move(LINES - 1, 2);
  clrtoeol();
}