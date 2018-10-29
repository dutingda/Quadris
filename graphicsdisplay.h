#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include <memory>
#include <string>
#include <vector>
#include "window.h"
#include "cell.h"
#include "block.h"
#include "type.h"

class GraphicsDisplay{
  // the size of the width of each square, left top right bottom boarder in display
  int cellSize, leftOffset, topOffset, rightOffset, bottomOffset;
  std::shared_ptr<Xwindow> theDisplay;              // The Xwindow used to display the grid
  std::vector<std::vector <Cell> > * board; // the pointer to the board that contains vectors of vectors of Cell
  
  // previous board
  std::vector<std::vector <Cell> > prev;
  // methods for drawing strings and cell using X11 library
  void drawCell(int row, int col, int colour);
  void drawText(int row, int col, std::string txt);
  void drawText(int row, int col, int i);
  // getting the colour of the current cell
  int getColour(Content c);

  // where to show the next block in the graphics window
  int promptRow;
    
 public:
  GraphicsDisplay(int width = 11, int height = 18,
                  int cellSize = 25,
                  int leftOffset = 25, int topOffset = 25,
                  int rightOffset = 175, int bottomOffset = 0);
  // update the grid
  void setGrid(std::vector<std::vector<Cell>> * board);
  // show on the display for information like scores and level
  void showInfo(int lvl, int total, int MaxSofar);
  // show the next block
  void showNextBlock(Block b);
  // display game over message
  void showGameOver();
  // overloading operater to print on main function
  friend std::ostream &operator<<(std::ostream &out, GraphicsDisplay &gd);
};

#endif

