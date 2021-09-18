#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <Buffer.hpp>

class Editor {
  private:
    int x;
    int y;
    char mode;
    Buffer* buffer;
    std::string status;
    std::string fileName;
    std::string tos(int);
    
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void deleteLine();
    void deleteLine(int);
    void saveFile();

  public:
    Editor();
    Editor(std::string);

    char getMode() {return mode;}
    void handleInput(int);
    void printBuff();
    void printStatusLine();
    void updateStatus();

}