#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <vector>
#include <iostream>
#include <memory>
#include "cell.h"
#include "type.h"

// forward Decarlation for avoiding the cyclic dependency
class Grid;
class TextDisplay {
  // the board that contains the cell for text displaying
  std::vector<std::vector <Cell>> * board;
 public:
  // set the grid to update the board
  void setGrid(std::vector<std::vector <Cell>> * board);
  // overloading the function for displaying
  friend std::ostream &operator<<(std::ostream &out, TextDisplay &td);
};
#endif /* TextDisplay_h */

